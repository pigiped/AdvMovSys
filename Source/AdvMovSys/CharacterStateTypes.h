// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStateTypes.generated.h"

/**
 * Enum representing the different character movement states
 * This can be accessed from Animation Blueprints
 */
UENUM(BlueprintType)
enum class ECharacterMovementState : uint8
{
	Default     UMETA(DisplayName = "Default"),
	Walking     UMETA(DisplayName = "Walking"),
	Sprinting   UMETA(DisplayName = "Sprinting"),
	Crouching   UMETA(DisplayName = "Crouching"),
	Proning     UMETA(DisplayName = "Proning"),
	Sliding     UMETA(DisplayName = "Sliding")
};