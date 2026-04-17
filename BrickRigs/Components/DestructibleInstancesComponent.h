// Copyright Fluppisoft, 2020

#pragma once

#include "Physics/ContactModifyInterface.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Misc/DynamicBitfield.h"
#include "DestructibleInstancesComponent.generated.h"

class UDestructibleInstancesComponent;
class UInstancedStaticMeshComponent;

UCLASS(BlueprintType)
class BRICKRIGS_API UDestructibleInstanceTemplate : public UDataAsset
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Defaults)
	UStaticMesh* Mesh = nullptr;
	// The mesh displayed when the instance has been broken
	UPROPERTY(EditAnywhere, Category = Defaults)
	UStaticMesh* BrokenMesh = nullptr;
	// Whether the broken mesh should simulate physics
	UPROPERTY(EditAnywhere, Category = Defaults)
	bool bSimulateBrokenMesh = false;
	// The maximum randomized linear speed
	UPROPERTY(EditAnywhere, Category = Defaults)
	float BrokenMeshMaxLinearSpeed = 300.f;
	// The maximum randomized angular speed in degrees/s
	UPROPERTY(EditAnywhere, Category = Defaults)
	float BrokenMeshMaxAngularSpeed = 180.f;
	// The damage needed to break the instance, < 0 means it can't be broken
	UPROPERTY(EditAnywhere, Category = Defaults)
	float MinDamage = -1.f;
	// Max render distance for the instances
	UPROPERTY(EditAnywhere, Category = Defaults)
	float MaxDrawDistance = 100000.f;
	// Max distance at which the emitter will be spawned
	UPROPERTY(EditAnywhere, Category = Defaults)
	float MaxBreakEmitterSpawnDistance = 50000.f;
	// Emitter spawned when the instance has been broken
	UPROPERTY(EditAnywhere, Category = Defaults)
	UParticleSystem* BreakEmitter = nullptr;
	// Sound spawned when the instance has been broken
	UPROPERTY(EditAnywhere, Category = Defaults)
	USoundBase* BreakSound = nullptr;
	// ~Properties
};

// These structs are used to replicate the per instance damage
USTRUCT()
struct FDestructibleInstanceDamageItem : public FFastArraySerializerItem
{
	GENERATED_BODY()

	using FElementType = bool;
	using FChunkType = TDynamicBitfieldChunk<FElementType, uint32, 1, 1>;

	// ~Variables
	FChunkType Chunk;
	FChunkType ComparisonChunk;
	// ~Variables

	bool operator==(const FDestructibleInstanceDamageItem& Other) const
	{
		return Chunk == Other.Chunk;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		return Chunk.NetSerialize(Ar, Map, bOutSuccess);
	}
};

template <>
struct TStructOpsTypeTraits<FDestructibleInstanceDamageItem> : TStructOpsTypeTraitsBase2<FDestructibleInstanceDamageItem>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

USTRUCT()
struct FDestructibleInstanceDamageArray : public FFastArraySerializer
{
	GENERATED_BODY()

	using FChunkType = FDestructibleInstanceDamageItem::FChunkType;

private:
	// ~Variables
	TWeakObjectPtr<UDestructibleInstancesComponent> Component;

public:
	// The damage chunks, note that this has to be named Items for the fast array serializer
	UPROPERTY()
	TArray<FDestructibleInstanceDamageItem> Items;
	// ~Variables

	// ~Constructor
	explicit FDestructibleInstanceDamageArray(UDestructibleInstancesComponent* Component = nullptr)
		: Component(Component)
	{
	}

	IMPLEMENT_DYNAMIC_BITFIELD_FAST_ARRAY(FChunkType, FChunkType::FElementType, InstanceDestroyed);

	bool ShouldCallOnReplicatedChange(const bool bRemovingItems) const;
	void OnReplicatedChange(const TArrayView<int32>& ChangedIndices, const bool bRemovingItems);

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FastArrayDeltaSerialize<FDestructibleInstanceDamageItem, FDestructibleInstanceDamageArray>(Items, DeltaParams, *this);
	}
};

template <>
struct TStructOpsTypeTraits<FDestructibleInstanceDamageArray> : TStructOpsTypeTraitsBase2<FDestructibleInstanceDamageArray>
{
	enum
	{
		WithNetDeltaSerializer = true
	};
};

USTRUCT()
struct FDestructibleInstance
{
	GENERATED_BODY()

	// The desired transform of the instance
	UPROPERTY()
	FTransform InstanceTransform;
#if WITH_EDITORONLY_DATA
	// The owner actor of this instance, useful to associate instances with fence actors for example
	TSoftObjectPtr<AActor> InstanceOwner;
#endif
	// Whether the instance is broken
	uint8 bIsBroken : 1;
	// The accumulated damage
	float Damage;

	// ~Constructor
	FDestructibleInstance(const FTransform& InTransform = FTransform::Identity)
		: InstanceTransform(InTransform), bIsBroken(false), Damage(0.f)
	{
	}
};

USTRUCT()
struct FDestructibleInstanceArray
{
	GENERATED_BODY()

