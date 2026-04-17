#pragma once

#include "Engine/NetSerialization.h"
#include "CoreMinimal.h"

// These classes are used to replicate bulk data, like the damage state of bricks

template <typename ItemType, typename ElementType, typename BitfieldType, uint8 NumBitfields, BitfieldType NumBitsPerElement>
struct FFastArraySerializerChunk
{
public:
	const static BitfieldType NumBytesPerBitfield = sizeof(BitfieldType);
	const static BitfieldType NumBitsPerBitfield = NumBytesPerBitfield * 8;
	const static uint16 NumElementsPerBitfield = NumBitsPerBitfield / NumBitsPerElement;
	const static uint16 NumElementsPerChunk = NumBitfields * NumElementsPerBitfield;
	const static BitfieldType NumBytesPerChunk = NumBitfields * NumBytesPerBitfield;

private:
	// ~Variables
	// The bits that represent the per element data
	BitfieldType Bitfields[NumBitfields];
	// Copy of the bitfields that is used to detect changes
	BitfieldType ComparisonBitfields[NumBitfields];

public:
	// Index of the chunk
	uint16 ChunkIndex;
	// ~Variables

	// ~Constructor
	FFastArraySerializerChunk()
		: ChunkIndex(0)
	{
		FMemory::Memzero(Bitfields, NumBytesPerBitfield * NumBitfields);
		FMemory::Memzero(ComparisonBitfields, NumBytesPerBitfield * NumBitfields);
	}

	// Resets all data, but NOT the comparison field
	void Clear()
	{
		FMemory::Memzero(Bitfields, NumBytesPerBitfield * NumBitfields);
	}

