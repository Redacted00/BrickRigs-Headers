// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "BrickEditor/BrickEditorObjectID.h"
#include "RepVehicleMovement.generated.h"

class ABrickVehicle;

USTRUCT()
struct FRepBrickMovementState
{
	GENERATED_BODY()

	// ~Variables
	FBrickEditorObjectID BrickID;
	FVector_NetQuantize100 Location;
	FQuat Rotation;
	FVector_NetQuantize100 LinearVelocity;
	FVector_NetQuantize100 AngularVelocity;
	// ~Variables

	friend bool operator==(const FRepBrickMovementState& Lhs, const FRepBrickMovementState& Rhs)
	{
		return Lhs.BrickID == Rhs.BrickID
			&& Lhs.Location == Rhs.Location
			&& Lhs.Rotation == Rhs.Rotation
			&& Lhs.LinearVelocity == Rhs.LinearVelocity
			&& Lhs.AngularVelocity == Rhs.AngularVelocity;
	}

	friend bool operator!=(const FRepBrickMovementState& Lhs, const FRepBrickMovementState& Rhs)
	{
		return !(Lhs == Rhs);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		BrickID.NetSerialize(Ar, Map, bOutSuccess);
		Location.NetSerialize(Ar, Map, bOutSuccess);
		Rotation.NetSerialize(Ar, Map, bOutSuccess);
		LinearVelocity.NetSerialize(Ar, Map, bOutSuccess);
		AngularVelocity.NetSerialize(Ar, Map, bOutSuccess);

		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickMovementState> : TStructOpsTypeTraitsBase2<FRepBrickMovementState>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

USTRUCT()
struct FRepBrickMovement : public FFastArraySerializerItem
{
	GENERATED_BODY()

	// ~Variables
	FRepBrickMovementState State;
	// Owner time seconds when this movement was captured
	float OwnerTimestamp = 0.f;
	// ~Variables

	// ~Constructor
	FRepBrickMovement()
	{
	}

	// ~Constructor
	FRepBrickMovement(const FRepBrickMovementState& State, const float OwnerTimestamp)
		: State(State), OwnerTimestamp(OwnerTimestamp)
	{
	}

	bool operator==(const FRepBrickMovement& Other) const
	{
		return OwnerTimestamp == Other.OwnerTimestamp && State.BrickID == Other.State.BrickID;
	}

	bool operator!=(const FRepBrickMovement& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		State.NetSerialize(Ar, Map, bOutSuccess);
		Ar << OwnerTimestamp;

		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FRepBrickMovement> : TStructOpsTypeTraitsBase2<FRepBrickMovement>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

USTRUCT()
struct FRepVehicleMovement : public FFastArraySerializer
{
	GENERATED_BODY()

	// ~Variables
	TWeakObjectPtr<ABrickVehicle> Vehicle;
	UPROPERTY()
	TArray<FRepBrickMovement> Items;
	// ~Variables

	// ~Constructor
	FRepVehicleMovement(ABrickVehicle* Vehicle = nullptr);

	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FRepBrickMovement, FRepVehicleMovement>(Items, DeltaParams, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FRepVehicleMovement> : TStructOpsTypeTraitsBase2<FRepVehicleMovement>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

// Stores the root of a movement replication cluster together with all connected parts
struct FVehicleRepMoveCluster
{
	FBrickEditorObjectID RootBrick;
	TArray<FBrickEditorObjectID> ChildRootBricks;
};
