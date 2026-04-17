// Copyright Fluppisoft, 2020

#pragma once

#include "Components/FirearmComponent.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentItemWidget.generated.h"

// Actors
class AInventoryItem;
class AFirearm;
// Components
class UInventoryComponent;

/**
 * Used to display the current item or firearm component
 */
UCLASS(Abstract)
class BRICKRIGS_API UCurrentItemWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	TWeakObjectPtr<AInventoryItem> CurrentItem;
	// ~Variables

public:
	// ~Constructor
	UCurrentItemWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Updates the displayed item
	void UpdateCurrentItem(AInventoryItem* NewItem);

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOut(bool bImmediate);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItemName(const FText& NewName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFireMode(EFireMode NewMode);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmo(int32 Current, int32 Capacity, int32 Ammo);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAmmoType(EAmmoType NewType);
	// ~Blueprint Interface

private:
	// Returns the current item casted to a firearm
	AFirearm* GetFirearm() const;
	// Returns the firearm component from the current firearm if possible
	UFirearmComponent* GetFirearmComponent() const;
	// Returns the owning inventory of the item
	UInventoryComponent* GetItemInventory() const;
	// Updates the displayed ammo
	void UpdateAmmo();
	// Update the fire mode displayed
	void UpdateFireMode();
	// Update the ammo type displayed
	void UpdateAmmoType();
	// Callbacks for the firearm component
	void OnFirearmStateChanged(const FFirearmState& NewState);
	void OnFireModeChanged(EFireMode NewMode);
	void OnAmmoTypeChanged(EAmmoType NewType);
	// Callback for the inventory component
	void OnInventoryModified();
};
