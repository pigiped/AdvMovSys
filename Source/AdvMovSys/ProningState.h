// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterState.h"

struct FInputActionValue;
class AAdvMovSysCharacter;

/**
 * 
 */
class ADVMOVSYS_API ProningState : public CharacterState
{
public:
	ProningState();
	virtual ~ProningState();

	// Singleton accessor to avoid header/circular static definitions
	static ProningState& Get();

	virtual void HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value) override;
	virtual void EnterState(AAdvMovSysCharacter* Character) override;
	virtual void ExitState(AAdvMovSysCharacter* Character) override;

private:
	void Prone(AAdvMovSysCharacter* Character);
	void UnProne(AAdvMovSysCharacter* Character);
	float PronedHalfHeight = 20.0f;
	float PronedWalkSpeed = 100.0f;

};
