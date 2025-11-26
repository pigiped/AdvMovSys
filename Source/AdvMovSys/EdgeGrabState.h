// Copyright 2026 Pierluigi Pedicone

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

/**
 * 
 */
class ADVMOVSYS_API EdgeGrabState : public CharacterState
{
public:
	EdgeGrabState();
	virtual ~EdgeGrabState();

	// Singleton accessor to avoid header/circular static definitions
	static EdgeGrabState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;
	virtual ECharacterMovementState GetStateType() const override { return ECharacterMovementState::EdgeGrab; }
};
