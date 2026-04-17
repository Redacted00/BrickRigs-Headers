#pragma once

#include "CoreMinimal.h"

struct FFluidDynamicElement
{
	// Location relative to the part root
	FVector Location;
};

struct FFluidDynamicSurface : public FFluidDynamicElement
{
	// Surface normal
	FVector Normal;
	// Radius of the surface
	float Radius;
};

struct FFluidDynamicPontoon : public FFluidDynamicElement
{
	// Float rotation
	FRotator Rotation;
	// Pontoon bounds size
	FVector BoundsSize;

	float GetPontoonRadius() const
	{
		// Make the radius based on the smallest side rather than the actual volume, so the pontoon doesn't poke out of the brick
		return BoundsSize.GetMin() * 0.5f;
	}
};

// This type is used to efficiently store fluid dynamic elements
template <typename T>
class TFluidDynamicElementArray
{
	uint16 NumElements = 0;
	TUniquePtr<T[]> Elements;

public:
	T& operator[](int32 Index)
	{
		return Elements[Index];
	}

	T* TryGet(int32 Index)
	{
		return Index >= 0 && Index < NumElements && Elements.IsValid() ? &Elements[Index] : nullptr;
	}

	auto Num() const
	{
		return NumElements;
	}

	auto Add(int32 Num)
	{
		return NumElements += Num;
	}

	auto Add(bool bAdd)
	{
		return bAdd ? ++NumElements : NumElements;
	}

	auto PreAllocate()
	{
		Elements = MakeUnique<T[]>(NumElements);
		return NumElements;
	}

	void PostAllocate(int32 Num)
	{
		NumElements = Num;
	}

	void Reset()
	{
		NumElements = 0;
		Elements.Reset();
	}

	auto NextIndex()
	{
		return --NumElements;
	}

	T& NextElement()
	{
		return Elements[--NumElements];
	}

	bool IsInitialized() const
	{
		// Array has been initialized if there are no elements or the pointer is valid
		return NumElements == 0 || Elements.IsValid();
	}

	// Iterates over each element and executes a function on it
	void ForEach(const TFunction<void(const T&)>& Func) const
	{
		for (auto i = 0; i < NumElements; ++i)
		{
			Func(Elements[i]);
		}
	}
};

// Contains fluid dynamic surfaces of all types
struct FFluidDynamicElements
{
	TFluidDynamicElementArray<FFluidDynamicSurface> Surfaces;
	TFluidDynamicElementArray<FFluidDynamicPontoon> Pontoons;

	auto IsInitialized() const
	{
		return Surfaces.IsInitialized() && Pontoons.IsInitialized();
	}

	auto Num() const
	{
		return Surfaces.Num() + Pontoons.Num();
	}

	auto PreAllocate()
	{
		return MakeTuple(Surfaces.PreAllocate(), Pontoons.PreAllocate());
	}

	// Restore the counters from pre allocation
	void PostAllocate(const TTuple<uint16, uint16>& Nums)
	{
		Surfaces.PostAllocate(Nums.Key);
		Pontoons.PostAllocate(Nums.Value);
	}

	void Reset()
	{
		Surfaces.Reset();
		Pontoons.Reset();
	}
};
