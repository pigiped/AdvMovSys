// Fill out your copyright notice in the Description page of Project Settings.


#include "ProningState.h"
#include "InputActionValue.h"
#include "AdvMovSysCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h" 
#include "CrouchingState.h"
#include "DefaultState.h"
#include "DrawDebugHelpers.h"


ProningState::ProningState()
{
}

ProningState::~ProningState()
{
}

ProningState& ProningState::Get()
{
	static ProningState Instance;
	return Instance;
}

void ProningState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Proning (state handler)"));
	}
	// Do not dereference Character here; input handling may be informative only.
}

void ProningState::EnterState(AAdvMovSysCharacter* Character)
{
	if (!Character) return;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Enter Prone"));
	}
	Prone(Character);
}

void ProningState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Exit Prone"));
	}
}

void ProningState::Prone(AAdvMovSysCharacter* Character)
{
	if (!Character) return;
	
	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	if (!Capsule) return;
	
	// Get the current capsule half height before changing it
	float CurrentHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	
	// Calculate the height difference
	float HeightDifference = CurrentHalfHeight - PronedHalfHeight;
	
	// Adjust the actor's Z position to keep the bottom of the capsule at the same location
	FVector NewLocation = Character->GetActorLocation();
	NewLocation.Z -= HeightDifference; // Move down by the difference
	Character->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
	
	// Now set the capsule height
	Character->RecalculateCapsuleHalfHeight(PronedHalfHeight);
	Character->SetWalkSpeed(PronedWalkSpeed);
	Character->RecalculateBaseEyeHeight();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, TEXT("Prone"));
	}
}

//void ProningState::UnProne(AAdvMovSysCharacter* Character)
//{
//	if (!Character) return;
//
//	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
//	if (!Capsule) return;
//
//	UWorld* World = Character->GetWorld();
//	if (!World) return;
//
//	FVector Start = Character->GetActorLocation() - FVector(0, 0, Capsule->GetScaledCapsuleRadius());
//	FVector End = Start + FVector(0.f, 0.f, CrouchingState::Get().GetCrouchedHalfHeight());
//
//	// Debug Line
//	bool bBlocked = World->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
//	DrawDebugLine(World, Start, End, FColor::Red, false, 5.0f, 0, 5.0f);
//
//	UE_LOG(LogTemp, Display, TEXT("bBlocked %s"), bBlocked ? TEXT("true") : TEXT("false"));
//
//	if (!bBlocked)
//	{
//		// Can crouch
//		UE_LOG(LogTemp, Display, TEXT("Can crouch from prone"));
//		return &CrouchingState::Get();
//	}
//
//	// Can't crouch either, stay prone
//	UE_LOG(LogTemp, Display, TEXT("Must stay prone - no space"));
//	return &ProningState::Get();
//}

