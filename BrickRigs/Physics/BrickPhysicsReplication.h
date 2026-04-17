#pragma once

#include "CoreMinimal.h"
#include "Physics/PhysicsInterfaceUtils.h"
#include "PhysicsReplication.h"
#include "BrickPhysicsReplication.generated.h"

class ABrickVehicle;

/**
 * This interface has to be implemented by any actor that wants to use physics replication
 */
UINTERFACE()
class BRICKRIGS_API UPhysicsReplicationInterface : public UInterface
{
	GENERATED_BODY()
};

class IPhysicsReplicationInterface
{
	GENERATED_IINTERFACE_BODY()
	struct FSetPhysicsReplicationStateParams
	{
		float DeltaTime;
		const TArray<FRepBoneMovement>& BoneMovements;
		float ExtrapolationTime;
		bool bStopOnTarget;
	};

	// Used to update the physics state from replication
	virtual void SetPhysicsReplicationState(const FSetPhysicsReplicationStateParams& Params) = 0;

	// Applies the physics replication state to a component
	static void SetComponentPhysicsReplicationState(UPrimitiveComponent* Component, const FTransform& ChildTransform, const FRepBoneMovement& Movement, const FSetPhysicsReplicationStateParams& Params);
};

// Replicates movement of a single sub item
USTRUCT()
struct FRepBoneMovement
{
	GENERATED_BODY()

public:
	// ~Variables
	// A uint16 should be good enough for bones as well as bricks
	uint16 BoneIndex;
	FVector_NetQuantize100 Location;
	FQuat Rotation;
	FVector_NetQuantize LinearVelocity;
	FVector_NetQuantize100 AngularVelocity;
	// ~Variables

	// ~Constructor
	FRepBoneMovement(uint16 InBoneIndex = 0)
		: BoneIndex(InBoneIndex), Location(FVector::ZeroVector), Rotation(FQuat::Identity), LinearVelocity(FVector::ZeroVector), AngularVelocity(FVector::ZeroVector)
	{
	}

	bool operator==(const FRepBoneMovement& Other) const
	{
		return BoneIndex == Other.BoneIndex && Location == Other.Location && Rotation == Other.Rotation && LinearVelocity == Other.LinearVelocity && AngularVelocity == Other.AngularVelocity;
	}

	bool operator!=(const FRepBoneMovement& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		Ar << BoneIndex;
		Location.NetSerialize(Ar, Map, bSuccess);
		Rotation.NetSerialize(Ar, Map, bSuccess);
		LinearVelocity.NetSerialize(Ar, Map, bSuccess);
		AngularVelocity.NetSerialize(Ar, Map, bSuccess);
		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FRepBoneMovement> : public TStructOpsTypeTraitsBase2<FRepBoneMovement>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

// Replicates movement of an entire actor
USTRUCT()
struct FRepPhysicsMovement
{
	GENERATED_BODY()

	// ~Variables
	// Owner reatime seconds when this movement was captured
	float OwnerTimestamp;
	TArray<FRepBoneMovement> BoneMovements;
	// ~Variables

	// ~Constructor
	FRepPhysicsMovement(float InOwnerTimestamp = 0.f)
		: OwnerTimestamp(InOwnerTimestamp)
	{
	}

	bool operator==(const FRepPhysicsMovement& Other) const
	{
		// Don't compare the entire array, too slow
		return OwnerTimestamp == Other.OwnerTimestamp;
	}

	void UpdateTimestamp(const UWorld* World);

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		Ar << OwnerTimestamp;

		auto NumBones = static_cast<uint16>(BoneMovements.Num());
		Ar << NumBones;

		// Resize the array when loading
		if (Ar.IsLoading())
		{
			BoneMovements.SetNumUninitialized(NumBones);
		}

		// Serialize the entries
		for (auto i = 0u; i < NumBones; ++i)
		{
			BoneMovements[i].NetSerialize(Ar, Map, bSuccess);
		}

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FRepPhysicsMovement> : public TStructOpsTypeTraitsBase2<FRepPhysicsMovement>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

struct FBrickReplicatedPhysicsTarget
{
	// The player state currently controlling the actor, used to detect owner changes
	TWeakObjectPtr<APlayerState> PlayerState;
	// The last replicated movement
	FRepPhysicsMovement RepMovement;
	// Whether interpolation should be stopped after reaching the last state
	bool bStopOnLastBuffer = false;
};

class FBrickPhysicsReplicationFactory : public IPhysicsReplicationFactory
{
public:
	virtual ~FBrickPhysicsReplicationFactory() = default;

private:
	// ~Super Interface
	virtual FPhysicsReplication* Create(FPhysScene* OwningPhysScene) override;
	virtual void Destroy(FPhysicsReplication* PhysicsReplication) override;
	// ~Super Interface
};

struct FBrickPhysicsReplication : FPhysicsReplication
{
private:
	// ~Variables
	TMap<TWeakObjectPtr<AActor>, FBrickReplicatedPhysicsTarget> ActorsToTargets;
	// ~Variables

public:
	// ~Constructor
	FBrickPhysicsReplication(FPhysScene* InPhysScene);

	// ~Super Interface
	virtual void OnTick(float DeltaSeconds, TMap<TWeakObjectPtr<UPrimitiveComponent>, FReplicatedPhysicsTarget>& ComponentsToTargets) override;
	// ~Super Interface

	// Allows an actor to add replicated movement upon receiving it
	void AddReplicatedActorTarget(AActor* Actor, APlayerState* PlayerState, const FRepPhysicsMovement& ReplicatedTarget, bool bTeleported, bool bStopOnTarget);
	// Removes the actor from physics replication
	void RemoveReplicatedActorTarget(AActor* Actor);

private:
	// Used to update the movement of a single actor
	bool ApplyActorTarget(float DeltaTime, float ServerTime, AActor* Actor, FBrickReplicatedPhysicsTarget& Target);
};
