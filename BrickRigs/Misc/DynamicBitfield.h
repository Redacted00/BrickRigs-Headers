#pragma once

#include "CoreMinimal.h"
#include "BrickStatics.h"
#include "Online/BitfieldSerializer.h"

// The dynamic bitfield struct sparsely stores a small amount of data (like a bool) for a large amount of elements
// This is useful to reduce the memory cost for replicating broken vehicle connections for example
// The implementation is a bit weird and uses static functions because it also has to work for fast array serializers, where the items array has exist in the parent struct as a UPROPERTY

template <typename FInElementType, typename FBitfieldType, uint8 NumBitfields, FBitfieldType NumBitsPerElement>
struct TDynamicBitfieldChunk
{
	using FElementType = FInElementType;
	const static FBitfieldType NumBytesPerBitfield = sizeof(FBitfieldType);
	const static FBitfieldType NumBitsPerBitfield = NumBytesPerBitfield * 8;
	const static uint16 NumElementsPerBitfield = NumBitsPerBitfield / NumBitsPerElement;
	const static uint16 NumElementsPerChunk = NumBitfields * NumElementsPerBitfield;
	const static FBitfieldType NumBitsPerBitfieldUsed = NumElementsPerBitfield * NumBitsPerElement;
	const static FBitfieldType NumBytesPerChunk = NumBitfields * NumBytesPerBitfield;
	const static FBitfieldType MaxBitfieldValue = UBrickStatics::GenerateIntWithLeadingOnes<FBitfieldType>(NumBitsPerBitfieldUsed);
	const static FBitfieldType FirstElementBitmask = UBrickStatics::GenerateIntWithLeadingOnes<FBitfieldType>(NumBitsPerElement);

private:
	// ~Variables
	// The bits that represent the per element data
	TStaticArray<FBitfieldType, NumBitfields> Bitfields;

public:
	// Index of the chunk
	uint16 ChunkIndex = 0;
	// ~Variables

	// ~Constructor
	TDynamicBitfieldChunk()
		: Bitfields(0)
	{
	}

	// Resets all data, but NOT the comparison field
	void Reset()
	{
		for (auto& Bitfield : Bitfields)
		{
			Bitfield = 0;
		}
	}

	// Returns true if all bits are set to zero
	bool IsZero() const
	{
		for (const auto& Bitfield : Bitfields)
		{
			if (Bitfield != 0)
			{
				return false;
			}
		}

		return true;
	}

	// Returns true if all bits are set to one
	bool IsOne() const
	{
		for (const auto& Bitfield : Bitfields)
		{
			if (Bitfield != MaxBitfieldValue)
			{
				return false;
			}
		}

		return true;
	}

private:
	// Converts an absolute element index to the relative bit index
	void GetBitIndex(uint16 ElementIndex, uint16& OutBitfieldIndex, uint16& OutBitIndex) const
	{
		ElementIndex -= ChunkIndex * NumElementsPerChunk;
		OutBitfieldIndex = ElementIndex / NumElementsPerBitfield;
		check(OutBitfieldIndex < NumBitfields);
		OutBitIndex = (ElementIndex - OutBitfieldIndex * NumElementsPerBitfield) * NumBitsPerElement;
		check(OutBitIndex + NumBitsPerElement <= NumBitsPerBitfield);
	}

	static FBitfieldType GenerateElementBitmask(uint16 BitIndex)
	{
		return FirstElementBitmask << FBitfieldType(BitIndex);
	}

public:
	FElementType GetElement(const uint16 ElementIndex) const
	{
		uint16 BitfieldIndex, BitIndex;
		GetBitIndex(ElementIndex, BitfieldIndex, BitIndex);
		return FElementType(Bitfields[BitfieldIndex] >> BitIndex & FirstElementBitmask);
	}

	void SetElement(const uint16 ElementIndex, const FElementType Value)
	{
		uint16 BitfieldIndex, BitIndex;
		GetBitIndex(ElementIndex, BitfieldIndex, BitIndex);

		// Clear all bits, then set them
		const auto Bitmask = GenerateElementBitmask(BitIndex);
		Bitfields[BitfieldIndex] &= ~Bitmask;
		Bitfields[BitfieldIndex] |= FBitfieldType(Value) << FBitfieldType(BitIndex) & Bitmask;
	}

	// Executes a custom function on each element
	void ForEachElement(const TFunction<void(uint16, FElementType)>& Func) const
	{
		for (auto i = 0; i < NumElementsPerChunk; ++i)
		{
			const auto ElementIdx = i + ChunkIndex * NumElementsPerChunk;
			Func(ElementIdx, GetElement(ElementIdx));
		}
	}

