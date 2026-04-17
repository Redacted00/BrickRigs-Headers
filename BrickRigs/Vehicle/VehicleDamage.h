#pragma once

#include "Bricks/Misc/BrickDamage.h"
#include "CoreMinimal.h"
#include "Misc/DynamicBitfield.h"
#include "Serialization/FluSerializationStatics.h"
#include "VehicleDamage.generated.h"

// The FRepBrickConnectionDamage struct is used to replicate the vehicle condition more efficiently across the network.
// The idea is that the broken status of all brick connections is stored in a bit array, which itself is divided into smaller chunks.
// Using the fast array serializer, only chunks that have been modified have to be replicated.

class ABrickVehicle;

USTRUCT()
struct FRepBrickConnectionDamageItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	using FChunkType = TDynamicBitfieldChunk<bool, uint64, 2, 1>;

	// ~Variables
	FChunkType Chunk;
	FChunkType ComparisonChunk;
	// ~Variables

	bool operator==(const FRepBrickConnectionDamageItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Chunk.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickConnectionDamageItem> : TStructOpsTypeTraitsBase2<FRepBrickConnectionDamageItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FRepBrickConnectionDamage : public FFastArraySerializer
{
	GENERATED_BODY()

	using FChunkType = FRepBrickConnectionDamageItem::FChunkType;

private:
	// ~Variables
	TWeakObjectPtr<ABrickVehicle> Vehicle;

public:
	// The damage chunks, note that this has to be named Items for the fast array serializer
	UPROPERTY()
	TArray<FRepBrickConnectionDamageItem> Items;
	// ~Variables

	// ~Constructor
	explicit FRepBrickConnectionDamage(ABrickVehicle* Vehicle = nullptr);

	IMPLEMENT_DYNAMIC_BITFIELD_FAST_ARRAY(FChunkType, FChunkType::FElementType, ConnectionBroken);

	bool ShouldCallOnReplicatedChange(const bool bRemovingItems) const;
	void OnReplicatedChange(const TArrayView<int32>& ChangedIndices, const bool bRemovingItems);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FRepBrickConnectionDamageItem, FRepBrickConnectionDamage>(Items, DeltaParams, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickConnectionDamage> : TStructOpsTypeTraitsBase2<FRepBrickConnectionDamage>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

// The FBrickConnectionDamageBitfield is the non fast array version of the connection damage bitfield, useful for RPCs and caching connections to break

struct FBrickConnectionDamageBitfieldItem
{
	using FChunkType = TDynamicBitfieldChunk<bool, uint64, 1, 1>;

	// ~Variables
	FChunkType Chunk;
	// ~Variables

	bool operator==(const FBrickConnectionDamageBitfieldItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Chunk.NetSerialize(Ar, Map, bOutSuccess);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FBrickConnectionDamageBitfieldItem> : TStructOpsTypeTraitsBase2<FBrickConnectionDamageBitfieldItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FBrickConnectionDamageBitfield
{
	GENERATED_BODY()

	using FChunkType = FBrickConnectionDamageBitfieldItem::FChunkType;

	// ~Variables
	TArray<FBrickConnectionDamageBitfieldItem> Items;
	// ~Variables

	IMPLEMENT_DYNAMIC_BITFIELD(FChunkType, FChunkType::FElementType, ConnectionBroken);

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		FFluSerializationStatics::NetSerializeArray<uint16>(Ar, Map, bOutSuccess, Items);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FBrickConnectionDamageBitfield> : TStructOpsTypeTraitsBase2<FBrickConnectionDamageBitfield>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

// The FRepBrickDamage struct is used to replicate generic per brick damage info
// Like if the brick is on fire, or has already been burnt

USTRUCT()
struct FRepBrickDamageItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	using FChunkType = TDynamicBitfieldChunk<FBrickDamage, uint64, 2, 3>;

	// ~Variables
	FChunkType Chunk;
	FChunkType ComparisonChunk;
	// ~Variables

	bool operator==(const FRepBrickDamageItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Chunk.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickDamageItem> : TStructOpsTypeTraitsBase2<FRepBrickDamageItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FRepBrickDamage : public FFastArraySerializer
{
	GENERATED_BODY()

	using FChunkType = FRepBrickDamageItem::FChunkType;

private:
	// ~Variables
	TWeakObjectPtr<ABrickVehicle> Vehicle;

public:
	// The damage chunks, note that this has to be named Items for the fast array serializer
	UPROPERTY()
	TArray<FRepBrickDamageItem> Items;
	// ~Variables

	// ~Constructor
	explicit FRepBrickDamage(ABrickVehicle* Vehicle = nullptr);

	IMPLEMENT_DYNAMIC_BITFIELD_FAST_ARRAY(FChunkType, FChunkType::FElementType, BrickDamage);

	bool ShouldCallOnReplicatedChange(const bool bRemovingItems) const;
	void OnReplicatedChange(const TArrayView<int32>& ChangedIndices, const bool bRemovingItems);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FRepBrickDamageItem, FRepBrickDamage>(Items, DeltaParams, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickDamage> : TStructOpsTypeTraitsBase2<FRepBrickDamage>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
