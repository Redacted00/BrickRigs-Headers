// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BrickEditor/BrickEditor.h"
#include "VehicleEditor.generated.h"

// Components
class UBrick;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AVehicleEditor : public ABrickEditor
{
	GENERATED_BODY()

	// ~Variables
	// This map is used to store the last edited values of basic brick properties
	TMap<FName, TArray<uint8>> BrickPropertyDefaultsMap;
	// ~Variables

	// ~Components
	// Used to visualize input channels
	UPROPERTY(Transient)
	UInstancedStaticMeshComponent* InputChannelISMComp;
	// ~Components

public:
	// ~Constructor
	AVehicleEditor(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void Tick(float DeltaTime) override;
	virtual const TArray<UClass*>& GetPlacableFilterClasses() const override;
	virtual void GetAttachedObjects(TArray<UBrickEditorObject*>& OutObjects, bool bRecursive) const override;
	virtual FBrickEditorSnappingOffset GetMoveSnappingOffset(const FGetMoveSnappingOffsetParams& Params) const override;
	virtual EBrickValidityState GetMoveObjectValidityState(const FGetMoveObjectValidityStateParams& Params) const override;
	virtual void OnBrickEditorObjectsCreated(const TArray<UBrickEditorObject*>& NewObjects, EBrickEditorObjectCreationContext CreationContext) override;
	virtual void PostModify(const FBrickEditorScopedOperation& Operation) override;
	virtual void OnAnyPropertyModified(const FBrickPropertyChangedEvent& Event) override;
	virtual void OnEditorModeChanged() override;
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	// ~Super Interface

private:
	// ~Selection
	// Get the active object casted to a brick
	UBrick* GetActiveBrick() const
	{
		return GetActiveObject<UBrick>();
	}

	// Helper function to get an array of all selected bricks
	void GetSelectedBricks(TArray<UBrick*>& OutBricks) const
	{
		GetSelectedObjects(OutBricks);
	}

	// Internal version used by GetAttachedObjects
	void GetAttachedObjectsInternal(UBrick* Brick, TArray<UBrick*>& RemainingBricks, TArray<UBrickEditorObject*>& OutObjects, bool bRecursive) const;
	// ~Selection

	void UpdateInputChannelVisualization();
};
