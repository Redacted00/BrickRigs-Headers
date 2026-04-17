// Copyright Fluppisoft, 2015

#pragma once

#include "Components/DestructibleInstancesComponent.h"
#include "BrickBuilding.h"
#include "LegacyBuilding.generated.h"

UENUM(BlueprintType)
enum class EBuildingSide : uint8
{
	Left,
	Right,
	Front,
	Back,
};

USTRUCT()
struct FBuildingMesh
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Material)
	UDestructibleInstanceTemplate* Template = nullptr;
};

USTRUCT()
struct FBuildingMaterialOverride
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Material)
	UMaterialInterface* FromMaterial = nullptr;
	UPROPERTY(EditAnywhere, Category = Material)
	UMaterialInterface* ToMaterial = nullptr;
};

// Buildings parts are used during construction of buildings
UCLASS()
class BRICKRIGS_API UBuildingPart : public UObject
{
	GENERATED_BODY()

public:
	// ~Variables
	FVector Location;
	FIntPoint Num;
	int32 NumFloors;
	TArray<int8> Meshes;
	// ~Variables

	void InitBuildingPart(int32 NumX, int32 NumY, int32 InNumFloors, const FVector& InLocation);

	int32 GetFloorOffset() const
	{
		return Num.X * 2 + Num.Y * 2;
	}

	UFUNCTION(BlueprintCallable, Category = Building)
	void Fill(uint8 MeshIdx);
	UFUNCTION(BlueprintCallable, Category = Building)
	void Grid(int32 Dist, uint8 MeshIdx);
	UFUNCTION(BlueprintCallable, Category = Building)
	void Place(EBuildingSide Side, int32 Floor, int32 Position, uint8 MeshIdx);
	UFUNCTION(BlueprintCallable, Category = Building)
	void Rect(EBuildingSide Side, int32 StartFloor, int32 EndFloor, int32 StartPosition, int32 EndPosition, uint8 MeshIdx);
	UFUNCTION(BlueprintCallable, Category = Building)
	void Floor(int32 Floor, uint8 MeshIdx);
};

UCLASS(Abstract)
class BRICKRIGS_API ALegacyBuilding : public ABrickBuilding
{
	GENERATED_BODY()

private:
	// ~Variables
	// These are used to cache the material overrides while adding instances
	TArray<TArray<UMaterialInterface*>> PerMeshMaterialOverrides;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UDestructibleInstancesComponent* DestructibleInstancesComponent;
	// ~Components

	// ~Properties
	UPROPERTY(EditAnywhere, Category = Building)
	TArray<FBuildingMesh> BuildingMeshes;
	UPROPERTY(EditAnywhere, Category = Building)
	TArray<FBuildingMaterialOverride> MaterialOverrides;
	// ~Properties

public:
	// ~Constructor
	ALegacyBuilding();

	// ~Super Interface
	virtual void RerunConstructionScripts() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void Reset() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual float TakeDamage(float DamageAmount, const struct FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual float TakeDamageInternal(float DamageAmount, const struct FDamageEvent& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// ~Super Interface

protected:
	// Building geometry generator
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 NumWallsX;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 NumWallsY;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Building)
	int32 NumFloors;

	// Constructs a new building part, which is used to easily place walls, windows, doors etc.
	UFUNCTION(BlueprintCallable, Category = Building)
	UBuildingPart* ConstructBuildingPart(int32 NumX, int32 NumY, int32 Floors, const FVector Location = FVector::ZeroVector);
	UFUNCTION(BlueprintCallable, Category = Building)
	void SpawnBuildingPart(UBuildingPart* Part);

	// Constructs a simple sloped root
	UFUNCTION(BlueprintCallable, Category = Building)
	void ConstructRoof(UBuildingPart* Part, uint8 RoofGableMesh, uint8 RoofMesh, bool bRotate = false);
	// Constructs a simple flat root
	UFUNCTION(BlueprintCallable, Category = Building)
	void ConstructFlatRoof(UBuildingPart* Part, uint8 Mesh);
	// Constructs an industrial style zig-zag roof /|/|/|
	UFUNCTION(BlueprintCallable, Category = Building)
	void ConstructIndustrialRoof(UBuildingPart* Part, uint8 RoofSlopeMesh, uint8 RoofMesh, uint8 WallMesh);

private:
	// Adds a single wall instance
	void AddWall(float X, float Y, float Z, const FRotator& Rotation, uint8 MeshIdx);
	// Adds an instance to the building
	void AddMeshInstance(uint8 MeshIdx, const FTransform& Transform);
};
