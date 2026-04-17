// Copyright Fluppisoft, 2021

#pragma once

#include "Projectiles/AmmoType.h"
#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "TargetMarkerIconWidget.generated.h"

class UGunBrick;

/**
 * Used by target markers to display the predicted impact points
 */
UCLASS(Abstract)
class BRICKRIGS_API UTargetMarkerIconWidget : public UHUDIconWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The currently displayed gun
	TWeakObjectPtr<UGunBrick> GunBrick;
	// ~Variables

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual void UninitializeIcon_Implementation() override;
	virtual void UpdateDrawDistance(float NewDist, float& InOutIconScale) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFireAction(int32 NewActionIndex);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmoType(EAmmoType NewType);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTargetDistance(float NewDist);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHasValidTarget(bool bNewHasValidTarget);
	// ~Blueprint Interface

private:
	// Updates the current gun
	void SetGun(UGunBrick* NewGun);
};
