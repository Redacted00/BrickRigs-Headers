#pragma once

#include "Engine/EngineTypes.h"
#include "Engine/NetSerialization.h"
#include "CoreMinimal.h"
#include "PawnRestartTransform.generated.h"

USTRUCT(BlueprintType)
struct FRestartTransformVersion
{
	GENERATED_BODY()

private:
	// ~Variables
	uint8 Version;
	// ~Variables

public:
	// ~Constructor
	FRestartTransformVersion()
		: Version(0)
	{
	}

	void Increment()
	{
		Version++;
	}

	bool operator==(const FRestartTransformVersion& Other) const
	{
		return Version == Other.Version;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Version;
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FRestartTransformVersion> : TStructOpsTypeTraitsBase2<FRestartTransformVersion>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true,
	};
};

/**
* This struct is used to replicate the teleportation location and rotation after restarting
*/
USTRUCT(BlueprintType)
struct FPawnRestartTransform
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Comparison index for the current location
	// NOTE: This is important since the restart transform might be identical, making it not replicate again
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRestartTransformVersion ComparisonIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector_NetQuantize100 Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;
	// ~Variables

public:
	// ~Constructor
	FPawnRestartTransform()
	{
	}

	// ~Constructor
	FPawnRestartTransform(const UWorld* World, const FVector& InLocation, const FRotator& InRotation)
		: FPawnRestartTransform()
	{
		Set(World, InLocation, InRotation);
	}

	void Set(const UWorld* World, const FVector& InLocation, const FRotator& InRotation)
	{
		Location = FRepMovement::RebaseOntoZeroOrigin(InLocation, World->OriginLocation);
		Rotation = InRotation;
		// Update the comparison index
		ComparisonIndex.Increment();
	}

	FVector GetLocation(const UWorld* World) const
	{
		return FRepMovement::RebaseOntoLocalOrigin(Location, World->OriginLocation);
	}

	const auto& GetRotation() const
	{
		return Rotation;
	}

	const FRestartTransformVersion& GetComparisonIndex() const
	{
		return ComparisonIndex;
	}

	bool operator==(const FPawnRestartTransform& Other) const
	{
		return ComparisonIndex == Other.ComparisonIndex && Location == Other.Location && Rotation == Other.Rotation;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		ComparisonIndex.NetSerialize(Ar, Map, bOutSuccess);
		Location.NetSerialize(Ar, Map, bOutSuccess);
		Rotation.SerializeCompressedShort(Ar);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FPawnRestartTransform> : public TStructOpsTypeTraitsBase2<FPawnRestartTransform>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true,
	};
};
