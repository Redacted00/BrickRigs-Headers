// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BrickAIController.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API ABrickAIController : public AAIController
{
	GENERATED_BODY()

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = AI)
	float ControlRotationInterpSpeed;
	// ~Properties

public:
	// ~Constructor
	ABrickAIController();

	// ~Super Interface
	virtual FVector GetFocalPointOnActor(const AActor* Actor) const override;
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;
	// ~Super Interface
};
