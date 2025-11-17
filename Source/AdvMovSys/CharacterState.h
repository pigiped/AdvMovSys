// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * Base class for character input states.
 */
class ADVMOVSYS_API CharacterState
{
public:
	CharacterState();
	virtual ~CharacterState();

	// Default no-op. Override in derived states.
	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value);
	virtual void EnterState(AAdvMovSysCharacter* Character) {};
	virtual void ExitState(AAdvMovSysCharacter* Character) {};
};
