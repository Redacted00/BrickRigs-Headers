// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditor/BrickEditorInterfaceComponent.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "BrickVehicleComponent.generated.h"

class ABrickVehicle;
class UBrick;
class USeatBrick;
struct FBrickTickFunction;

// This component is used to share the basic vehicle structure functionality between the editor and vehicles
UCLASS()
class BRICKRIGS_API UBrickVehicleComponent : public UBrickEditorInterfaceComponent
{
	GENERATED_BODY()

	// ~Variables
	// List of registered tick functions
	TArray<FBrickTickFunction*> BrickTickFunctions;
	// The seat that is assigned as the driver seat
	TBrickEditorObjectPtr<USeatBrick> DriverSeat;

public:
	// Used to resolve deprecated seat channels during load
	TMap<uint8, USeatBrick*> LoadSeatChannelMap;
	// Used to resolve deprecated input channel indices during load
	TMap<uint8, TArray<UBrick*>> LoadInputChannelIndexMap;
	// ~Variables

	// ~Constructor
	UBrickVehicleComponent();

	// ~Super Interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual EUGCType GetUGCType() const override;
	virtual void PostClearItem() override;
	virtual bool SaveToArchive(FArchive& Ar) override;
	virtual bool PostLoadFromArchive(FArchive& Ar, const FBrickEditorSaveHeader& Header, bool bImport, const FBrickEditorReferenceResolver& ReferenceResolver) override;
	virtual FString ResolveDeprecatedBrickEditorObjectClass(const FString& InClassName) override;
	virtual void PostLoadBrickEditorObject(const TArray<UBrickEditorObject*>& NewObjects, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void PreInitializeBrickEditorObjects() override;
	virtual void PostInitializeBrickEditorObjects() override;
	virtual const TArray<UClass*>& GetBrickEditorObjectLibrary() const override;
	virtual int32 GetNumBrickEditorObjectsWithAerodynamics() const override;
	// ~Super Interface

	// ~Vehicles
	// Get the owning vehicle, can be null
	ABrickVehicle* GetVehicle() const;
	// ~Vehicles

	// ~Bricks
	// Get the brick assigned as the driver seat
	USeatBrick* GetDriverSeat() const
	{
		return DriverSeat.Get();
	}

	// Returns a random seat to use as the driver seat
	USeatBrick* GetDefaultDriverSeat() const;

	// Get the ID of the driver seat
	const FBrickEditorObjectID& GetDriverSeatID() const
	{
		return DriverSeat.GetID();
	}

	// Assigns a new driver seat, returns true if the seat has been set
	bool SetDriverSeat(USeatBrick* InSeat);
	// ~Bricks

	// ~Ticking
	// Register a tick function with the vehicle
	void RegisterBrickTickFunction(FBrickTickFunction* TickFunction);
	// Unregister an existing tick function
	void UnregisterBrickTickFunction(FBrickTickFunction* TickFunction);
	// ~Ticking
};
