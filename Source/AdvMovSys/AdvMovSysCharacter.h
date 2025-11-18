// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "CharacterState.h"
#include "AdvMovSysCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
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
	void Slide(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

private:
	float StandingHalfHeight = 90.0f;
	float NormalWalkSpeed = 500.0f;
	float WalkWalkSpeed = 200.0f;
	float SprintWalkSpeed = 800.0f;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Character Movement")
	bool bIsProne = false;

	UPROPERTY(BlueprintReadWrite, Category = "Character Movement")
	bool bIsSliding = false;

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

	virtual void HandleInput(const FInputActionValue& Value);
	void SetCharacterState(CharacterState* NewState);

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	/** Getters for capsule half-heights and movement speeds (Blueprint-accessible, read-only) */
	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FORCEINLINE float GetStandingHalfHeight() const { return StandingHalfHeight; }

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FORCEINLINE float GetNormalWalkSpeed() const { return NormalWalkSpeed; }

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FORCEINLINE float GetWalkWalkSpeed() const { return WalkWalkSpeed; }

	UFUNCTION(BlueprintPure, Category = "Character Movement")
	FORCEINLINE float GetSprintWalkSpeed() const { return SprintWalkSpeed; }

	void SetWalkSpeed(float NewWalkSpeed);
	void RecalculateCapsuleHalfHeight(float NewHalfHeight);

};

