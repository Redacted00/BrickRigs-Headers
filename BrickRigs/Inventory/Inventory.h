#pragma once

#include "InventorySlot.h"
#include "Engine/NetSerialization.h"
#include "CoreMinimal.h"
#include "Inventory.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct FInventory : public FFastArraySerializer
{
	GENERATED_BODY()

private:
	// ~Variables
	// The inventory component housing this struct
	UInventoryComponent* OwnerInventory;

public:
	// The inventory slots, note this has to be named Items for the fast array serializer
	UPROPERTY(BlueprintReadOnly)
	TArray<FInventorySlot> Items;
	// ~Variables

	FInventory(UInventoryComponent* InInventory = nullptr)
		: OwnerInventory(InInventory)
	{
	}

	// Templated callbacks for the fast array serializer
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	// NOTE: The normal = operator won't account for fast array serialization keys etc.
	bool CopyInventory(const FInventory& Other);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FInventorySlot, FInventory>(Items, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FInventory> : public TStructOpsTypeTraitsBase2<FInventory>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
