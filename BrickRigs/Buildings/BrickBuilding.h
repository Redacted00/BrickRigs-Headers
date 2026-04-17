// Copyright Fluppisoft, 2018

#pragma once

#include "BrickRigsMacros.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickBuilding.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;
class UAudioComponent;
class UParticleSystemComponent;

// Actor spawned to display the debris and collapse effects for a building
UCLASS(Abstract)
class BRICKRIGS_API ABrickBuildingDebris : public AActor
{
	GENERATED_BODY()

	// ~Variables
	// Whether the building is currently collapsing
	bool bIsCollapsing;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient)
	UInstancedStaticMeshComponent* DebrisMeshComponent;
	UPROPERTY(Transient)
	UParticleSystemComponent* CollapsePSC;
	UPROPERTY(Transient)
	UAudioComponent* CollapseAC;
	// ~Components

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	UStaticMesh* DebrisMesh;
	// Distance between debris meshes
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float DebrisSpacing = 1200.f;
	// Maximum random offset added in every direction relative to the building
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	FVector DebrisLocationJitter = FVector(200.f, 200.f, 20.f);
	// Maximum random rotation about the pitch and roll axis of each pile
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float DebrisRotationJitter = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	USoundBase* CollapseSound;
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	UParticleSystem* CollapseEmitter;
	// ~Properties

public:
	// ~Constructor
	ABrickBuildingDebris();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void Reset() override;
	// ~Super Interface

	// Sets whether the building is currently collapsing
	void SetIsCollapsing(bool bNewCollapsing);
	// Update the debris mesh
	void UpdateDebris(float CollapseRatio);
};

UCLASS(Abstract, HideCategories = ("Actor Tick", Rendering, Replication, Input, LOD))
class BRICKRIGS_API ABrickBuilding : public AActor
{
	GENERATED_BODY()

	friend class ABrickBuildingDebris;

protected:
	// ~Variables
	// Original transform of the building, before it collapsed
	FTransform OriginalRootTransform;
	// The cached bounds of the building
	UPROPERTY()
	FVector BuildingBoundsOrigin;
	UPROPERTY()
	FVector BuildingBoundsExtent;
	// Damage this building has received so far
	float AccumulatedDamage;
	// Accumulated damage direction weighted by the damage amount
	FVector2D AccumulatedDamageDirection;
	// Whether the building has collapsed from the damage sustained
	UPROPERTY(Transient, ReplicatedUsing = OnRep_bCollapsed)
	uint8 bCollapsed : 1;
	UFUNCTION()
	void OnRep_bCollapsed();
	// Axis around which the building collapses
	UPROPERTY(Transient, Replicated)
	uint8 CollapseAxisCompressed;
	// Time when the building has collapsed
	float CollapseTime;
	// Last time collapse damage has been applied
	float LastCollapseDamageTime;

	// Actor that holds the collapse effects
	UPROPERTY(Transient)
	ABrickBuildingDebris* DebrisActor;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UStaticMeshComponent* StaticMeshComponent;
	// ~Components

	// ~Properties
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	UStaticMesh* StaticMesh;
	// List of materials to apply randomly to building instances
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Building)
	TArray<UMaterialInterface*> RandomMaterials;
	// Per building random seed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	FRandomStream RandomStream;
	// Maximum damage before the building collapses
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
	float MaxDamage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
	TSubclassOf<ABrickBuildingDebris> DebrisClass;

public:
#if WITH_EDITORONLY_DATA
	// The layer the foundation of the building should be painted on
	UPROPERTY(EditAnywhere, Category = Landscape)
	FName LandscapeLayerName;
	UPROPERTY(EditAnywhere, Category = Landscape)
	float LandscapeLayerInflation;
#endif
	// ~Properties

	// ~Constructor
	ABrickBuilding();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreRegisterAllComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void Destroyed() override;
	virtual void Tick(float DeltaTime) override;
#if !BR_BUILD_VANILLA
	virtual bool CanBeDamaged() const override;
#endif
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakePointDamage(float Damage, const FPointDamageEvent& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakeRadialDamage(float Damage, const FRadialDamageEvent& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Reset() override;
	// ~Super Interface

	// Returns the transform the building was spawned at
	auto GetOriginalRootTransform() const
	{
		return OriginalRootTransform;
	}

	// Returns the building bounds origin and extent
	void GetBuildingLocalBounds(FVector& OutOrigin, FVector& OutExtent) const
	{
		OutOrigin = BuildingBoundsOrigin;
		OutExtent = BuildingBoundsExtent;
	}

	// Whether the building has collapsed
	bool IsCollapsed() const
	{
		return bCollapsed;
	}

protected:
	// Custom take damage function that can be used to modify the damage
	virtual float TakeDamageInternal(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
	{
		return DamageAmount;
	}

	// Hide or show the building after it has collapsed
	virtual void UpdateBuildingHidden(bool bNewHidden);

private:
	// Adds accumulated damage from the direction of the given impact point
	void AddAccumulatedDamageAtLocation(float Damage, const FVector& Location);
	// Set bCollapsed
	void SetIsCollapsed(bool bNewCollapsed);
	// Called when the collapsed state has changed
	void OnCollapsedChanged(bool bSkipAnimation = false);
	// Get the location and rotation of the building after Time seconds
	void GetCollapseTransform(float Time, FVector& OutLocation, FQuat& OutRotation, float& OutCollapseRatio) const;
};
