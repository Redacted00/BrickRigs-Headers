// Copyright Fluppisoft, 2017

#pragma once

#include "BaseCharacterMovementComponent.h"
#include "BrickCharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickCharacterMovementComponent : public UBaseCharacterMovementComponent
{
	GENERATED_BODY()

public:
	// ~Constructor
	UBrickCharacterMovementComponent();

	// ~Super Interface
	virtual void Crouch(bool bClientSimulation) override;
	virtual void UnCrouch(bool bClientSimulation) override;
	// ~Super Interface
};