	// Executes a custom function on each changed element
	void ForEachChangedElement(TDynamicBitfieldChunk& Other, const bool bIsBeingRemoved, const TFunction<void(uint16, FElementType)>& Func) const
	{
		for (auto BitfieldIdx = 0; BitfieldIdx < NumBitfields; ++BitfieldIdx)
		{
			if (bIsBeingRemoved || Bitfields[BitfieldIdx] != Other.Bitfields[BitfieldIdx])
			{
				for (auto i = 0; i < NumElementsPerBitfield; ++i)
				{
					const auto Bitmask = GenerateElementBitmask(i * NumBitsPerElement);
					// When the item is being removed, all elements that weren't zero previously will be new zero and thus changed
					if (bIsBeingRemoved ? (Bitfields[BitfieldIdx] & Bitmask) != 0 : (Bitfields[BitfieldIdx] & Bitmask) != (Other.Bitfields[BitfieldIdx] & Bitmask))
					{
						const auto ElementIdx = ChunkIndex * NumElementsPerChunk + BitfieldIdx * NumElementsPerBitfield + i;
						// NOTE: When the item is being removed the value won't be set to zero
						Func(ElementIdx, bIsBeingRemoved ? FElementType(0) : GetElement(ElementIdx));
					}
				}

				// Update the comparison chunk
				Other.Bitfields[BitfieldIdx] = Bitfields[BitfieldIdx];
			}
		}
	}

	// Clears the changed state
	void ClearChangedElements(TDynamicBitfieldChunk& Other) const
	{
		Other.Bitfields = Bitfields;
	}

	bool operator==(const TDynamicBitfieldChunk& Other) const
	{
		return ChunkIndex == Other.ChunkIndex && Bitfields == Other.Bitfields;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		// NOTE: We assume that we never need the full 16 bits for the chunk index and instead use the last bit to optimize the case of all elements being set to 1
		auto Bitfield = TBitfieldSerializer<uint16>(Ar);
		Bitfield.Add(ChunkIndex, 15);
		auto bIsOne = Ar.IsLoading() || IsOne();
		Bitfield.Add(bIsOne);
		Bitfield.Finish();

		if (bIsOne)
		{
			// Set all bitfields to 1
			if (Ar.IsLoading())
			{
				for (auto& Item : Bitfields)
				{
					Item = MaxBitfieldValue;
				}
			}
		}
		else
		{
			// Serialize all bitfields
			for (auto& Item : Bitfields)
			{
				Ar << Item;
			}
		}

		return !Ar.IsError();
	}
};

template <typename T>
constexpr bool IsFastArray = std::is_base_of_v<FFastArraySerializer, T>;

struct FDynamicBitfield
{
	// Returns the chunk index associated with the given element
	template <typename FChunkType>
	static uint16 GetChunkIndex(const uint16 ElementIndex)
	{
		return FMath::DivideAndRoundDown(ElementIndex, FChunkType::NumElementsPerChunk);
	}

	// Returns the index of them item containing the given element, or INDEX_NONE if it doesn't exist
	template <typename FChunkType, typename T>
	static int32 FindItem(const T& Array, const uint16 ElementIndex)
	{
		const auto ChunkIndex = GetChunkIndex<FChunkType>(ElementIndex);
		return Array.Items.IndexOfByPredicate([ChunkIndex](const auto& Item)
		{
			return Item.Chunk.ChunkIndex == ChunkIndex;
		});
	}

	// Returns the current value of the element
	template <typename FChunkType, typename FElementType = typename FChunkType::FElementType, typename T>
	static FElementType GetElement(const T& Array, const uint16 ElementIndex)
	{
		const auto Index = FindItem<FChunkType>(Array, ElementIndex);
		if (Index != INDEX_NONE)
		{
			return Array.Items[Index].Chunk.GetElement(ElementIndex);
		}

		return FElementType(0);
	}

	// Sets the value of the given element index
	template <typename FChunkType, typename FElementType = typename FChunkType::FElementType, typename T>
	static void SetElement(T& Array, const uint16 ElementIndex, const FElementType NewElement)
	{
		auto Index = FindItem<FChunkType>(Array, ElementIndex);

		// Add a new chunk if needed, but don't bother if it would just be empty
		const auto bAddNewItem = Index == INDEX_NONE && NewElement != FElementType(0);
		if (bAddNewItem)
		{
			Index = Array.Items.Emplace();
			Array.Items[Index].Chunk.ChunkIndex = GetChunkIndex<FChunkType>(ElementIndex);
		}

		if (Index != INDEX_NONE)
		{
			//UE_LOG(LogTemp, Warning, TEXT("SetElement: ElementIndex %d NewElement %d"), ElementIndex, NewElement);

			// Update the chunk
			Array.Items[Index].Chunk.SetElement(ElementIndex, NewElement);
			ensure(Array.Items[Index].Chunk.GetElement(ElementIndex) == NewElement);

			// Remove empty items
			if (Array.Items[Index].Chunk.IsZero())
			{
				// Makes no sense to add an item and remove it right again
				ensure(!bAddNewItem);
				Array.Items.RemoveAtSwap(Index, 1, false);

				// Mark fast arrays dirty when removing an item
				if constexpr (IsFastArray<T>)
				{
					Array.MarkArrayDirty();
				}
			}
			else
			{
				// Mark newly added or modified items dirty
				if constexpr (IsFastArray<T>)
				{
					Array.MarkItemDirty(Array.Items[Index]);
				}
			}
		}
	}

