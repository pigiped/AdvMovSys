// Copyright 2026 Pierluigi Pedicone


#include "EdgeGrabState.h"
#include "AdvMovSysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "InputMappingContext.h"
#include <EnhancedInputSubsystems.h>

EdgeGrabState::EdgeGrabState()
{
	LoadInputMappingContexts();
}

EdgeGrabState::~EdgeGrabState()
{

	ClearInputMappingContexts();
}

EdgeGrabState& EdgeGrabState::Get()
{
	static EdgeGrabState Instance;
	return Instance;
}

void EdgeGrabState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EdgeGrabState: HandleInput"));
	}
}
void EdgeGrabState::EnterState(AAdvMovSysCharacter* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EdgeGrabState: EnterState"));
	}

	// 1
	// Lock sprint, crouch, prone, slide. 
	// Maybe change action mapping to one that moves left and right in edgegrab and up makes stand on the ledge down drops down
	//DO SMTH FOR JOYSTICK
	SetMappingContext(Character, IMC_Default, IMC_EdgeGrab);

	// 2
	// Freeze height position, disable gravity and go to fly mode?
	// Ensure the character is facing the correct direction during edge grab
	Character->bSimGravityDisabled = true;
	Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
	Character->GetCharacterMovement()->Velocity = FVector::ZeroVector;
	Character->GetCharacterMovement()->StopMovementImmediately();
	

	// 3
	// set animation
}
void EdgeGrabState::ExitState(AAdvMovSysCharacter* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EdgeGrabState: ExitState"));
	}

	SetMappingContext(Character, IMC_EdgeGrab, IMC_Default);
}

void EdgeGrabState::SetMappingContext(AAdvMovSysCharacter* Character, UInputMappingContext* OldContext, UInputMappingContext* NewContext)
{
	if (!Character) return;

	// Get the player controller
	APlayerController* PC = Cast<APlayerController>(Character->GetController());
	if (!PC) return;

	// Get the Enhanced Input Subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(OldContext);

		// Add NewContext with higher priority
		if (NewContext)
		{
			Subsystem->AddMappingContext(NewContext, 1);
			Subsystem->RequestRebuildControlMappings();
			UE_LOG(LogTemp, Log, TEXT("NewContext controls activated"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NewContext is null! Check asset path."));
		}
	}
	

	UE_LOG(LogTemp, Log, TEXT("Old Context: %s, New Context: %s"), *OldContext->GetName(), NewContext ? *NewContext->GetName() : TEXT("None"));
}

void EdgeGrabState::LoadInputMappingContexts()
{
	// Only load once
	if (IMC_Default && IMC_EdgeGrab) return;

	// Load at runtime using StaticLoadObject (safe anytime)
	IMC_Default = Cast<UInputMappingContext>(
		StaticLoadObject(UInputMappingContext::StaticClass(), nullptr,
			TEXT("/Game/Input/IMC_Default"))
	);
	IMC_EdgeGrab = Cast<UInputMappingContext>(
		StaticLoadObject(UInputMappingContext::StaticClass(), nullptr,
			TEXT("/Game/Input/IMC_EdgeGrab"))
	);
}

void EdgeGrabState::ClearInputMappingContexts()
{
	IMC_Default = nullptr;
	IMC_EdgeGrab = nullptr;
}

