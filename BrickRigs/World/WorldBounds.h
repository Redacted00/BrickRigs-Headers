// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldBounds.generated.h"

class UStaticMeshComponent;
class UInstancedStaticMeshComponent;

UCLASS(Abstract)
class BRICKRIGS_API AWorldBounds : public AActor
{
	GENERATED_BODY()

protected:
	// ~Components
	UPROPERTY(Transient)
	UInstancedStaticMeshComponent* BoundsMeshComponent;
	UPROPERTY(Transient)
	UStaticMeshComponent* WaterMeshComponent;
	// ~Components

public:
	// ~Properties
	// Mesh to use for the instances
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = WorldBounds)
	UStaticMesh* StaticMesh;
	// Mesh to instantiate for the water
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBounds)
	UStaticMesh* WaterMesh;

	// Optional material override
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBounds)
	UMaterialInterface* MaterialOverride;
	// Optional water material override
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBounds)
	UMaterialInterface* WaterMaterialOverride;

	// Size of the bounds in all directions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBounds)
	FVector BoundsSize;
	// Size of the hole to cut out for the landscape
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldBounds)
	FVector2D LandscapeSize;
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	FVector2D HoleSize_DEPRECATED;
	// Can be used to automatically Center the bounds around and (e.g. for the landscape)
	UPROPERTY(EditAnywhere, Category = WorldBounds)
	TSoftObjectPtr<AActor> BoundsAlignActor;
	// Set to true to automatically update the bounds from the assigned actor
	UPROPERTY(EditAnywhere, Category = WorldBounds)
	bool bSnapBounds;
#endif
	// Optionally creates water if this is > 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Water, Meta = (ClampMin = 0))
	float WaterDepth;
	// ~Properties

	// ~Constructor
	AWorldBounds();

	// ~Super Interface
	virtual void PostLoad() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreRegisterAllComponents() override;
	// ~Super Interface
};
