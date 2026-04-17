// Copyright Fluppisoft, 2020

#pragma once

#include "Vehicle/VehicleInputAxis.h"
#include "UI/BrickUIStyle.h"
#include "UI/Misc/NumericValueType.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardWidget.generated.h"

class ABrickVehicle;
class USeatBrick;
class UDashboardSliderWidget;
class UDashboardIconWidget;
enum class EVehiclePinMode : uint8;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UDashboardWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The associated vehicle
	UPROPERTY(Transient)
	ABrickVehicle* Vehicle;
	// The associated seat
	TWeakObjectPtr<USeatBrick> VehicleSeat;
	// These flags indicate for every slider whether it is visible and should be updated
	uint8 bRPMVisible : 1;
	uint8 bGearVisible : 1;
	uint8 bFuelVisible : 1;
	// The cached ammo capacity
	int32 AmmoCapacity;
	// All icon widgets that have been created
	UPROPERTY(Transient)
	TArray<UDashboardIconWidget*> IconWidgets;
	// All slider widgets that have been created
	UPROPERTY(Transient)
	TArray<UDashboardSliderWidget*> SliderWidgets;
	// The sea level to measure the altitude against
	float SeaLevel;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Dashboard)
	TSubclassOf<UDashboardIconWidget> IconWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Dashboard)
	TSubclassOf<UDashboardSliderWidget> SliderWidgetClass;
	// Determines how low fuel, ammo etc have to be in order to be displayed red
	UPROPERTY(EditDefaultsOnly, Category = Dashboard)
	float LowResourceWarningThreshold;
	// ~Properties

public:
	// ~Constructor
	UDashboardWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
	// ~Super Interface

	// Sets the associated vehicle and seat the player is in
	void SetVehicleAndSeat(ABrickVehicle* InVehicle, USeatBrick* InSeat);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddIconWidget(UDashboardIconWidget* Widget, int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void AddSliderWidget(UDashboardSliderWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSliderWidgetSlot(UDashboardSliderWidget* Widget, int32 Index, int32 NumSliders);
	// ~Blueprint Interface

private:
	// Initializes the icon widgets
	void InitializeIcons();
	// Creates a new icon widget
	void AddIcon(EVehicleInputAxis InputAction, EBrickUIColorStyle ColorStyle, int32 IconIndex);
	// Updates the state of an icon for a specific action
	void UpdateIcon(EVehicleInputAxis InputAction);
	// Updates the state of an icon widget
	void UpdateIcon(UDashboardIconWidget* Widget);
	// Returns whether the icon should be visible and an optional icon index override
	TTuple<bool, int32> GetIconState(EVehicleInputAxis InputAction) const;

	// Initializes the sliders for the current vehicle
	void InitializeSliders();

	// Callback for the vehicle
	void OnVehicleConstructed();
	void OnInputActionChanged(EVehicleInputAxis Action, bool bNewValue);
	void OnFireActionModeChanged(int32 NewMode);
	void OnCanBeDamagedChanged(bool bNewCanBeDamaged);
	void OnPinModeChanged(EVehiclePinMode NewPinMode);
};
