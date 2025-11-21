// Copyright 2026 Pierluigi Pedicone


#include "DefaultState.h"
#include "AdvMovSysCharacter.h"
#include "Engine/Engine.h"

DefaultState::DefaultState()
{
}

DefaultState::~DefaultState()
{
}

DefaultState& DefaultState::Get()
{
	static DefaultState Instance;
	return Instance;
}

void DefaultState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Default (state handler)"));
	}
	// Handle input specific to the default state here
}

void DefaultState::EnterState(AAdvMovSysCharacter* Character)
{
	if (!Character) return;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Enter Default"));
	}

	// Set character properties for default state
	Character->SetWalkSpeed(DefaultWalkSpeed);
	Character->RecalculateCapsuleHalfHeight(DefaultHalfHeight);
}

void DefaultState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Exit Default"));
	}
	// Cleanup or reset properties when exiting default state if necessary
}
