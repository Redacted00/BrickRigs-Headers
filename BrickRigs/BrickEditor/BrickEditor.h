// Copyright Fluppisoft, 2015

#pragma once

#include "Modes/BrickEditorMode.h"
#include "BrickEditorUndoStep.h"
#include "Player/ViewTargetInterface.h"
#include "UGC/UGCTypes.h"
#include "BrickEditorObject.h"
#include "BrickEditorInterfaceComponent.h"
#include "BrickEditorSnappingOffset.h"
#include "BrickEditorScopedOperation.h"
#include "BrickEditorTypes.h"
#include "BrickEditorStaticInfo.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Modes/BrickEditorMoveMode.h"
#include "BrickEditor.generated.h"

// Actors
class ABrickPlayerController;
// Components
class UStaticMeshComponent;
// Assets
class UMaterialInstanceDynamic;
// Misc
class UBrickEditorObject;
struct FBrickPropertyChangedEvent;

UCLASS(Abstract)
class BRICKRIGS_API ABrickEditor : public AActor, public IViewTargetInterface
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorModeChanged, UBrickEditorMode*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFileInfoChanged, const FUGCFileInfo&);
	DECLARE_MULTICAST_DELEGATE(FOnSelectionChanged);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHasUnsavedChangesChanged, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnSaved, bool, bool);

protected:
	const static FName EditorInterfaceComponentName;

	// ~Variables
	// The player controller that is viewing and controlling the editor locally
	UPROPERTY(Transient)
	ABrickPlayerController* PlayerController;
	// Cached values for the zoom feature
	mutable FViewTargetZoomCache ZoomCache;
	mutable FViewTargetCameraSpeedCache CameraSpeedCache;
	// Task handle for querying opened item details
	FFluUGCAsyncTaskHandle UGCTask_QueryDetails;

	// Info about the currently opened file
	FUGCFileInfo FileInfo;
	// Number objects that have aerodynamics enabled
	int32 NumObjectsWithAerodynamics;
	// Current scoped operation
	FBrickEditorScopedOperation* ScopedOperationPtr;
	// All cached undo/redo steps
	TArray<FBrickEditorUndoStep> UndoSteps;
	// Current depth into the undo steps
	int32 UndoNum;
	// The current mode instance
	UPROPERTY(Transient)
	UBrickEditorMode* CurrentEditorMode;
	// List of all editor modes that have been created so far, useful for carrying over settings
	UPROPERTY(Transient)
	TArray<UBrickEditorMode*> EditorModes;

	// Array of currently selected objects
	TArray<TWeakObjectPtr<UBrickEditorObject>> SelectedObjects;
	// The object that was the last active object, useful for reselecting it later on
	TWeakObjectPtr<UBrickEditorObject> LastActiveObject;
	// The object that is currently hovered by the mouse
	TWeakObjectPtr<UBrickEditorObject> HoveredObject;

	// The current camera location
	FVector CameraLocation;
	// Accumulated movement input for the camera
	FVector CameraMovementInput;
	// The gizmo axis that is currently hovered
	FBrickEditorGizmoAxis HoveredGizmoAxis;
	// The optional gizmo axis to highlight
	FBrickEditorGizmoAxis HighlightedGizmoAxis;

	// Whether there are unsaved changes
	uint8 bHasUnsavedChanges : 1;
	// Whether the currently hovered object is pressed
	uint8 bIsHoveredObjectPressed : 1;
	// Whether the camera is currently being moved
	uint8 bIsMovingCamera : 1;
	// Whether the camera is currently in pivot mode
	uint8 bIsPivotingCamera : 1;

	// Material instance used for the outline post process effect
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* OutlineMID;
	// Material instance used for the bounds mesh
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* BoundsMID;
	// The cached UI style reference
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UBrickEditorInterfaceComponent* EditorInterfaceComponent;
	// The mesh component displayed when the player gets close enough to the editor bounds
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* BoundsMeshComponent;
	// Static mesh displayed for the editor grid
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* GridMeshComponent;
	// Static mesh displayed for the transform gizmo
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* GizmoMeshComponent;
	// Displays the center of mass
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* CenterOfMassMeshComponent;
	// ~Components

