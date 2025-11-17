// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintingState.h"
#include "AdvMovSysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

SprintingState::SprintingState()
{
}

SprintingState::~SprintingState()
{
}

SprintingState& SprintingState::Get()
{
	static SprintingState Instance;
	return Instance;
}

void SprintingState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	// Do not dereference Character here; input handling may be informative only.
}

void SprintingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetSprintWalkSpeed();
	}
}

void SprintingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetNormalWalkSpeed();
	}
}
