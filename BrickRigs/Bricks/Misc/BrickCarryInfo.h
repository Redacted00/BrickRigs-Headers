#pragma once

#include "Player/PawnRestartTransform.h"
#include "CoreMinimal.h"
#include "BrickCarryInfo.generated.h"

class UBrick;

// This is used to replicate carrying of bricks by characters
USTRUCT()
struct FBrickCarryInfo
{
	GENERATED_BODY()

public:
	// ~Variables
	TWeakObjectPtr<UBrick> Brick;
	FRestartTransformVersion RestartTransformVersion;
	FVector_NetQuantize100 DropLocation;
	FRotator DropRotation;
	FVector_NetQuantize100 DropVelocity;
	// ~Variables


	bool operator==(const FBrickCarryInfo& Other) const
	{
		return Brick == Other.Brick && RestartTransformVersion == Other.RestartTransformVersion && DropLocation == Other.DropLocation && DropRotation == Other.DropRotation && DropVelocity == Other.DropVelocity;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		RestartTransformVersion.NetSerialize(Ar, Map, bOutSuccess);

		auto bHasBrick = Brick.IsValid();
		Ar << bHasBrick;

		if (bHasBrick)
		{
			Ar << Brick;

			if (Ar.IsLoading())
			{
				DropLocation = FVector::ZeroVector;
				DropRotation = FRotator::ZeroRotator;
				DropVelocity = FVector::ZeroVector;
			}
		}
		else
		{
			DropLocation.NetSerialize(Ar, Map, bOutSuccess);
			DropRotation.SerializeCompressed(Ar);
			DropVelocity.NetSerialize(Ar, Map, bOutSuccess);

			if (Ar.IsLoading())
			{
				Brick.Reset();
			}
		}

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FBrickCarryInfo> : public TStructOpsTypeTraitsBase2<FBrickCarryInfo>
{
	enum
	{
		// IMPORTANT: Can't use WithNetSharedSerialization because of the brick reference
		WithNetSerializer = true,
		WithIdenticalViaEquality = true,
	};
};