public:
	// ~Delegates
	FOnEditorModeChanged OnEditorModeChangedDelegate;
	FOnFileInfoChanged OnFileInfoChangedDelegate;
	FOnSelectionChanged OnSelectionChangedDelegate;
	FOnHasUnsavedChangesChanged OnHasUnsavedChangesChangedDelegate;
	FOnSaved OnSavedDelegate;
	// ~Delegates

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSubclassOf<UBrickEditorStaticInfo> StaticInfoClass;
	// ~Properties

public:
	// ~Constructor
	ABrickEditor(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;
	virtual void EnableInput(APlayerController* PC) override;
	virtual void DisableInput(APlayerController* PC) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	// ~Super Interface

	// ~Editor
	// Get the static info CDO
	const UBrickEditorStaticInfo* GetStaticInfo() const;
	// Get the editor interface
	template <typename T = UBrickEditorInterfaceComponent>
	T* GetEditorInterfaceComponent() const
	{
		return Cast<T>(EditorInterfaceComponent);
	}

	// Get the world transform of the editor root
	FTransform GetEditorTransform() const
	{
		return GetActorTransform();
	}

	// Get the player controller controlling this editor
	ABrickPlayerController* GetPlayerController() const
	{
		return PlayerController;
	}

	// Goes to the default mode
	bool EnterDefaultEditorMode(bool bRevertChanges);
	// Try to start a new editor mode
	template <class T = UBrickEditorMode, typename... P>
	T* EnterEditorMode(UClass* ModeClass, bool bRevertChanges, const P&... Params);
	// Commits the changes from the current mode
	bool CommitEditorMode();
	// Cancels the current mode, returns true if successful
	bool CancelEditorMode();
	// Whether the editor is in a mode of the given type
	bool IsInEditorMode(const UClass* ModeClass) const
	{
		const auto* CurrentClass = CurrentEditorMode ? CurrentEditorMode->GetClass() : nullptr;
		return ModeClass == CurrentClass;
	}

	// Templated version
	template <class T>
	bool IsInEditorMode() const
	{
		return IsInEditorMode(T::StaticClass());
	}

	// Get the current mode casted to a specific type
	template <class T = UBrickEditorMode>
	T* GetCurrentEditorMode() const
	{
		return Cast<T>(CurrentEditorMode);
	}

protected:
	// Called when the current mode has changed
	virtual void OnEditorModeChanged();

public:
	// Get the size of the editor usable area
	void GetEditorBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const;
	// Takes a location and makes sure it is within bounds (optionally inflating or deflating it), returns true if it was outside
	bool ConstraintLocationToEditorBounds(FVector& InOutLocation, float Inflation = 0.f) const;
	// Returns the current mirror mode
	EAxis::Type GetMirrorAxis() const;
	// Returns the current view mode
	EBrickEditorViewMode GetViewMode() const;

protected:
	// Called when the view mode has changed
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode);
	// Updates center of mass etc
	void UpdatePhysicsProperties();
	// ~Editor

public:
	// ~Input
	// Used to commit the current operation
	bool OnPressedCommit();
	// Used to cancel the current operation
	bool OnPressedCancel();
	// ~Input

	// ~Gizmo
	// Whether the gizmo is currently visible
	bool IsGizmoVisible() const
	{
		return GizmoMeshComponent && GizmoMeshComponent->IsVisible();
	}

	// Shows or hides the gizmo
	void SetGizmoVisible(bool bNewVisible);
	// Updates the gizmo position and rotation
	void SetGizmoTransform(const FTransform& NewTransform);
	// Get the gizmo axis the mouse is over at the given screen position
	bool GetGizmoAxisAtScreenPosition(const FVector2D& ScreenPosition, FBrickEditorGizmoAxis& OutGizmoAxis) const;
	// Used to update the hovered gizmo axis
	void SetHoveredGizmoAxis(const FBrickEditorGizmoAxis& InGizmoAxis);
	// Can be used to override the highlighted axis
	void SetHighlightedGizmoAxis(const FBrickEditorGizmoAxis& InGizmoAxis);
	// Get the currently hovered axis
	const auto& GetHoveredGizmoAxis() const
	{
		return HoveredGizmoAxis;
	}

private:
	// Updates the material parameters of the gizmo mesh
	void UpdateGizmoPrimitiveData();
	// ~Gizmo

