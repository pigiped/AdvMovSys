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

	// 1
	// Lock sprint, crouch, prone, slide. 
	// Maybe change action mapping to one that moves left and right in edgegrab and up makes stand on the ledge down drops down
	
	// 2
	// Freeze height position, disable gravity and go to fly mode?
	// Ensure the character is facing the correct direction during edge grab

	// 3
	// set animation
}
void EdgeGrabState::ExitState(AAdvMovSysCharacter* Character)
{
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("EdgeGrabState: ExitState"));
	}
}

