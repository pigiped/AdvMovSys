// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

/**
 * 
 */
class ADVMOVSYS_API SlidingState : public CharacterState
{
public:
	SlidingState();
	virtual ~SlidingState();

	// Singleton accessor to avoid header/circular static definitions
	static SlidingState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
	virtual ECharacterMovementState GetStateType() const override { return ECharacterMovementState::Sliding; }

private:
	void Slide(AAdvMovSysCharacter* Character);
	void UnSlide(AAdvMovSysCharacter* Character);
};
