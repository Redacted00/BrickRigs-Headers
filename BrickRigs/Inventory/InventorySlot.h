#pragma once

#include "InventoryItemRef.h"
#include "Engine/NetSerialization.h"
#include "CoreMinimal.h"
#include "InventorySlot.generated.h"

class AInventoryItem;
struct FInventory;

// Used to uniquely identify a slot in an inventory
USTRUCT(BlueprintType)
struct FInventorySlotID
{
	GENERATED_BODY()

protected:
	// The unique ID
	UPROPERTY(BlueprintReadWrite)
	uint8 ID;

public:
	constexpr static auto MaxSlotID = MAX_uint8;

	// Default constructor generates an invalid slot
	FInventorySlotID(int32 Index = INDEX_NONE)
	{
		ID = Index + 1;
	}

	static FInventorySlotID MakeSpecialSlotID(int32 Index)
	{
		return FInventorySlotID(Index);
	}

	static FInventorySlotID MakeWildcardSlotID(int32 Index, int32 NumSpecialSlots)
	{
		return FInventorySlotID(Index + NumSpecialSlots);
	}

	bool IsValid() const
	{
		return ID > 0;
	}

	int32 GetSlotIndex() const
	{
		return ID - 1;
	}

	int32 GetSpecialSlotIndex() const
	{
		return GetSlotIndex();
	}

	bool operator>(const FInventorySlotID& Other) const
	{
		return ID > Other.ID;
	}

	bool operator<(const FInventorySlotID& Other) const
	{
		return ID < Other.ID;
	}

	bool operator<=(const FInventorySlotID& Other) const
	{
		return ID <= Other.ID;
	}

	FInventorySlotID& operator++()
	{
		ID++;
		return *this;
	}

	bool operator==(const FInventorySlotID& Other) const
	{
		return ID == Other.ID;
	}

	bool operator!=(const FInventorySlotID& Other) const
	{
		return ID != Other.ID;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << ID;
		return !Ar.IsError();
	}

	friend uint32 GetTypeHash(const FInventorySlotID& SlotID)
	{
		return GetTypeHash(SlotID.ID);
	}
};

template <>
struct TStructOpsTypeTraits<FInventorySlotID> : TStructOpsTypeTraitsBase2<FInventorySlotID>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true,
	};
};

USTRUCT(BlueprintType)
struct FInventorySlot : public FFastArraySerializerItem
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The unique id of the slot, used to reference items in an inventory
	UPROPERTY(BlueprintReadOnly)
	FInventorySlotID SlotID;
	// The item held by this slot
	UPROPERTY(BlueprintReadOnly)
	FInventoryItemRef ItemRef;
	// Non replicated item instance
	UPROPERTY(BlueprintReadOnly)
	AInventoryItem* LocalItem;
	// The amount of items of this type, not necessarily items, can also be individual rounds for example
	UPROPERTY(BlueprintReadOnly)
	int32 Amount;
	// ~Variables

public:
	// ~Statics
	// Max allowed number of items in a slot, this is limited because of net serialization
	constexpr static auto MaxAmount = static_cast<int32>(MAX_uint16);
	// ~Statics

	FInventorySlot(const FInventorySlotID& InSlotID = FInventorySlotID(), const FInventoryItemRef& InItemRef = FInventoryItemRef(), int32 InAmount = 0)
		: SlotID(InSlotID), ItemRef(InItemRef), LocalItem(nullptr), Amount(InAmount)
	{
	}

	// Custom copy constructor to account for the replication ID etc. which should not be copied
	FInventorySlot(const FInventorySlot& Other)
	{
		*this = Other;
	}

	bool IsValid() const
	{
		return ItemRef.IsValid();
	}

	const FInventorySlotID& GetSlotID() const
	{
		return SlotID;
	}

	void SetSlotID(const FInventorySlotID& InSlotID)
	{
		SlotID = InSlotID;
	}

	AInventoryItem* GetAnyItem() const
	{
		return GetReplicatedItem() ? GetReplicatedItem() : LocalItem;
	}

	AInventoryItem* GetReplicatedItem() const
	{
		return ItemRef.GetItem();
	}

	void SetReplicatedItem(AInventoryItem* InItem)
	{
		ItemRef.SetItem(InItem);
	}

	AInventoryItem* GetLocalItem() const
	{
		return LocalItem;
	}

	void SetLocalItem(AInventoryItem* InItem)
	{
		LocalItem = InItem;
	}

	template <class T = AInventoryItem>
	const T* GetItemOrCDO() const
	{
		return Cast<T>(ItemRef.GetItemOrCDO());
	}

	UClass* GetItemClass() const
	{
		return ItemRef.GetItemClass();
	}

	const FInventoryItemRef& GetItemRef() const
	{
		return ItemRef;
	}

	int32 GetAmount() const
	{
		return Amount;
	}

	void SetAmount(int32 InAmount)
	{
		Amount = FMath::Clamp(InAmount, 0, MaxAmount);
	}

	bool IsEmpty() const
	{
		return Amount == 0;
	}

	bool IsSameSlot(const FInventorySlot& Other) const
	{
		return SlotID == Other.SlotID && IsSameItem(Other);
	}

	bool IsSameItem(const FInventorySlot& Other) const
	{
		return ItemRef == Other.GetItemRef();
	}

	bool IsSameItem(const FInventoryItemRef& InItem) const
	{
		return ItemRef == InItem;
	}

	bool IsSameItemClass(const FInventoryItemRef& InItem) const
	{
		return ItemRef.GetItemClass() == InItem.GetItemClass();
	}

	// Used for sorting
	bool operator<(const FInventorySlot& Other) const
	{
		return SlotID < Other.SlotID;
	}

	bool operator==(const FInventorySlot& Other) const
	{
		return SlotID == Other.SlotID && ItemRef == Other.ItemRef && Amount == Other.Amount;
	}

	bool operator!=(const FInventorySlot& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		SlotID.NetSerialize(Ar, Map, bOutSuccess);

		auto LocalAmount = static_cast<uint16>(Amount);
		Ar << LocalAmount;
		Amount = LocalAmount;

		// IMPORTANT: Make sure to return the success of ItemRef.NetSerialize, since that serializes an object reference
		return ItemRef.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FInventorySlot> : TStructOpsTypeTraitsBase2<FInventorySlot>
{
	enum
	{
		// IMPORTANT: Can't use WithNetSharedSerialization since an actor pointer is replicated
		WithNetSerializer = true,
		WithIdenticalViaEquality = true,
	};
};
