// Copyright Fluppisoft, 2020

#pragma once

#include "Vehicle/VehicleInputAxis.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardIconWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UDashboardIconWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The action this icon represents
	EVehicleInputAxis InputAction;
	// The index of the icon used
	int32 IconIndex;
	// The optional icon index override
	int32 IconIndexOverride;
	// The cached color style
	EBrickUIColorStyle ColorStyle;
	// Whether the icon is currently visible
	bool bIsIconVisible;
	// ~Variables

public:
	// Initializes the icon appearance
	void InitializeIcon(EVehicleInputAxis InInputAction, EBrickUIColorStyle InColorStyle, int32 InIconIndex, bool bInIsVisible, int32 InIconIndexOverride);
	// Updates the current icon state
	void UpdateIcon(bool bInIsVisible, int32 InIconIndexOverride);
	// Get the input action associated with the icon
	EVehicleInputAxis GetVehicleInputAction() const
	{
		return InputAction;
	}

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIconImage(bool bInVisible, EBrickUIColorStyle InColorStyle, int32 InIconIndex);
	// ~Blueprint Interface

private:
	// Calls the blueprint function with all required parameters
	void UpdateIconImage();
};
