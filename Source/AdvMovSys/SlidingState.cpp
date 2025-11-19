// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingState.h"
#include "InputActionValue.h"
#include "Components/CapsuleComponent.h"
#include "ProningState.h"
#include "DefaultState.h"
#include "CrouchingState.h"
#include "AdvMovSysCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"


SlidingState::SlidingState()
{
}

SlidingState::~SlidingState()
{
}

SlidingState& SlidingState::Get()
{
	static SlidingState Instance;
	return Instance;
}

void SlidingState::HandleInput(AAdvMovSysCharacter* Character, const FInputActionValue& Value)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("Sliding (state handler)"));
	}
	if (Value.Get<bool>())
	{
		Slide(Character);
	}
	else
	{
		UnSlide(Character);
	}
}

void SlidingState::EnterState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Enter Slide"));
	}
}

void SlidingState::ExitState(AAdvMovSysCharacter* Character)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Exit Slide"));
	}
}

void SlidingState::Slide(AAdvMovSysCharacter* Character)
{
	if (!Character) return;
	
	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	if (!Capsule) return;

	float CurrentHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float HeightDifference = CurrentHalfHeight - ProningState::Get().GetProningHalfHeight();

	// Compensa la posizione verticale dell'attore per mantenerlo a terra
	FVector NewLocation = Character->GetActorLocation();
	NewLocation.Z -= HeightDifference;
	Character->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// Aggiusta anche la mesh
	Character->RecalculateCapsuleHalfHeight(ProningState::Get().GetProningHalfHeight());

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green, TEXT("Slide"));
	}
}

void SlidingState::UnSlide(AAdvMovSysCharacter* Character)
{
	if (!Character) return;
	
	UCapsuleComponent* Capsule = Character->GetCapsuleComponent();
	if (!Capsule) return;

	UWorld* World = Character->GetWorld();
	if (!World) return;

	float CurrentHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	float HeightDifference = DefaultState::Get().GetDefaultHalfHeight() - CurrentHalfHeight;

	FVector Start = Character->GetActorLocation() - FVector(0, 0, Capsule->GetScaledCapsuleRadius());
	FVector End = Start + FVector(0.f, 0.f, DefaultState::Get().GetDefaultHalfHeight() * 2);

	// Debug Line
	bool bBlocked = World->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
	DrawDebugLine(World, Start, End, FColor::Red, false, 5.0f, 0, 5.0f);

	if (!bBlocked)
	{
		// Compensa la posizione
		FVector NewLocation = Character->GetActorLocation();
		NewLocation.Z += HeightDifference;
		Character->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

		// Aggiusta la mesh
		Character->RecalculateCapsuleHalfHeight(DefaultState::Get().GetDefaultHalfHeight());
	}
	else
	{
		End = Start + FVector(0.f, 0.f, CrouchingState::Get().GetCrouchedHalfHeight() * 2);
		bBlocked = World->LineTraceTestByChannel(Start, End, ECollisionChannel::ECC_Visibility);
		DrawDebugLine(World, Start, End, FColor::Red, false, 5.0f, 0, 5.0f);
		if (!bBlocked)
		{
			// Compensa la posizione
			//FVector NewLocation = GetActorLocation();
			//NewLocation.Z += (CrouchedHalfHeight - CurrentHalfHeight);
			//SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
			//// Aggiusta la mesh
			//RecalculateCapsuleHalfHeight(CrouchedHalfHeight);
			Character->SetCharacterState(&CrouchingState::Get());
		}
		else
		{
			Character->SetCharacterState(&ProningState::Get());
		}
	}
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Yellow, TEXT("UnSlide"));
	}
}
