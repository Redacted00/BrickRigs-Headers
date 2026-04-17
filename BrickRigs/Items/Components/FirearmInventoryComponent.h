// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "FirearmInventoryComponent.generated.h"

class AFirearm;

/**
 * 
 */
UCLASS(Within = Firearm)
class BRICKRIGS_API UFirearmInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	// ~Constructor
	UFirearmInventoryComponent();

	// ~Super Interface
	virtual int32 CanAddItems(const FInventorySlot& InSlot, bool bSpawningItems, bool bIgnoreIfOccupied) const override;
	virtual bool GetItemAttachInfo(AInventoryItem* InItem, USceneComponent*& OutAttachParent, FName& OutAttachSocket, FVector& OutLocation, FRotator& OutRotation) const override;
	// ~Super Interface

	// Helper function to get the owning firearm
	AFirearm* GetFirearm() const;
};
