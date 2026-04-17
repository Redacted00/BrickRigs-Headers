#pragma once

#include "CoreMinimal.h"

// Helper struct to serialize bitfiels
template <class T>
struct TBitfieldSerializer
{
	TBitfieldSerializer(FArchive& InAr)
		: Ar(InAr), Bitfield(0), Index(0), bCalledFinish(false)
	{
		if (Ar.IsLoading())
		{
			Ar << Bitfield;
		}
	}

	~TBitfieldSerializer()
	{
		ensure(bCalledFinish);
	}

	template <typename V>
	void Add(V& Value, T NumBits)
	{
		check(NumBits > 0);
		ensure(Index + NumBits <= sizeof(T) * 8);

		T BitMask = 0;
		for (uint8 i = 0; i < NumBits; ++i)
		{
			BitMask |= 1 << i;
		}

		if (Ar.IsLoading())
		{
			Value = V((Bitfield >> Index) & BitMask);
		}
		else
		{
			Bitfield |= (T(Value) & BitMask) << Index;
		}

		Index += NumBits;
	}

	// Specialized version for bools
	void Add(bool& bValue)
	{
		Add(bValue, 1);
	}

	// Specialized version for bitfield bools, which cannot take a reference
#define AddBitfieldBool(Bitfield, bValue) bValue = Bitfield.AddBitfieldBoolInternal(bValue)

	bool AddBitfieldBoolInternal(bool bValue)
	{
		auto bTmp = bValue;
		Add(bTmp, 1);
		return bTmp;
	}

	void Finish()
	{
		ensure(!bCalledFinish);
		bCalledFinish = true;

		if (Ar.IsSaving())
		{
			Ar << Bitfield;
		}
	}

private:
	FArchive& Ar;
	T Bitfield;
	uint8 Index;
	bool bCalledFinish;
};
