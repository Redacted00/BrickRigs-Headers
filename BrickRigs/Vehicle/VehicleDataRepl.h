#pragma once

#include "CoreMinimal.h"
#include "VehicleDataRepl.generated.h"

// This struct is used for replicating custom vehicles via RPC's or as variables, since the maximum size for byte arrays is hardcoded to 65k
USTRUCT()
struct FVehicleDataRepl
{
	GENERATED_BODY()
	;

public:
	FVehicleDataRepl()
	{
	}

	FVehicleDataRepl(const TArray<uint8>& InData)
	{
		*this = InData;
	}

	FORCEINLINE bool IsEmpty() const
	{
		return Data.Num() == 0;
	}

	FORCEINLINE const TArray<uint8>& GetData() const
	{
		return Data;
	}

	FORCEINLINE void operator=(const TArray<uint8>& InData)
	{
		Data = InData;
		Version++;
	}

	FORCEINLINE bool operator==(const FVehicleDataRepl& Other) const
	{
		return Version == Other.Version;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Data;
		return true;
	}

private:
	TArray<uint8> Data;
	// Version index, used for faster comparisons
	uint8 Version;
};

template <>
struct TStructOpsTypeTraits<FVehicleDataRepl> : public TStructOpsTypeTraitsBase2<FVehicleDataRepl>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