public:
	// ~Selection
	// Generates a short display name for all selected objects
	FText GetSelectionDisplayName() const;
	// Get the active object, i.e. the last selected object
	template <class T = UBrickEditorObject>
	T* GetActiveObject() const
	{
		if (SelectedObjects.Num())
		{
			return Cast<T>(SelectedObjects[0].Get());
		}

		return nullptr;
	}

	// Get an array of all currently selected objects
	template <typename T = UBrickEditorObject>
	void GetSelectedObjects(TArray<T*>& OutObjects) const
	{
		OutObjects.Reserve(SelectedObjects.Num());
		for (const auto& WeakPtr : SelectedObjects)
		{
			if (WeakPtr.IsValid() && WeakPtr->IsA<T>())
			{
				OutObjects.Add(CastChecked<T>(WeakPtr.Get()));
			}
		}
		OutObjects.Shrink();
	}

	// Version that returns the weak pointer array
	const TArray<TWeakObjectPtr<UBrickEditorObject>>& GetSelectedObjects() const
	{
		return SelectedObjects;
	}

	// Get all editable objects that are not currently selected or hidden
	template <typename T = UBrickEditorObject>
	void GetUnselectedObjects(TArray<T*>& OutObjects) const
	{
		GetObjectsByPredicate(OutObjects, [](UBrickEditorObject* Object)
		                      {
			                      return !Object->IsBrickSelected();
		                      }
		);
	}

	// Returns all objects that aren't hidden
	template <typename T = UBrickEditorObject>
	void GetVisibleObjects(TArray<T*>& OutObjects) const
	{
		GetObjectsByPredicate(OutObjects, [](UBrickEditorObject* Object)
		                      {
			                      return true;
		                      }
		);
	}

	// Get all visible objects that are not currently being moved
	template <typename T = UBrickEditorObject>
	void GetUnmovedObjects(TArray<T*>& OutObjects) const
	{
		GetObjectsByPredicate(OutObjects, [](UBrickEditorObject* Object)
		                      {
			                      return !Object->IsBeingMovedInEditor();
		                      }
		);
	}

	// Used to get all objects that match a certain criteria
	template <typename T = UBrickEditorObject, typename P, bool bIncludeHidden = false>
	void GetObjectsByPredicate(TArray<T*>& OutObjects, P Predicate) const
	{
		OutObjects.Reserve(EditorInterfaceComponent->GetBrickEditorObjects().Num());
		for (auto Object : EditorInterfaceComponent->GetBrickEditorObjects())
		{
			if (Predicate(Object) && (bIncludeHidden || !Object->IsHiddenInEditor()))
			{
				OutObjects.Add(CastChecked<T>(Object));
			}
		}
		OutObjects.Shrink();
	}

	// Used for selecting attached objects
	virtual void GetAttachedObjects(TArray<UBrickEditorObject*>& OutObjects, bool bRecursive) const;
	// Return whether any object is selected currently
	bool IsAnythingSelected() const
	{
		return SelectedObjects.Num() > 0;
	}

	// Return whether the given object can currently be selected
	virtual bool CanSelectObject(UBrickEditorObject* Object) const;
	// Main function used to select or unselect objects
	void SelectObjects(TArray<UBrickEditorObject*> InObjects, ESelectObjectsMode Mode, bool bUseLastActive, bool bMirrorSelection);
	// Get the currently hovered object
	UBrickEditorObject* GetHoveredObject() const;
	// Sets the currently hovered object
	void SetHoveredObject(UBrickEditorObject* Object);
	// Clears the hovered objects if it can't be hovered anymore
	void ValidateHoveredObject();
	// Whether the hovered object is currently pressed
	bool IsHoveredObjectPressed() const;
	// Set whether the hovered object should appear as pressed
	void SetIsHoveredObjectPressed(bool bNewPressed);

private:
	// Updates the selection state of a single objects
	void UpdateObjectSelectionState(UBrickEditorObject* Object, EBrickSelectionState NewSelectionState);
	// Used to add or remove objects to the selected list, returns true if the selection has changed
	bool SelectOrHideObjects(
		TArray<TWeakObjectPtr<UBrickEditorObject>>& OutSelectedObjects,
		const TArray<UBrickEditorObject*>& ObjectsToSelect,
		ESelectObjectsMode Mode,
		void (ABrickEditor::*SelectFunc)(UBrickEditorObject*, EBrickSelectionState),
		bool (ABrickEditor::*CanSelectFunc)(UBrickEditorObject*) const);
	// ~Selection

