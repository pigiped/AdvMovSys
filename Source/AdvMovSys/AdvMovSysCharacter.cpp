// Copyright Epic Games, Inc. All Rights Reserved.

#include "AdvMovSysCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "CrouchingState.h"
#include "WalkingState.h"
#include "SprintingState.h"
#include "ProningState.h"
#include "DefaultState.h"
#include "SlidingState.h"
#include "AdvMovSys.h"

AAdvMovSysCharacter::AAdvMovSysCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(20.f, DefaultState::Get().GetDefaultHalfHeight());

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultState::Get().GetDefaultWalkSpeed();
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AAdvMovSysCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Look);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Walk);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Sprint);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AAdvMovSysCharacter::DoCrouch);
		EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Started, this, &AAdvMovSysCharacter::DoProne);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::DoSlide);


		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Look);

	}
	else
	{
		UE_LOG(LogAdvMovSys, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AAdvMovSysCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAdvMovSysCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}



void AAdvMovSysCharacter::Walk(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		if (Value.Get<bool>())
		{
			SetCharacterState(&WalkingState::Get());
		}
		else
		{
			SetCharacterState(&DefaultState::Get());
		}
	}
}

void AAdvMovSysCharacter::Sprint(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		if (Value.Get<bool>())
		{
			SetCharacterState(&SprintingState::Get());
		}
		else
		{
			SetCharacterState(GetTargetState());
		}
	}
}

void AAdvMovSysCharacter::DoCrouch(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->MaxWalkSpeed < SprintingState::Get().GetSprintWalkSpeed())
	{
		if (CurrentState == &CrouchingState::Get() || CurrentState == &ProningState::Get())
		{
			// We're already crouching or proning, try to get up
			CharacterState* TargetState = GetTargetState();

			SetCharacterState(TargetState);
		}
		else
		{
			SetCharacterState(&CrouchingState::Get());
		}
	}
}

void AAdvMovSysCharacter::DoProne(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		if (CurrentState == &ProningState::Get())
		{
			// We're already prone, try to get up
			CharacterState* TargetState = GetTargetState();

			// Only change state if we can actually get up
			if (TargetState != &ProningState::Get())
			{
				SetCharacterState(TargetState);
				UE_LOG(LogTemp, Display, TEXT("current state: %s"), *UEnum::GetValueAsString(CurrentMovementState));
			}
			else if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Not enough space to get up!"));
			}
		}
		else
		{
			// Go prone
			SetCharacterState(&ProningState::Get());
		}
	}
}

void AAdvMovSysCharacter::DoSlide(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("we slidin"));
	if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->MaxWalkSpeed >= SprintingState::Get().GetSprintWalkSpeed())
	{
		if(Value.Get<bool>() == false)
		{
			SetCharacterState(GetTargetState());
		}
		else
		{
			SetCharacterState(&SlidingState::Get(), Value);
		}
	}
	
}

void AAdvMovSysCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AAdvMovSysCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAdvMovSysCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void AAdvMovSysCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void AAdvMovSysCharacter::SetCharacterState(CharacterState* NewState)
{
	if (CurrentState)
		CurrentState->ExitState(this);

	// Cache previous state
	PreviousState = CurrentState;
	PreviousMovementState = CurrentMovementState;

	// Set new state
	CurrentState = NewState;
	CurrentMovementState = CurrentState->GetStateType();
	
	if (CurrentState)
		CurrentState->EnterState(this);
}

void AAdvMovSysCharacter::SetCharacterState(CharacterState* NewState,const FInputActionValue& Value)
{
	if (CurrentState)
		CurrentState->ExitState(this);
	
	// Cache previous state
	PreviousState = CurrentState;
	PreviousMovementState = CurrentMovementState;

	// Set new state
	CurrentState = NewState;
	CurrentMovementState = CurrentState->GetStateType();

	if (CurrentState)
	{
		CurrentState->EnterState(this);
		CurrentState->HandleInput(this, Value);
	}
}

// sets state for blueprint use
void AAdvMovSysCharacter::SetMovementState(ECharacterMovementState NewState)
{
	switch (NewState)
	{
	case ECharacterMovementState::Default:
		SetCharacterState(&DefaultState::Get());
		break;
	case ECharacterMovementState::Walking:
		SetCharacterState(&WalkingState::Get());
		break;
	case ECharacterMovementState::Sprinting:
		SetCharacterState(&SprintingState::Get());
		break;
	case ECharacterMovementState::Crouching:
		SetCharacterState(&CrouchingState::Get());
		break;
	case ECharacterMovementState::Proning:
		SetCharacterState(&ProningState::Get());
		break;
	case ECharacterMovementState::Sliding:
		SetCharacterState(&SlidingState::Get());
		break;
	default:
		UE_LOG(LogAdvMovSys, Warning, TEXT("AAdvMovSysCharacter::SetMovementState: Unknown state type!"));
		break;
	}
}

