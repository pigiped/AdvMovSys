// Copyright 2026 Pierluigi Pedicone


#include "EdgeGrabState.h"
#include "AdvMovSysCharacter.h"
#include "Engine/Engine.h"

EdgeGrabState::EdgeGrabState()
{
}

EdgeGrabState::~EdgeGrabState()
{
}

EdgeGrabState& EdgeGrabState::Get()
{
	static EdgeGrabState Instance;
	return Instance;
}

void EdgeGrabState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("EdgeGrabState: HandleInput"));
	}
}
void EdgeGrabState::EnterState(AAdvMovSysCharacter* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EdgeGrabState: EnterState"));
	}
}
void EdgeGrabState::ExitState(AAdvMovSysCharacter* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EdgeGrabState: ExitState"));
	}
}

