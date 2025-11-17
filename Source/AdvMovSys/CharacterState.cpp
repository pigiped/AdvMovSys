// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterState.h"
#include "InputActionValue.h"

CharacterState::CharacterState()
{
}

CharacterState::~CharacterState()
{
}

void CharacterState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	// Base implementation intentionally empty. Derived states override this.
}