public:
	// ~Visibility
	// Returns whether the object may be hidden
	bool CanHideObject(UBrickEditorObject* Object) const;
	// Used to hide or unhide objects
	void HideObjects(const TArray<UBrickEditorObject*>& InObjects, bool bHide);
	// ~Visibility

	// ~Objects
	// Implement to get the list of object filters for the editor
	virtual const TArray<UClass*>& GetPlacableFilterClasses() const;
	// Return whether the given object can be deleted from the editor
	virtual bool CanDeleteObject(UBrickEditorObject* Object) const;
	// Called from the save interface when new objects have been created
	virtual void OnBrickEditorObjectsCreated(const TArray<UBrickEditorObject*>& NewObjects, EBrickEditorObjectCreationContext CreationContext);
	// Called from the save interface when objects have been destroyed
	void OnBrickEditorObjectsDestroyed(const TArray<UBrickEditorObject*>& DestroyedObjects);
	// Get the desired transform where new objects should be placed
	bool GetPlaceObjectTransform(FVector& OutLocation, FRotator& OutRotation) const;
	// Return whether a property of the object can be modified in the current mode
	bool CanModifyBrickProperty(const UBrickEditorObject* Object, const TSharedRef<const FBrickPropertyEditInfo>& PropertyInfo) const;
	// Returns the current amount of bricks with aerodynamics enabled
	auto GetNumObjectsWithAerodynamics() const
	{
		return NumObjectsWithAerodynamics;
	}

	// ~Objects

	// ~Tracing
	// Trace the world to get the object at the given screen position
	UBrickEditorObject* GetObjectAtScreenPosition(const FVector2D& ScreenPosition) const;

private:
	// Get the desired query params for click traces etc.
	void GetEditorTraceParams(ECollisionChannel& OutCollisionChannel, FCollisionQueryParams& OutParams) const;
	// ~Tracing

public:
	// ~Moving
	// Return whether the given object can be moved
	virtual bool CanMoveObject(UBrickEditorObject* Object) const { return true; }

	struct FGetMoveSnappingOffsetParams
	{
		const UBrickEditorMoveMode* MoveMode;
		const TArray<TWeakObjectPtr<UBrickEditorObject>>& MovedObjects;
		const TArray<UBrickEditorObject*>& UnselectedObjects;
		const FMoveTransform MoveTransform;
		const FMoveTransform MirroredTransform;
		float MaxSnappingDist;
		float MaxSnappingAngle;
		bool bSnapLocation;
		bool bSnapRotation;
	};

	// Called while moving objects, can be implemented to add brick or grid snapping
	virtual FBrickEditorSnappingOffset GetMoveSnappingOffset(const FGetMoveSnappingOffsetParams& Params) const
	{
		return FBrickEditorSnappingOffset();
	}

	struct FGetMoveObjectValidityStateParams
	{
		UBrickEditorObject* Object;
		const FTransform& NewTransform;
		const TArray<UBrickEditorObject*>& UnselectedObjects;
	};

	// Returns the current validity state of an object being moved
	virtual EBrickValidityState GetMoveObjectValidityState(const FGetMoveObjectValidityStateParams& Params) const;
	// ~Moving

	// ~Camera
	// Get the current camera view rotation in world space
	FRotator GetViewRotation() const;
	// Get the camera rotation relative to editor space
	FQuat GetCameraRelativeRotation() const;
	// Returns the editor relative location of the camera
	FVector GetCameraRelativeLocation() const
	{
		return CameraLocation;
	}

	// Sets the relative location of the camera
	void SetCameraRelativeLocation(FVector NewLocation);
	// Add movement input for the camera
	void AddCameraMovementInput(EAxis::Type Direction, float Val);
	// Called from the input component when the camera mode has changed
	void UpdateCameraMode(bool bNewMove, bool bNewPivot);
	// Focuses the camera on all selected objects
	void FocusSelection();

private:
	// Focuses the camera on the given objects
	void FocusCamera(const TArray<UBrickEditorObject*>& Objects);
	// ~Camera

public:
	// ~Undo/Redo
	// Used to undo or redo
	bool UndoRedo(bool bUndo);
	// Removes all undo and redo steps
	void ClearUndoBuffer();
	// Called from the scoped operation constructor, returns the root scope
	FBrickEditorScopedOperation* StartScopedOperation(FBrickEditorScopedOperation* Operation);
	// Called from the scoped operation destructor
	bool FinishScopedOperation(FBrickEditorScopedOperation* Operation);
	// Called after anything has been done in a scoped operation
	virtual void PostModify(const FBrickEditorScopedOperation& Operation);