	// Clears all items
	template <typename T>
	static void Reset(T& Array)
	{
		Array.Items.Empty();

		if constexpr (IsFastArray<T>)
		{
			Array.MarkArrayDirty();
		}
	}

	// Executes a custom function on each element
	template <typename FChunkType, typename FElementType = typename FChunkType::FElementType, typename T>
	static void ForEachElement(const T& Array, const TFunction<void(uint16, FElementType)>& Func)
	{
		for (const auto& Item : Array.Items)
		{
			Item.Chunk.ForEachElement(Func);
		}
	}

	// Executes a custom function on each changed element
	template <typename FChunkType, typename FElementType = typename FChunkType::FElementType, typename T>
	static void ForEachChangedElement(T& Array, const TArrayView<int32>& ChangedItemIndices, const bool bRemovingItems, const TFunction<void(uint16, FElementType)>& Func)
	{
		for (const auto& ItemIndex : ChangedItemIndices)
		{
			auto& Chunk = Array.Items[ItemIndex].Chunk;
			auto& ComparisonChunk = Array.Items[ItemIndex].ComparisonChunk;
			Chunk.ForEachChangedElement(ComparisonChunk, bRemovingItems, Func);
		}
	}

	// Clears changed elements without iterating them
	template <typename T>
	static void ClearChangedElements(T& Array, const TArrayView<int32>& ChangedItemIndices, const bool bRemovingItems)
	{
		for (const auto& ItemIndex : ChangedItemIndices)
		{
			auto& Chunk = Array.Items[ItemIndex].Chunk;
			auto& ComparisonChunk = Array.Items[ItemIndex].ComparisonChunk;
			Chunk.ClearChangedElements(ComparisonChunk);
		}
	}

	// Calculates the maximum number of array items needed to store all elements
	template <typename FChunkType>
	static int32 GetMaxNumItems(const int32 MaxNumElements)
	{
		return FMath::DivideAndRoundUp(MaxNumElements, static_cast<int32>(FChunkType::NumElementsPerChunk));
	}

	// Used for fast array replication events
	template <typename T>
	static void PostReplicatedChange(T& Array, const TArrayView<int32>& ChangedIndices, const bool bRemovingItems)
	{
		if (!Array.ShouldCallOnReplicatedChange(bRemovingItems))
		{
			// Just clear changed elements without iterating them
			FDynamicBitfield::ClearChangedElements(Array, ChangedIndices, bRemovingItems);
			return;
		}

		Array.OnReplicatedChange(ChangedIndices, bRemovingItems);
	}
};

// Declares common functions for classes implementing dynamic bitfields
#define IMPLEMENT_DYNAMIC_BITFIELD(FChunkType, FElementType, ElementName) \
FElementType Get##ElementName(const uint16 BrickIndex) const \
{ \
	return FDynamicBitfield::GetElement<FChunkType>(*this, BrickIndex); \
} \
void Set##ElementName(const uint16 BrickIndex, const FElementType& NewDamage) \
{ \
	FDynamicBitfield::SetElement<FChunkType>(*this, BrickIndex, NewDamage); \
} \
void ForEach##ElementName(const TFunction<void(uint16, const FElementType&)>& Func) const \
{ \
	FDynamicBitfield::ForEachElement<FChunkType, FElementType>(*this, [&](const uint16 BrickIdx, const FElementType Damage) \
	{ \
		Func(BrickIdx, Damage); \
	}); \
} \
void Reset() \
{ \
	FDynamicBitfield::Reset(*this); \
} \
bool IsEmpty() const \
{ \
	return Items.Num() == 0; \
} \
static int32 GetMaxNumItems(const int32 NumConnections) \
{ \
	return FDynamicBitfield::GetMaxNumItems<FChunkType>(NumConnections); \
}

// Declares common dynamic bitfield functions as well as fast array specific functions
#define IMPLEMENT_DYNAMIC_BITFIELD_FAST_ARRAY(FChunkType, FElementType, ElementName) \
IMPLEMENT_DYNAMIC_BITFIELD(FChunkType, FElementType, ElementName) \
void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize) \
{ \
	FDynamicBitfield::PostReplicatedChange(*this, RemovedIndices, true); \
} \
void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize) \
{ \
	FDynamicBitfield::PostReplicatedChange(*this, AddedIndices, false); \
} \
void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize) \
{ \
	FDynamicBitfield::PostReplicatedChange(*this, ChangedIndices, false); \
}
