#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/World.h"
#include "Engine/NetSerialization.h"
#include "CoreMinimal.h"
#include "PlayerViewPoint.generated.h"

/**
* This struct is used to efficiently and accurately replicate the client view point to the server
*/
USTRUCT()
struct FPlayerViewPoint
{
	GENERATED_BODY()
	;

private:
	FVector_NetQuantize100 Location;
	FRotator Rotation;

public:
	FPlayerViewPoint()
	{
	}

	FPlayerViewPoint(UWorld* World, const FVector& InLocation, const FRotator& InRotation)
	{
		Location = FRepMovement::RebaseOntoZeroOrigin(InLocation, World->OriginLocation);
		Rotation = InRotation;
	}

	void Get(UWorld* World, FVector& OutLocation, FRotator& OutRotation) const
	{
		OutLocation = FRepMovement::RebaseOntoLocalOrigin(Location, World->OriginLocation);
		OutRotation = Rotation;
	}

	FORCEINLINE bool operator==(const FPlayerViewPoint& Other) const
	{
		return Location == Other.Location && Rotation == Other.Rotation;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Location.NetSerialize(Ar, Map, bOutSuccess);
		Rotation.SerializeCompressedShort(Ar);

		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FPlayerViewPoint> : public TStructOpsTypeTraitsBase2<FPlayerViewPoint>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true,
	};
};
