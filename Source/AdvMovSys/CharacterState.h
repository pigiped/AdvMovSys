// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateTypes.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * Base class for character input states. Derive from this class to implement specific states.
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

	// Override this in each derived state to return its type
	virtual ECharacterMovementState GetStateType() const { return ECharacterMovementState::Default; }
};
