// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fence.generated.h"

class UDestructibleInstanceTemplate;
class UDestructibleInstancesComponent;
class USplineComponent;

// Fence element assets are used to determine what sort objects are placed by a fence
UCLASS(BlueprintType)
class BRICKRIGS_API UFenceElement : public UDataAsset
{
	GENERATED_BODY()

public:
	// ~Properties
	// The mesh to place on the point/along the segment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	UDestructibleInstanceTemplate* Template;
	// Axis to align the meshes along
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	TEnumAsByte<EAxis::Type> ForwardAxis;
	// Describes the amount of space needed to either side
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	float Spacing;
	// The max location jitter in any direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	FVector LocationJitter;
	// The max rotation jitter in any direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	FRotator RotationJitter;
	// The max randomized scale in any direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	FVector ScaleJitter;
	// The min and max scale used to fit instances onto a segment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fence)
	FVector2D StretchRange;
	// ~Properties

	// ~Constructor
	UFenceElement()
		: Template(nullptr), Spacing(100.f), LocationJitter(10.f, 10.f, 0.f), RotationJitter(5.f, 5.f, 5.f), ScaleJitter(0.05f), StretchRange(0.9f, 1.1f)
	{
	}
};

// One fence manager actor has to be placed in the level to handle the instances
UCLASS(HideCategories = ("Actor Tick", "Rendering", "Replication", "Input", "Actor", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API AFenceManager : public AActor
{
	GENERATED_BODY()

public:
	// ~Components
	UPROPERTY()
	UDestructibleInstancesComponent* InstancesComponent;
	// ~Components

	// ~Constructor
	AFenceManager();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Reset() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	// ~Super Interface

	// Builds all the fences
	void BuildFence();
};

UCLASS(HideCategories = ("Actor Tick", "Rendering", "Replication", "Input", "Actor", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API AFence : public AActor
{
	GENERATED_BODY()

protected:
	// ~Components
#if WITH_EDITORONLY_DATA
	// The spline used to place the fence instances along
	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Fence)
	UFenceElement* FenceElementType;
	UPROPERTY(EditAnywhere, Category = Fence)
	FRandomStream FenceRandStream;
#endif
	// ~Properties

	// ~Constructor
	AFence();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Destroyed() override;
	// ~Super Interface

	// Adds the fence elements to the manager
	void BuildFence(AFenceManager* Manager);
};
