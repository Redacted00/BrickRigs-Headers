// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "LoadoutInventoryComponent.generated.h"

class ALoadoutContainer;

/**
 * Specialized inventory component used for the loadout container
 * NOTE: Only one if these is created per level
 */
UCLASS()
class BRICKRIGS_API ULoadoutInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

	// ~Variables
	// Object through which the loadout is being accessed
	FWeakObjectPtr LoadoutAccessor;
	// ~Variables

public:
	// ~Constructor
	ULoadoutInventoryComponent();

	// ~Super Interface
	virtual void BeginPlay() override;

	virtual bool IsLoadoutContainer() const override
	{
		return true;
	}

	virtual bool ShouldReplicateItem(const AInventoryItem* InItem) const override
	{
		// Never spawn items on the loadout container
		return false;
	}

	virtual int32 CanAddItems(const FInventorySlot& InSlot, bool bSpawningItems, bool bIgnoreIfOccupied) const override;
	virtual int32 CanRemoveItems(const FInventorySlotID& SlotID, int32 Amount) const override;
	virtual void OnIsAccessedByPlayerChanged() override;
	// ~Super Interface

	// Sets the object currently accessing the loadout
	void SetLoadoutAccessor(UObject* InAccessor);
};
