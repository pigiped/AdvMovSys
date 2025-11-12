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
#include "AdvMovSys.h"

AAdvMovSysCharacter::AAdvMovSysCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

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
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
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
	bool ShouldWalk = Value.Get<bool>();

	if (ShouldWalk)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void AAdvMovSysCharacter::Sprint(const FInputActionValue& Value)
{
	bool ShouldSprint = Value.Get<bool>();

	if (ShouldSprint)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->MaxWalkSpeed = 800.0f;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	}
}

void AAdvMovSysCharacter::DoCrouch(const FInputActionValue& Value)
{
	bool ShouldCrouchInput = Value.Get<bool>();

	if (ShouldCrouchInput)
	{
		if (GetCharacterMovement()->IsMovingOnGround())
		{
			if (!bIsCrouched)
			{
				Crouch();
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Crouch"));
			}
			else
			{
				if (bIsProne)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Unproning for crouching"));
					UnProne();
					return;
				}
				UnCrouch();
				GetCharacterMovement()->MaxWalkSpeed = 600.0f;
				GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnCrouch"));
			}
		}
	}
}

void AAdvMovSysCharacter::DoProne(const FInputActionValue& Value)
{
	bool ShouldProneInput = Value.Get<bool>();

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
	}
}

void AAdvMovSysCharacter::Prone()
{
	Crouch();
	bIsProne = true;
	GetCapsuleComponent()->SetCapsuleHalfHeight(34.0f);
	GetCharacterMovement()->MaxWalkSpeed = 100.0f;

	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Prone"));
}

void AAdvMovSysCharacter::UnProne()
{
	// Esci da prone - verifica spazio sopra
	/*FVector Start = GetActorLocation();
	FVector End = Start;
	float CapsuleRadius = GetCapsuleComponent()->GetScaledCapsuleRadius();
	float TargetHalfHeight = 96.0f;

	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ProneTrace), false, this);

	const bool bBlocked = GetWorld()->SweepTestByChannel(
		Start, End, FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeCapsule(CapsuleRadius, TargetHalfHeight),
		QueryParams
	);

	if (!bBlocked)
	{
		bIsProne = false;
		GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
		GetCharacterMovement()->MaxWalkSpeed = 500.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnProne"));
	}*/
	bIsProne = false;
	GetCapsuleComponent()->SetCapsuleHalfHeight(40.0f);
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnProne"));
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
