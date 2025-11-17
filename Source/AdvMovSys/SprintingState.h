// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * Sprinting state implementation.
 */
class ADVMOVSYS_API SprintingState : public CharacterState
{
public:
	SprintingState();
	virtual ~SprintingState();

	// Singleton accessor to avoid header/circular static definitions
	static SprintingState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
};
