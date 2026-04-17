// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/PawnIconWidget.h"
#include "VehicleIconWidget.generated.h"

class ABrickVehicle;
class ABrickCharacter;
enum class EVehiclePinMode : uint8;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UVehicleIconWidget : public UPawnIconWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached pointer to the owning vehicle
	UPROPERTY(Transient)
	ABrickVehicle* Vehicle;
	// The current vehicle pin mode
	EVehiclePinMode VehiclePinMode;
	// Whether this is the icon for the local player
	uint32 bIsLocalPlayer : 1;
	// ~Variables

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual void UninitializeIcon_Implementation() override;
	virtual FText GetIconDisplayName_Implementation() const override;
	virtual void OnCanBeDamagedChanged(bool bNewCanBeDamaged) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleIcon(bool bNewIsLocalPlayer, bool bNewCanBeDamaged, EVehiclePinMode NewPinMode);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with all required parameters
	void UpdateVehicleIcon();
	// Callbacks for the vehicle
	void OnUGCDownloadProgressChanged(bool bIsDownloading, float Progress);
	void OnPassengerEnteredOrLeftVehicle(ABrickCharacter* InCharacter, bool bEntered);
	void OnPinModeChanged(EVehiclePinMode NewPinMode);
};
