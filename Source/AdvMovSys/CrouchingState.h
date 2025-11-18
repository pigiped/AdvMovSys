// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * Crouching state implementation.
 */
class ADVMOVSYS_API CrouchingState : public CharacterState
{
public:
	CrouchingState();
	virtual ~CrouchingState();

	// Singleton accessor to avoid header/circular static definitions
	static CrouchingState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
	float GetCrouchedHalfHeight() const;

private:
	float CrouchedHalfHeight = 45.0f;

};
