// Fill out your copyright notice in the Description page of Project Settings.


#include "SprintingState.h"
#include "AdvMovSysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DefaultState.h"
#include "Engine/Engine.h"

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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Sprinting (state handler)"));
	}
	// Do not dereference Character here; input handling may be informative only.
}

void SprintingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Enter Sprint"));
	}
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = SprintWalkSpeed;
	}
}

void SprintingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Exit Sprint"));
	}
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = DefaultState::Get().GetDefaultWalkSpeed();
	}
}
