// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "PawnInputComponent.generated.h"

/**
 * The base class for all pawn input components, be it spectators or game pawns
 */
UCLASS(Abstract)
class BRICKRIGS_API UPawnInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

	// ~Variables
	FHoldKeyTimer HoldKeyTimer_Recover;
	FHoldKeyTimer HoldKeyTimer_DestroyPawn;
	FHoldKeyTimer HoldKeyTimer_ToggleInvincible;
	FHoldKeyTimer HoldKeyTimer_CycleVehicles;
	FHoldKeyTimer HoldKeyTimer_CycleCharacters;
	FHoldKeyTimer HoldKeyTimer_RecoverDummies;
	FHoldKeyTimer HoldKeyTimer_FreeCam;
	FHoldKeyTimer HoldKeyTimer_SlowMotion;
	FHoldKeyTimer HoldKeyTimer_SlomoSpeedUp;
	FHoldKeyTimer HoldKeyTimer_SlomoSpeedDown;
	FHoldKeyTimer HoldKeyTimer_ScrapAllVehicles;
	FHoldKeyTimer HoldKeyTimer_OpenVehicle;
	// ~Variables

public:
	// ~Constructor
	UPawnInputComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetPlayerController(ABasePlayerController* PC) override;
	// ~Super Interface

private:
	// Callback for the player controller
	void OnViewTargetSubobjectChanged(UObject* NewViewTargetSubobject);

protected:
	// Sandbox
	UFUNCTION()
	void OnPressedRecover();
	UFUNCTION()
	void OnReleasedRecover();
	UFUNCTION()
	void OnTappedRecover();
	UFUNCTION()
	void OnHeldRecover();
	UFUNCTION()
	void OnPressedDestroyPawn();
	UFUNCTION()
	void OnReleasedDestroyPawn();
	UFUNCTION()
	void OnTappedDestroyPawn();
	UFUNCTION()
	void OnHeldDestroyPawn();
	UFUNCTION()
	bool GetDestroyPawnEnabled(bool bInSecondaryAction) const;
	UFUNCTION()
	void OnPressedToggleInvincible();
	UFUNCTION()
	void OnReleasedToggleInvincible();
	UFUNCTION()
	void OnTappedToggleInvincible();
	UFUNCTION()
	void OnHeldToggleInvincible();
	UFUNCTION()
	void OnPressedCycleVehicles();
	UFUNCTION()
	void OnReleasedCycleVehicles();
	UFUNCTION()
	void OnTappedCycleVehicles();
	UFUNCTION()
	void OnHeldCycleVehicles();
	UFUNCTION()
	void OnPressedCycleCharacters();
	UFUNCTION()
	void OnReleasedCycleCharacters();
	UFUNCTION()
	void OnTappedCycleCharacters();
	UFUNCTION()
	void OnHeldCycleCharacters();
	UFUNCTION()
	void OnPressedRecoverDummies();
	UFUNCTION()
	void OnReleasedRecoverDummies();
	UFUNCTION()
	void OnTappedRecoverDummies();
	UFUNCTION()
	void OnHeldRecoverDummies();
	UFUNCTION()
	void OnPressedProjectileCamera();

	// Inventory
	UFUNCTION()
	void OnPressedInventory();
	UFUNCTION()
	bool GetInventoryEnabled(bool bInSecondaryAction) const;
	// Camera
	UFUNCTION()
	void OnPressedFreeCamera();
	UFUNCTION()
	void OnReleasedFreeCamera();
	UFUNCTION()
	void OnHeldFreeCamera();
	UFUNCTION()
	void OnTappedFreeCamera();

	// Slow motion
	UFUNCTION()
	void OnPressedSlowMotion();
	UFUNCTION()
	void OnReleasedSlowMotion();
	UFUNCTION()
	void OnTappedSlowMotion();
	UFUNCTION()
	void OnHeldSlowMotion();
	UFUNCTION()
	void OnPressedSlomoSpeedUp();
	UFUNCTION()
	void OnReleasedSlomoSpeedUp();
	UFUNCTION()
	void OnTappedSlomoSpeedUp();
	UFUNCTION()
	void OnHeldSlomoSpeedUp();
	UFUNCTION()
	void OnPressedSlomoSpeedDown();
	UFUNCTION()
	void OnReleasedSlomoSpeedDown();
	UFUNCTION()
	void OnTappedSlomoSpeedDown();
	UFUNCTION()
	void OnHeldSlomoSpeedDown();

	// Vehicles
	UFUNCTION()
	void OnPressedScrapAllVehicles();
	UFUNCTION()
	void OnReleasedScrapAllVehicles();
	UFUNCTION()
	void OnTappedScrapAllVehicles();
	UFUNCTION()
	void OnHeldScrapAllVehicles();
	UFUNCTION()
	void OnPressedOpenVehicle();
	UFUNCTION()
	void OnReleasedOpenVehicle();
	UFUNCTION()
	void OnTappedOpenVehicle();
	UFUNCTION()
	void OnHeldOpenVehicle();

	// UI
	UFUNCTION()
	void OnPressedMap();
};
