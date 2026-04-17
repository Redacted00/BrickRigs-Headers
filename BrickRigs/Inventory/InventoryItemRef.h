#pragma once

#include "CoreMinimal.h"
#include "InventoryItemRef.generated.h"

class AInventoryItem;

// A reference to an inventory item, for both replicated and not replicated items
USTRUCT(BlueprintType)
struct FInventoryItemRef
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Pointer to the item actor
	UPROPERTY(BlueprintReadWrite)
	AInventoryItem* Item;
	// Class of the referenced item, required when the actor isn't replicated
	UPROPERTY(BlueprintReadWrite)
	UClass* ItemClass;
	// ~Variables

public:
	FInventoryItemRef(AInventoryItem* InItem = nullptr);

	FInventoryItemRef(UClass* InClass)
		: Item(nullptr), ItemClass(InClass)
	{
	}

	FORCEINLINE bool IsValid() const
	{
		return Item != nullptr || ItemClass != nullptr;
	}

	FORCEINLINE AInventoryItem* GetItem() const
	{
		return Item;
	}

	void SetItem(AInventoryItem* InItem);

	FORCEINLINE UClass* GetItemClass() const
	{
		return ItemClass;
	}

	const AInventoryItem* GetCDO() const;

	// Return the item instance or its CDO if it doesn't exist
	FORCEINLINE const AInventoryItem* GetItemOrCDO() const
	{
		return Item ? Item : GetCDO();
	}

	FORCEINLINE bool operator==(const FInventoryItemRef& Other) const
	{
		return Item == Other.Item && ItemClass == Other.ItemClass;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);
};

template <>
struct TStructOpsTypeTraits<FInventoryItemRef> : public TStructOpsTypeTraitsBase2<FInventoryItemRef>
{
	enum
	{
		// IMPORTANT: Can't use WithNetSharedSerialization since an actor pointer is replicated
		WithNetSerializer = true,
		WithIdenticalViaEquality = true,
	};
};
