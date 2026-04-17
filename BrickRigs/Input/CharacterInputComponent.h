// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "PlayerPawnInputComponent.h"
#include "CharacterInputComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UCharacterInputComponent : public UPlayerPawnInputComponent
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface

private:
	// Movement input
	UFUNCTION()
	void WalkForward(float Val);
	UFUNCTION()
	void WalkRight(float Val);
	// Helper function to add movement input
	void AddMovementInput(const FVector& Direction, float Val);

	UFUNCTION()
	void OnPressedSprint();
	UFUNCTION()
	void OnReleasedSprint();
	// Return whether sprint should be controlled with toggle rather than hold
	bool IsSprintToggle() const;

	UFUNCTION()
	void OnPressedCrouch();
	UFUNCTION()
	void OnReleasedCrouch();
	UFUNCTION()
	void OnToggleCrouch();
	// Whether crouching should be toggled
	bool IsCrouchToggle() const;

	UFUNCTION()
	void OnPressedJump();
	UFUNCTION()
	void OnReleasedJump();
};
