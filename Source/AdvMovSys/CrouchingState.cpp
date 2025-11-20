// Fill out your copyright notice in the Description page of Project Settings.

#include "CrouchingState.h"
#include "InputActionValue.h"
#include "Engine/Engine.h"
#include "AdvMovSysCharacter.h"

CrouchingState::CrouchingState()
{
}

CrouchingState::~CrouchingState()
{
}

CrouchingState& CrouchingState::Get()
{
	static CrouchingState Instance;
	return Instance;
}

void CrouchingState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Crouch (state handler)"));
	}
	// Do not dereference Character here; input handling may be informative only.
}

void CrouchingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Enter Crouch"));
	}

	if (Character)
		if (!Character) return;

	Character->RecalculateCapsuleHalfHeight(CrouchedHalfHeight);
	Character->SetWalkSpeed(CrouchedWalkSpeed);
	Character->RecalculateBaseEyeHeight();
}

void CrouchingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Exit Crouch"));
	}
}

float CrouchingState::GetCrouchedHalfHeight() const
{
	return CrouchedHalfHeight;
}
