// Fill out your copyright notice in the Description page of Project Settings.


#include "WalkingState.h"
#include "AdvMovSysCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Enter Walk"));
	}
	if (Character)
	{
		Character->SetWalkSpeed(WalkWalkSpeed);
	}
}

void WalkingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Exit Walk"));
	}
	if (Character)
	{
		//Character->SetWalkSpeed(Character->GetNormalWalkSpeed());
	}
}

void WalkingState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Walking (state handler)"));
	}
	// Handle input for walking state
}
