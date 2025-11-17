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
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Crouch (state handler)"));
	}
	// Do not dereference Character here; input handling may be informative only.
}

void CrouchingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Enter Crouch"));
	}
	if (Character)
	{
		Character->Crouch();
	}
}

void CrouchingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Exit Crouch"));
	}
	if (Character)
	{
		Character->UnCrouch();
	}
}
