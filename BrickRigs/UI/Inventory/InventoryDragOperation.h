#pragma once

#include "Inventory/InventorySlot.h"
#include "CoreMinimal.h"

class UInventoryComponent;

struct FInventoryDragOperation
{
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	FInventorySlot Slot;
	bool bIsMouseDragging;
};