// Determine if we can stand up and how much, SHOULD SWEEP INSTEAD OF LINETRACE
CharacterState* AAdvMovSysCharacter::GetTargetState() const
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return &DefaultState::Get();

	UWorld* World = GetWorld();
	if (!World) return &DefaultState::Get();

	FVector Start = GetActorLocation() - FVector(0, 0,Capsule->GetScaledCapsuleHalfHeight());

	// First, try to stand up fully
	FVector End = Start + FVector(0.f, 0.f, DefaultState::Get().GetDefaultHalfHeight() * 2);
	bool bBlocked = World->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(World, Start, End, bBlocked ? FColor::Red : FColor::Green, false, 2.0f, 0, 2.0f);
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredComponent(Capsule);
	//QueryParams.AddIgnoredActor(this);
	//QueryParams.bTraceComplex = true;
	//bool bBlocked = World->SweepTestByChannel(Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), DefaultState::Get().GetDefaultHalfHeight()), QueryParams);
	//FHitResult HitResult;
	//World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeCapsule(Capsule->GetScaledCapsuleRadius(), DefaultState::Get().GetDefaultHalfHeight()), QueryParams);
	//DrawDebugCapsule(World, (Start+End)/2, DefaultState::Get().GetDefaultHalfHeight(), Capsule->GetScaledCapsuleRadius(), FQuat::Identity, bBlocked ? FColor::Red : FColor::Green, false, 2.0f, 0, 2.0f);
	//
	//// Draw purple point at hit location
	//if (HitResult.bBlockingHit)
	//{
	//	DrawDebugPoint(World, HitResult.ImpactPoint, 10.0f, FColor::Purple, false, 2.0f);
	//}
	//
	//UE_LOG(LogTemp, Display, TEXT("HitResult: %s"), *HitResult.ToString());

	if (!bBlocked)
	{
		// Can stand up fully
		UE_LOG(LogTemp, Display, TEXT("Can stand up from prone"));
		return &DefaultState::Get();
	}

	// Can't stand, try crouching
	End = Start + FVector(0.f, 0.f, CrouchingState::Get().GetCrouchedHalfHeight() * 2);
	bBlocked = World->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(World, Start, End, bBlocked ? FColor::Red : FColor::Yellow, false, 2.0f, 0, 2.0f);

	if (!bBlocked)
	{
		// Can crouch
		UE_LOG(LogTemp, Display, TEXT("Can crouch from prone"));
		return &CrouchingState::Get();
	}

	// Can't crouch either, stay prone
	UE_LOG(LogTemp, Display, TEXT("Must stay prone - no space"));
	return &ProningState::Get();
}

ECharacterMovementState AAdvMovSysCharacter::GetTargetMovementState() const
{
	return GetTargetState()->GetStateType();
}

void AAdvMovSysCharacter::SetWalkSpeed(float NewWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;
}

void AAdvMovSysCharacter::RecalculateCapsuleHalfHeight(float NewHalfHeight)
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return;
	float PresentHeight = Capsule->GetScaledCapsuleHalfHeight();
	float OldZ = GetMesh()->GetRelativeLocation().Z;
	Capsule->SetCapsuleHalfHeight(NewHalfHeight);
	FVector NewLocation = GetMesh()->GetRelativeLocation();
	NewLocation.Z += (PresentHeight - NewHalfHeight);
	GetMesh()->SetRelativeLocation(NewLocation);
	UE_LOG(LogTemp, Display, TEXT("Passing from %f to %f, newLocation is %f, before it was: %f"), PresentHeight, NewHalfHeight, NewLocation.Z, OldZ);
}

void AAdvMovSysCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	
	EMovementMode CurrentMode = GetCharacterMovement()->MovementMode;
	
	// Character started falling
	if (CurrentMode == MOVE_Falling && PrevMovementMode != MOVE_Falling)
	{
		UE_LOG(LogTemp, Display, TEXT("Character started falling!"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Started Falling!"));
		}
		
		bool bTheresALedge = GetWorld()->SweepTestByChannel(GetActorLocation(), GetActorLocation() + FVector(0, 0, -200), FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeCapsule(GetCapsuleComponent()->GetScaledCapsuleRadius(), GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));
	}
}