	// The template used for the instances
	UPROPERTY()
	UDestructibleInstanceTemplate* Template;
	// Replaces materials to use for the mesh and broken mesh
	UPROPERTY()
	TArray<UMaterialInterface*> MaterialOverrides;
	// The component used for the normal instances
	TWeakObjectPtr<UInstancedStaticMeshComponent> ISMComponent;
	// The component used for the broken instances
	TWeakObjectPtr<UInstancedStaticMeshComponent> BrokenISMComponent;
	// Contains info about each instance, needs to be serialized for the transforms
	UPROPERTY()
	TArray<FDestructibleInstance> Instances;

	// ~Constructor
	FDestructibleInstanceArray(UDestructibleInstanceTemplate* InTemplate = nullptr, const TArray<UMaterialInterface*>& InMaterialOverrides = {})
		: Template(InTemplate), MaterialOverrides(InMaterialOverrides)
	{
	}
};

// The destructible instances component can be used by buildings and fences for, it handles spawning them, damage etc.
UCLASS(HideCategories = (Activation, Cooking, "Asset User Data", Collision, Tags))
class BRICKRIGS_API UDestructibleInstancesComponent : public USceneComponent
{
	GENERATED_BODY()

	friend FDestructibleInstanceDamageArray;

	// ~Variables
	// Stores cached and runtime information about the instances
	UPROPERTY()
	TArray<FDestructibleInstanceArray> InstanceArrays;

	// Used to replicate per instance damage to all players
	UPROPERTY(Transient, Replicated)
	FDestructibleInstanceDamageArray ReplicatedDamage;
	// The unique replication key, used to avoid unnecessary replication updates
	int32 ReplicationKey = 0;
	// Used to revert damage to the replicated state
	FTimerHandle TimerHandle_RevertDamage;

	// List of broken mesh components that were created
	TArray<TWeakObjectPtr<UStaticMeshComponent>> BrokenMeshComponents;

	// The current max damage amount received
	float MaxDamageReceived = 0.f;
	// ~Variables

public:
	// ~Properties
	// If true hierarchical ISM components will be used, otherwise normal ones
	UPROPERTY(EditAnywhere, Category = DestructibleInstances)
	bool bUseHierarchicalISM = false;
	// Whether to use full float precision on the ISM components
	UPROPERTY(EditAnywhere, Category = DestructibleInstances)
	bool bForceFullFloatPrecision = false;
	// ~Properties

	// ~Constructor
	UDestructibleInstancesComponent()
		: ReplicatedDamage(this)
	{
	}

	// ~Super Interface
	virtual void OnRegister() override;
	// ~Super Interface

	// Get the current replication key for ReplicateSubobjects
	int32 GetReplicationKey() const
	{
		return ReplicationKey;
	}

	// Clears all instance data, optionally only for a specific owner
	void ClearInstances(AActor* Owner = nullptr);
	// Adds a new instance
	void AddInstance(UDestructibleInstanceTemplate* Template, const TArray<UMaterialInterface*>& MaterialOverrides, const FTransform& InstanceTransform, AActor* Owner = nullptr);

	// Repairs all instances, server only
	void ResetInstances();
	// Spawns or repairs all instances
	void InitializeInstances();

protected:
	// Used to destroy or repair a set of instances
	void UpdateInstances(const TMap<int32, TArray<int32>>& InstancesToUpdate, const FRadialDamageEvent* RadialDamageEvent, bool bDestroy, bool bSpawnEffects);
	// Instantly removes all broken mesh components that have been created
	void RemoveBrokenMeshComponents();

public:
	// Converts an original instance index to a replicated one
	int32 OriginalToReplicatedInstanceIndex(int32 ArrayIndex, int32 InstanceIndex) const;
	// Converts a replicated instance index to a original index and component
	void ReplicatedToOriginalInstanceIndex(int32 InstanceIndex, int32& OutOriginalInstanceIndex, int32& OutArrayIndex) const;
	// Get the instance array associated with an ISM component
	int32 FindInstanceArrayFromComponent(const UInstancedStaticMeshComponent* ISMComp, bool& bOutIsBrokenISM) const;

	// Called by the revert damage timer
	void RevertDamage();

	// Events called from the ISM components
	TMap<int32, TArray<int32>> OnISMCompReceivedDamage(const UInstancedStaticMeshComponent* ISMComp, const FRadialDamageEvent* RadialDamageEvent, const TMap<int32, float>& PerInstanceDamage, bool bAccumulateDamage);
	void OnISMCompContactModify(const UInstancedStaticMeshComponent* ISMComp, const FContactModifyParams& Params, int32 ActorIdx) const;
	void OnISMCompPostContactModify(const UInstancedStaticMeshComponent* ISMComp, const FPostContactModifyParams& Params, int32 ActorIdx);
	// Called from the vehicle server RPC
	void OnClientBrokeInstances(const TMap<int32, TArray<int32>>& Instances);

	// Resets the max damage meter
	void ResetMaxDamageReceived()
	{
		MaxDamageReceived = 0.f;
	}

	// Public accessor
	float GetMaxDamageReceived() const
	{
		return MaxDamageReceived;
	}
};
