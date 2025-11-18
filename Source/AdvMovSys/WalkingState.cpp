// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"
#include "AdvMovSysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

WalkingState::WalkingState()
{
}

WalkingState::~WalkingState()
{
}

WalkingState& WalkingState::Get()
{
	static WalkingState Instance;
	return Instance;
}

void WalkingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetWalkWalkSpeed();
	}
}

void WalkingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (Character)
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = Character->GetNormalWalkSpeed();
	}
}

void WalkingState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	// Handle input for walking state
}
