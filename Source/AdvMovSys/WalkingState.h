// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * Walking state for the character
 */
class ADVMOVSYS_API WalkingState : public CharacterState
{
public:
	WalkingState();
	~WalkingState();
	static WalkingState& Get();

	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual ECharacterMovementState GetStateType() const override { return ECharacterMovementState::Walking; }

private:
	float WalkWalkSpeed = 200.0f;
};
