// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterStateTypes.h"
#include "AdvMovSysCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class CharacterState;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AAdvMovSysCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

protected:

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* WalkAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ProneAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* SlideAction;

	/** Current state (state pattern) */
	CharacterState* CurrentState = nullptr;
	CharacterState* PreviousState = nullptr;

	/** Cached current state type for Blueprint access */
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	ECharacterMovementState CurrentMovementState;
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	ECharacterMovementState PreviousMovementState;


public:

	/** Constructor */
	AAdvMovSysCharacter();

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
	void Walk(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void DoCrouch(const FInputActionValue& Value);
	void DoProne(const FInputActionValue& Value);
	void DoSlide(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

public:

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	/** Gets the current movement state type (accessible from Animation Blueprint) */
	UFUNCTION(BlueprintPure, Category = "Character Movement")
	ECharacterMovementState GetCurrentMovementState() const { return CurrentMovementState; }
	UFUNCTION(BlueprintPure, Category = "Character Movement")
	ECharacterMovementState GetPreviousMovementState() const { return PreviousMovementState; }

	// Internal C++ methods - not exposed to Blueprints
	CharacterState* GetCurrentState() const { return CurrentState; }
	CharacterState* GetPreviousState() const { return PreviousState; }

	void SetCharacterState(CharacterState* NewState);
	void SetCharacterState(CharacterState* NewState, const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	void SetMovementState(ECharacterMovementState NewState);

	CharacterState* GetTargetState() const;

	UFUNCTION(BlueprintCallable, Category = "Character Movement")
	ECharacterMovementState GetTargetMovementState() const;

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void SetWalkSpeed(float NewWalkSpeed);
	void RecalculateCapsuleHalfHeight(float NewHalfHeight);

};

