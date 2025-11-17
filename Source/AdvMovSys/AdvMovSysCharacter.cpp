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
#include "AdvMovSys.h"

AAdvMovSysCharacter::AAdvMovSysCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(20.f, StandingHalfHeight);

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
	GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
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
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::DoCrouch);
		EnhancedInputComponent->BindAction(ProneAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::DoProne);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Triggered, this, &AAdvMovSysCharacter::Slide);


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

void AAdvMovSysCharacter::RecalculateCapsuleHalfHeight(float NewHalfHeight)
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return;
	float PresentHeight = Capsule->GetScaledCapsuleHalfHeight();
	float OldZ = GetMesh()->GetRelativeLocation().Z;
	Capsule->SetCapsuleHalfHeight(NewHalfHeight);
	FVector NewLocation = GetMesh()->GetRelativeLocation();
	NewLocation.Z += (PresentHeight-NewHalfHeight);
	GetMesh()->SetRelativeLocation(NewLocation);
	UE_LOG(LogTemp, Display, TEXT("Passing from %f to %f, newLocation is %f, before it was: %f"), PresentHeight, NewHalfHeight, NewLocation.Z, OldZ);
}

void AAdvMovSysCharacter::Walk(const FInputActionValue& Value)
{
	bool ShouldWalk = Value.Get<bool>();

	if (ShouldWalk)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkWalkSpeed;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
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
			SetCharacterState(&WalkingState::Get());
		}
	}
}

void AAdvMovSysCharacter::DoCrouch(const FInputActionValue& Value)
{
	if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->MaxWalkSpeed < SprintWalkSpeed)
	{
		SetCharacterState(&CrouchingState::Get());
	}
}

void AAdvMovSysCharacter::DoProne(const FInputActionValue& Value)
{
	/*bool ShouldProneInput = Value.Get<bool>();

	if (ShouldProneInput)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			if (!bIsProne)
			{
				Prone();
			}
			else
			{
				UnProne();
			}
		}
	}*/
}

void AAdvMovSysCharacter::Prone()
{
	bIsProne = true;
	RecalculateCapsuleHalfHeight(PronedHalfHeight);
	GetCharacterMovement()->MaxWalkSpeed = PronedWalkSpeed;

	RecalculateBaseEyeHeight();

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Prone"));
}

void AAdvMovSysCharacter::UnProne()
{
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return;

	FVector Start = GetActorLocation() - FVector(0,0,Capsule->GetScaledCapsuleRadius());
	FVector End = Start + FVector(0.f, 0.f, CrouchedHalfHeight);
	
	//Debug Line
	bool bBlocked = GetWorld()->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 5.0f);

	UE_LOG(LogTemp, Display, TEXT("bBlocked %s"), bBlocked ? TEXT("true") : TEXT("false"));

	if (!bBlocked)
	{
		bIsProne = false;
		RecalculateCapsuleHalfHeight(CrouchedHalfHeight);
		GetCharacterMovement()->MaxWalkSpeed = NormalWalkSpeed;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnProne"));
	}
}

void AAdvMovSysCharacter::Slide(const FInputActionValue& Value)
{
	//if (!GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->MaxWalkSpeed < SprintWalkSpeed)
	//	return;

	//UCapsuleComponent* Capsule = GetCapsuleComponent();
	//if (!Capsule) 
	//	return;

	//if (Value.Get<bool>())
	//{
	//	float CurrentHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	//	float HeightDifference = CurrentHalfHeight - PronedHalfHeight;

	//	// Compensa la posizione verticale dell'attore per mantenerlo a terra
	//	FVector NewLocation = GetActorLocation();
	//	NewLocation.Z -= HeightDifference;
	//	SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	//	// Aggiusta anche la mesh
	//	RecalculateCapsuleHalfHeight(PronedHalfHeight);

	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Slide"));
	//	bIsSliding = true;
	//}
	//else
	//{
	//	float CurrentHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	//	float HeightDifference = StandingHalfHeight - CurrentHalfHeight;

	//	FVector Start = GetActorLocation() - FVector(0, 0, Capsule->GetScaledCapsuleRadius());
	//	FVector End = Start + FVector(0.f, 0.f, StandingHalfHeight * 2);

	//	//Debug Line
	//	bool bBlocked = GetWorld()->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	//	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 5.0f);

	//	if (!bBlocked)
	//	{
	//		// Compensa la posizione
	//		FVector NewLocation = GetActorLocation();
	//		NewLocation.Z += HeightDifference;
	//		SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	//		// Aggiusta la mesh
	//		RecalculateCapsuleHalfHeight(StandingHalfHeight);
	//	}
	//	else
	//	{
	//		End = Start + FVector(0.f, 0.f, CrouchedHalfHeight * 2);
	//		bBlocked = GetWorld()->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	//		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 5.0f);
	//		if (!bBlocked)
	//		{
	//			// Compensa la posizione
	//			//FVector NewLocation = GetActorLocation();
	//			//NewLocation.Z += (CrouchedHalfHeight - CurrentHalfHeight);
	//			//SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	//			//// Aggiusta la mesh
	//			//RecalculateCapsuleHalfHeight(CrouchedHalfHeight);
	//			Crouch();
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Display, TEXT("prona cojone"));
	//			Prone();
	//		}
	//	}
	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnSlide"));
	//	bIsSliding = false;
	//}
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

void AAdvMovSysCharacter::HandleInput(const FInputActionValue& Value)
{
	if (CurrentState)
		CurrentState->HandleInput(this, Value);
}

void AAdvMovSysCharacter::SetCharacterState(CharacterState* NewState)
{
	if (CurrentState)
		CurrentState->ExitState(this);

	CurrentState = NewState;
	
	if (CurrentState)
		CurrentState->EnterState(this);

	HandleInput(FInputActionValue());
}
