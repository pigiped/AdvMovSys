// Copyright 2026 Pierluigi Pedicone

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

/**
 * 
 */
class ADVMOVSYS_API DefaultState : public CharacterState
{
public:
	DefaultState();
	virtual ~DefaultState();

	// Singleton accessor to avoid header/circular static definitions
	static DefaultState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
	virtual ECharacterMovementState GetStateType() const override { return ECharacterMovementState::Default; }

	float GetDefaultWalkSpeed() const { return DefaultWalkSpeed; }
	float GetDefaultHalfHeight() const { return DefaultHalfHeight; }

private:
	float DefaultWalkSpeed = 500.0f;
	float DefaultHalfHeight = 90.0f;
};
