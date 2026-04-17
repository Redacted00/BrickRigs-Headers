// Copyright Fluppisoft, 2020

#pragma once

#include "Projectiles/AmmoType.h"
#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "AmmoBox.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API AAmmoBox : public AInventoryItem
{
	GENERATED_BODY()

private:
	// ~Variables
	// The number of rounds (items) in this ammo box
	uint8 NumRounds;
	// ~Variables

public:
	// ~Constructor
	AAmmoBox();

	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void SetNumItems_Implementation(int32 Num) override;
	virtual int32 GetNumItems_Implementation() const override;
	virtual int32 GetNumDefaultItems_Implementation() const override;
	// ~Super Interface

	// Returns whether the ammo of this box is compatible with projectile type
	bool IsCompatibleAmmo(const FAmmoInfo& InAmmo) const;
};
