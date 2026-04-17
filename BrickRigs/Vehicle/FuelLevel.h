#pragma once

#include "Serialization/SerializationHelper.h"
#include "CoreMinimal.h"
#include "FuelLevel.generated.h"

// Used to access and replicate the fuel level of a vehicle
USTRUCT()
struct FFuelLevel
{
	GENERATED_BODY()
	;

private:
	// Current fuel level, normalized
	float FuelRatio;
	// The total fuel capacity in liters
	float FuelCapacity;

public:
	// ~Constructor
	FFuelLevel()
		: FuelRatio(1.f), FuelCapacity(0.f)
	{
		// NOTE: Default the fuel ratio to 1, so it doesn't have to be replicated initially most of the time
	}

	FORCEINLINE bool operator==(const FFuelLevel& Other) const
	{
		return FuelRatio == Other.FuelRatio && FuelCapacity == Other.FuelCapacity;
	}

	FORCEINLINE float GetCapacity() const
	{
		return FuelCapacity;
	}

	FORCEINLINE void SetCapacity(float NewCapacity)
	{
		FuelCapacity = NewCapacity;
	}

	FORCEINLINE bool HasAnyFuel() const
	{
		return FuelRatio > 0.f;
	}

	FORCEINLINE float GetAbsolute() const
	{
		return FuelRatio * FuelCapacity;
	}

	FORCEINLINE void SetAbsolute(float NewLevel)
	{
		FuelRatio = FuelCapacity != 0.f ? NewLevel / FuelCapacity : 0.f;
	}

	FORCEINLINE float GetRelative() const
	{
		return FuelRatio;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		// Compress the fuel level
		FSerializationHelper::SerializeFloatCompressed(Ar, FuelRatio, 0.f, 1.f, 8);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FFuelLevel> : public TStructOpsTypeTraitsBase2<FFuelLevel>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
