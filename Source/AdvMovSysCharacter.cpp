// In the SetCharacterState methods, add after CurrentState = NewState;

void AAdvMovSysCharacter::SetCharacterState(CharacterState* NewState)
{
	if (CurrentState)
		CurrentState->ExitState(this);

	CurrentState = NewState;
	
	// Cache the state type for Blueprint access
	if (CurrentState)
	{
		CurrentMovementState = CurrentState->GetStateType();
		CurrentState->EnterState(this);
	}
}

void AAdvMovSysCharacter::SetCharacterState(CharacterState* NewState, const FInputActionValue& Value)
{
	if (CurrentState)
		CurrentState->ExitState(this);

	CurrentState = NewState;

	// Cache the state type for Blueprint access
	if (CurrentState)
	{
		CurrentMovementState = CurrentState->GetStateType();
		CurrentState->EnterState(this);
		CurrentState->HandleInput(this, Value);
	}
}