protected:
	// Callback for the property system
	virtual void OnAnyPropertyModified(const FBrickPropertyChangedEvent& Event);
	// ~Undo/Redo

public:
	// ~Saving/Loading
	// Public accessor to the creation info
	const FUGCFileInfo& GetFileInfo() const
	{
		return FileInfo;
	}

	// Called from the editor interface after the item has been loaded and initialized
	void PostLoadItem(bool bSuccess, bool bFromImport, bool bFromUndo);
	// Sets the currently opened file info
	void SetFileInfo(const FUGCFileInfo& InFileInfo, const bool bFromQueryDetails = false);
	// Used to copy the meta data
	void SetMetaData(const FUGCFileInfo& InFileInfo);

private:
	// To be called any time the file info has changed
	void OnFileInfoChanged();
	// Updates the object count, dimensions, mass etc on the file info
	void UpdateItemProperties(bool bUpdateFileInfo = true);

public:
	// Sets the unsaved changes flag and broadcasts the delegate
	void SetHasUnsavedChanges(bool bNewHasUnsavedChanges);
	// Whether the edited item has to be saved at some point
	bool HasUnsavedChanges() const
	{
		return bHasUnsavedChanges;
	}

	// ~Saving/Loading

private:
	// ~Meshes
	// Updates the style of the bounds MID
	void UpdateBoundsMIDStyle();
	// Updates the grid mesh if needed
	void UpdateGridMesh();
	// ~Meshes

	// ~Outline
	// Callback for the UI style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Updates the pressed material parameter
	void UpdateHoveredObjectPressed();
	// Updates a single color parameter on the outline MID
	void UpdateOutlineColorParam(const FString& ParamName, EBrickUIColorStyle ColorStyle, EBrickUIStyleState StyleState);
	// ~Outline

public:
	// ~IViewTargetInterface
	virtual void UpdateViewRotation(FRotator& InOutRotation, float DeltaTime) override;
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual bool OverrideFocalDistance(float& InOutFocalDistance) override;

	virtual const FViewTargetCameraSpeedParams* GetCameraSpeedParams() const override
	{
		return &GetStaticInfo()->CameraSpeedParams;
	}

	virtual FViewTargetCameraSpeedCache* GetCameraSpeedCache() const override
	{
		return &CameraSpeedCache;
	}

	virtual const FViewTargetZoomParams* GetZoomParams() const override
	{
		return &GetStaticInfo()->ZoomParams;
	}

	virtual FViewTargetZoomCache* GetZoomCache() const override
	{
		return &ZoomCache;
	}

	// ~IViewTargetInterface
};

template <class T, typename... P>
FORCEINLINE T* ABrickEditor::EnterEditorMode(UClass* ModeClass, bool bRevertChanges, const P&... Params)
{
	// Don't allow reentering the same mode again
	if (IsInEditorMode(ModeClass))
	{
		return nullptr;
	}

	// Try to find a pooled mode
	T* NewMode = nullptr;
	for (auto* OtherMode : EditorModes)
	{
		if (OtherMode->GetClass() == ModeClass)
		{
			NewMode = CastChecked<T>(OtherMode);
			break;
		}
	}

	// Create a new mode if needed
	if (!NewMode && ModeClass)
	{
		NewMode = NewObject<T>(this, ModeClass, NAME_None, RF_Transient);
		NewMode->InitializeMode();
		EditorModes.Add(NewMode);
	}

	// Make sure the new mode can actually be started
	if (NewMode && !NewMode->CanBeginMode())
	{
		return nullptr;
	}

	// Try to end the previous mode
	if (CurrentEditorMode)
	{
		// Make sure the mode can currently be ended
		if (!CurrentEditorMode->EndMode(bRevertChanges))
		{
			return nullptr;
		}

		CurrentEditorMode = nullptr;
	}

	// Assign and start the new mode
	CurrentEditorMode = NewMode;
	if (NewMode)
	{
		NewMode->BeginMode(Params...);
		// NOTE: Don't call unnecessarily when the mode has been cleared
		OnEditorModeChanged();
	}

	return NewMode;
}
