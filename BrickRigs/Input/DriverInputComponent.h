// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Input/VehicleInputComponent.h"
#include "DriverInputComponent.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UDriverInputComponent : public UVehicleInputComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	FHoldKeyTimer HoldKeyTimer_CycleSiren;
	// ~Variables

public:
	// ~Constructor
	UDriverInputComponent();

	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface

private:
	// Misc
	UFUNCTION()
	void OnPressedCycleTransmissionMode();
	UFUNCTION()
	FText GetCycleTransmissionModeValueText() const;
	UFUNCTION()
	void OnPressedToggleAutoCounterSteering();
	UFUNCTION()
	FText GetToggleAutoCounterSteeringValueText() const;
};
