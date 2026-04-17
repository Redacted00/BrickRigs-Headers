#pragma once

#include "Bricks/Misc/BrickDamage.h"
#include "Serialization/FastArraySerializerChunk.h"
#include "CoreMinimal.h"
#include "VehicleDamage.generated.h"

// The FBrickConnectionDamage struct is used to replicate the vehicle status more efficiently across the network.
// The idea is that the broken status of all brick connections is stored in a bit array, which itself is divded into smaller chunks.
// Using the fast array serializer, only chunks that have been modified have to be replicated.

USTRUCT()
struct FBrickConnectionDamageItem : public FFastArraySerializerItem
{
	GENERATED_BODY()
	;

public:
	using ElementType = bool;
	using ChunkType = FFastArraySerializerChunk<FBrickConnectionDamageItem, ElementType, uint64, 2, 1>;

	// ~Variables
	ChunkType Chunk;
	// ~Variables

	// ~Constructor
	FBrickConnectionDamageItem()
		: FFastArraySerializerItem(), Chunk()
	{
	}

	bool operator==(const FBrickConnectionDamageItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		return Chunk.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FBrickConnectionDamageItem> : public TStructOpsTypeTraitsBase2<FBrickConnectionDamageItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FBrickConnectionDamageArray : public FFastArraySerializer
{
	GENERATED_BODY()
	;

private:
	using ChunkArrayType = FFastArraySerializerChunkArray<FBrickConnectionDamageArray, FBrickConnectionDamageItem, FBrickConnectionDamageItem::ChunkType, FBrickConnectionDamageItem::ElementType>;

	// ~Variables
	// Used to cache the connections that have just been broken or repaired
	TArray<uint16> ChangedConnectionIndices;

public:
	// The damage chunks, note that this has to be named Items for the fast array serializer
	UPROPERTY()
	TArray<FBrickConnectionDamageItem> Items;
	// ~Variables

	const TArray<uint16>& GetChangedConnectionIndices() const
	{
		return ChangedConnectionIndices;
	}

	// Clears the pending broken and repaired connections
	void ClearChangedConnections()
	{
		ChangedConnectionIndices.Reset();
	}

	// Return whether the given connenction is marked as broken
	bool IsConnectionBroken(uint16 ConnectionIndex) const
	{
		bool bIsBroken;
		return ChunkArrayType::GetElement(*this, ConnectionIndex, bIsBroken) && bIsBroken;
	}

	void SetConnectionsBroken(const TArray<uint16>& ConnectionIndices, bool bNewBroken)
	{
		// IMPORTANT: Never remove a chunk from the array, just replicate it as empty
		// This has to be done to avoid the case where a vehicle is repaired and immediatelly breaks again on the server.
		// The client would be notified about the connections breaking and the same connections being repaired,
		// possibly ending up with connections repaired that should be broken.
		for (uint16 ConnectionIndex : ConnectionIndices)
		{
			ChunkArrayType::SetElement(*this, ConnectionIndex, bNewBroken, false);
		}
	}

	// Templated callbacks for the fast array serializer
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	// NOTE: We need a normal non delta serializer for RPC usage
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		uint16 NumItems = Items.Num();
		Ar << NumItems;

		if (Ar.IsLoading())
		{
			Items.SetNumZeroed(NumItems);
		}

		for (auto& Item : Items)
		{
			Item.NetSerialize(Ar, Map, bOutSuccess);
		}

		if (Ar.IsLoading())
		{
			// Gather the changed connections
			ChangedConnectionIndices.Reset();
			ChunkArrayType::GatherAbsoluteReplicatedChange(*this, ChangedConnectionIndices);
		}

		return true;
	}

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FBrickConnectionDamageItem, FBrickConnectionDamageArray>(Items, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FBrickConnectionDamageArray> : public TStructOpsTypeTraitsBase2<FBrickConnectionDamageArray>
{
	enum
	{
		WithNetDeltaSerializer = true,
		WithNetSerializer = true
	};
};

// The FBrickDamage struct is used to replicate generic per brick damage info
// Like if the brick is on fire, or has already been burnt

USTRUCT()
struct FBrickDamageItem : public FFastArraySerializerItem
{
	GENERATED_BODY()
	;

public:
	using ElementType = uint8;
	using ChunkType = FFastArraySerializerChunk<FBrickDamageItem, ElementType, uint64, 2, 3>;

	// ~Variables
	ChunkType Chunk;
	// ~Variables

	// ~Constructor
	FBrickDamageItem()
		: FFastArraySerializerItem(), Chunk()
	{
	}

	bool operator==(const FBrickDamageItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		return Chunk.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FBrickDamageItem> : public TStructOpsTypeTraitsBase2<FBrickDamageItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FBrickDamageArray : public FFastArraySerializer
{
	GENERATED_BODY()
	;

private:
	using ChunkArrayType = FFastArraySerializerChunkArray<FBrickDamageArray, FBrickDamageItem, FBrickDamageItem::ChunkType, FBrickDamageItem::ElementType>;

	// ~Variables
	// List of bricks for which the damage has changed after replication
	TArray<uint16> ChangedBrickIndices;

public:
	// The damage chunks, note that this has to be named Items for the fast array serializer
	UPROPERTY()
	TArray<FBrickDamageItem> Items;
	// ~Variables

	const TArray<uint16>& GetChangedBrickIndices() const
	{
		return ChangedBrickIndices;
	}

	void ClearChangedBricks()
	{
		ChangedBrickIndices.Reset();
	}

	bool GetBrickDamage(uint16 BrickIndex, FBrickDamage& OutDamage) const
	{
		uint8 Data = 0;
		const bool bRetVal = ChunkArrayType::GetElement(*this, BrickIndex, Data);
		OutDamage.SetCompressed(Data);
		return bRetVal;
	}

	void SetBrickDamage(uint16 BrickIndex, const FBrickDamage& NewDamage)
	{
		ChunkArrayType::SetElement(*this, BrickIndex, NewDamage.GetCompressed(), true);
	}

	void Empty()
	{
		Items.Empty();
		MarkArrayDirty();
	}

	// Templated callbacks for the fast array serializer
	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FastArrayDeltaSerialize<FBrickDamageItem, FBrickDamageArray>(Items, DeltaParms, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FBrickDamageArray> : public TStructOpsTypeTraitsBase2<FBrickDamageArray>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};