	// Returns true if the chunk is null
	bool IsEmpty() const
	{
		for (uint16 i = 0; i < NumBitfields; ++i)
		{
			if (Bitfields[i] != 0)
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

	BitfieldType GenerateBitmask() const
	{
		return (BitfieldType(1) << BitfieldType(NumBitsPerElement)) - BitfieldType(1);
	}

	BitfieldType GenerateElementBitmask(uint16 BitIndex) const
	{
		return GenerateBitmask() << BitfieldType(BitIndex);
	}

public:
	ElementType GetElement(uint16 ElementIndex) const
	{
		uint16 BitfieldIndex, BitIndex;
		GetBitIndex(ElementIndex, BitfieldIndex, BitIndex);
		return (Bitfields[BitfieldIndex] >> BitIndex) & GenerateBitmask();
	}

	void SetElement(uint16 ElementIndex, ElementType Data)
	{
		//UE_LOG(LogTemp, Warning, TEXT("PreSetElement: ElementIndex %d Data %d BitfieldIndex %d BitIndex %d Bitmask %llu Result %llu"), ElementIndex, Data);

		uint16 BitfieldIndex, BitIndex;
		GetBitIndex(ElementIndex, BitfieldIndex, BitIndex);

		// Clear all bits, then set them
		const BitfieldType Bitmask = GenerateElementBitmask(BitIndex);
		Bitfields[BitfieldIndex] &= ~Bitmask;
		Bitfields[BitfieldIndex] |= (BitfieldType(Data) << BitfieldType(BitIndex)) & Bitmask;

		//UE_LOG(LogTemp, Warning, TEXT("SetElement: ChunkIndex %d BitfieldIndex %d BitIndex %d Bitmask %llu Result %llu"), ChunkIndex, BitfieldIndex, BitIndex, Bitmask, Bitfields[BitfieldIndex]);
	}

	// Used to gather element indices that have changed after replication
	void GatherReplicatedChange(TArray<uint16>& OutChangedIndices, bool bIsBeingRemoved)
	{
		for (uint16 BitfieldIdx = 0; BitfieldIdx < NumBitfields; ++BitfieldIdx)
		{
			if (bIsBeingRemoved || Bitfields[BitfieldIdx] != ComparisonBitfields[BitfieldIdx])
			{
				for (uint16 ElementIdx = 0; ElementIdx < NumElementsPerBitfield; ++ElementIdx)
				{
					const auto Bitmask = GenerateElementBitmask(ElementIdx * NumBitsPerElement);
					if (bIsBeingRemoved ? ((Bitfields[BitfieldIdx] & Bitmask) != 0) : ((Bitfields[BitfieldIdx] & Bitmask) != (ComparisonBitfields[BitfieldIdx] & Bitmask)))
					{
						OutChangedIndices.Add(ChunkIndex * NumElementsPerChunk + BitfieldIdx * NumElementsPerBitfield + ElementIdx);
					}
				}

				// Update the comparison bitfield
				ComparisonBitfields[BitfieldIdx] = Bitfields[BitfieldIdx];
			}
		}
	}

	bool operator==(const FFastArraySerializerChunk& Other) const
	{
		return ChunkIndex == Other.ChunkIndex && FMemory::Memcmp(Bitfields, Other.Bitfields, NumBytesPerBitfield * NumBitfields) == 0;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << ChunkIndex;
		Ar.Serialize(Bitfields, NumBytesPerChunk);

		return true;
	}
};

template <typename ArrayType, typename ItemType, typename ChunkType, typename ElementType>
struct FFastArraySerializerChunkArray
{
	static uint16 GetChunkIndex(uint16 ElementIndex)
	{
		return FMath::DivideAndRoundDown(ElementIndex, ChunkType::NumElementsPerChunk);
	}

	static const ItemType* FindItem(const ArrayType& Array, uint16 ElementIndex)
	{
		const uint16 ChunkIndex = GetChunkIndex(ElementIndex);
		return Array.Items.FindByPredicate([ChunkIndex](const ItemType& Item)
		{
			return Item.Chunk.ChunkIndex == ChunkIndex;
		});
	}

	// Non const version
	static ItemType* FindItem(ArrayType& Array, uint16 ElementIndex)
	{
		const uint16 ChunkIndex = GetChunkIndex(ElementIndex);
		return Array.Items.FindByPredicate([ChunkIndex](const ItemType& Item)
		{
			return Item.Chunk.ChunkIndex == ChunkIndex;
		});
	}

	static bool GetElement(const ArrayType& Array, uint16 ElementIndex, ElementType& OutElement)
	{
		const ItemType* Item = FindItem(Array, ElementIndex);
		if (Item)
		{
			OutElement = Item->Chunk.GetElement(ElementIndex);
			return true;
		}

		return false;
	}

	static void SetElement(ArrayType& Array, uint16 ElementIndex, ElementType NewElement, bool bRemoveEmpty)
	{
		ItemType* Item = FindItem(Array, ElementIndex);

		// Add a new chunk if needed, but don't bother if it would just be empty
		const bool bAddNewItem = !Item && NewElement != 0;
		if (bAddNewItem)
		{
			Item = &Array.Items[Array.Items.Add(ItemType())];
			Item->Chunk.ChunkIndex = GetChunkIndex(ElementIndex);
		}

		if (Item)
		{
			//UE_LOG(LogTemp, Warning, TEXT("SetElement: ElementIndex %d NewElement %d"), ElementIndex, NewElement);

			// Update the chunk
			Item->Chunk.SetElement(ElementIndex, NewElement);
			ensure(Item->Chunk.GetElement(ElementIndex) == NewElement);

			if (bRemoveEmpty && Item->Chunk.IsEmpty())
			{
				// Makes no sense to add an item and remove it right again
				ensure(!bAddNewItem);
				Array.Items.RemoveSingleSwap(*Item, false);
				// Need to mark the entirey array dirty when an item was removed
				Array.MarkArrayDirty();
			}
			else
			{
				// Mark the item dirty, needed when it was changed or added
				Array.MarkItemDirty(*Item);
			}
		}
	}

	// Used to gather element indices that have changed after delta replication
	static void GatherReplicatedChange(ArrayType& Array, const TArrayView<int32>& ChangedItemIndices, TArray<uint16>& OutChangedElementIndices, bool bRemovingItems = false)
	{
		if (ChangedItemIndices.Num())
		{
			const int32 NumToReserve = ChangedItemIndices.Num() * ChunkType::NumElementsPerChunk;
			OutChangedElementIndices.Reserve(OutChangedElementIndices.Num() + NumToReserve);

			// Go through all changed or added items and find their deltas
			for (int32 ItemIndex : ChangedItemIndices)
			{
				ChunkType& Chunk = Array.Items[ItemIndex].Chunk;
				Chunk.GatherReplicatedChange(OutChangedElementIndices, bRemovingItems);
			}
		}
	}

	// Used to gather element indices that have changed after normal serialization
	static void GatherAbsoluteReplicatedChange(ArrayType& Array, TArray<uint16>& OutChangedElementIndices)
	{
		if (Array.Items.Num())
		{
			const int32 NumToReserve = Array.Items.Num() * ChunkType::NumElementsPerChunk;
			OutChangedElementIndices.Reserve(OutChangedElementIndices.Num() + NumToReserve);

			// Go through all items
			for (auto& Item : Array.Items)
			{
				ChunkType& Chunk = Item.Chunk;
				Chunk.GatherReplicatedChange(OutChangedElementIndices, false);
			}
		}
	}
};
