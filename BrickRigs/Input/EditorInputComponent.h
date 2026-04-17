// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditor/BrickEditorTypes.h"
#include "CoreMinimal.h"
#include "Input/BaseInputComponent.h"
#include "EditorInputComponent.generated.h"

class ABrickEditor;
class UBrickEditorMode;
struct FBrickInputMode;

// Base class for all editor related input components
UCLASS(Abstract)
class BRICKRIGS_API UBaseEditorInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Cached pointer to the outer editor
	UPROPERTY(Transient)
	ABrickEditor* BrickEditor;
	// Cached editor mode, needed to unbind delegates
	TWeakObjectPtr<UBrickEditorMode> CurrentEditorMode;
	// ~Variables

public:
	// ~Constructor
	UBaseEditorInputComponent();

	// ~Super Interface
	virtual void OnUnregister() override;
	virtual void OverrideActionDisplayInfo(const FName& ActionName, FDisplayInfo& InOutDisplayInfo) const override;
	virtual void SetPlayerController(ABasePlayerController* PC) override;
	// ~Super Interface

protected:
	// Sets the outer editor
	virtual void SetBrickEditor(ABrickEditor* InEditor);
	// Called from the editor when the current mode has changed
	virtual void OnEditorModeChanged(UBrickEditorMode* NewMode);
	// Sets the current editor mode, binds and unbinds delegates
	virtual void SetEditorMode(UBrickEditorMode* NewMode);
	// Returns the current editor mode
	template <class T>
	T* GetCurrentEditorMode() const
	{
		return Cast<T>(CurrentEditorMode.Get());
	}

	// Exiting
	UFUNCTION()
	void OnPressedExitEditor();
	UFUNCTION()
	void OnPressedExitEditorWithVehicle();
	UFUNCTION()
	bool GetExitEditorWithVehicleEnabled() const;

	// File IO
	UFUNCTION()
	void OnPressedNewItem();
	UFUNCTION()
	void OnPressedOpenItem();
	UFUNCTION()
	void OnPressedImportItem();
	UFUNCTION()
	void OnPressedSaveItem();
	UFUNCTION()
	bool GetSaveItemEnabled() const;
	UFUNCTION()
	void OnPressedSaveItemAs();
	UFUNCTION()
	bool GetSaveItemAsEnabled() const;
	UFUNCTION()
	void OnPressedUploadItem();
	UFUNCTION()
	bool GetUploadItemEnabled() const;
	UFUNCTION()
	void OnPressedUploadItemAs();
	UFUNCTION()
	bool GetUploadItemAsEnabled() const;
	UFUNCTION()
	void OnPressedEditMetaData();
	UFUNCTION()
	void OnPressedCaptureThumbnail();

	// Commit and cancel
	UFUNCTION()
	void OnPressedEditorCommit();
	UFUNCTION()
	void OnPressedEditorCancel();

	// Undo/redo
	UFUNCTION()
	void OnPressedUndo();
	UFUNCTION()
	void OnPressedRedo();

	// Select
	UFUNCTION()
	void OnPressedSelect();
	UFUNCTION()
	void OnReleasedSelect();
	UFUNCTION()
	void OnPressedAddToSelection();
	UFUNCTION()
	void OnReleasedAddToSelection();
	// Shared event used for normal and additive selection
	void OnPressedSelect(bool bAddToSelection);
	void OnReleasedSelect(bool bAddToSelection);

	UFUNCTION()
	void OnPressedToggleSelection();
	UFUNCTION()
	void OnPressedInvertSelection();

	// Select by type
	UFUNCTION()
	void OnPressedSelectByType();
	UFUNCTION()
	void OnPressedSelectByClass();
	UFUNCTION()
	void OnPressedSelectByMaterial();
	UFUNCTION()
	void OnPressedSelectByColor();
	UFUNCTION()
	void OnPressedSelectByPattern();

	// Select Attached
	UFUNCTION()
	void OnPressedSelectAttached();
	UFUNCTION()
	void OnPressedSelectAttachedRecursive();

	// Focus
	UFUNCTION()
	void OnPressedFocusSelection();

	// Moving and Rotating
	UFUNCTION()
	void OnPressedMoveSelection();

	// Deleting
	UFUNCTION()
	void OnPressedDeleteSelection();

	// Duplicating
	UFUNCTION()
	void OnPressedDuplicateSelection();
	UFUNCTION()
	void OnPressedDuplicateSelectionMirrored();
	UFUNCTION()
	void OnPressedMirrorSelection();

	// Visibility
	UFUNCTION()
	void OnPressedHideSelection();
	UFUNCTION()
	void OnPressedUnhideAll();

	// View Modes
	UFUNCTION()
	void OnPressedCycleViewMode();
	UFUNCTION()
	FText GetCycleViewModeValueText() const;

	// Mirroring
	UFUNCTION()
	void OnPressedToggleMirrorMode();

	// Move Mode
	UFUNCTION()
	void OnPressedToggleSnapping();
	UFUNCTION()
	void OnPressedToggleTransformSpace();
	UFUNCTION()
	void OnPressedMovePerpendicular();
	UFUNCTION()
	void OnReleasedMovePerpendicular();

	UFUNCTION()
	void OnPressedMoveAxisX()
	{
		StartMoveOrSetGizmoAxis({EAxis::X, EBrickEditorGizmoFeature::RotateAxis}, {EAxis::None, EBrickEditorGizmoFeature::MoveAxis});
	}

	UFUNCTION()
	void OnPressedMoveAxisY()
	{
		StartMoveOrSetGizmoAxis({EAxis::Y, EBrickEditorGizmoFeature::RotateAxis}, {EAxis::None, EBrickEditorGizmoFeature::MoveAxis});
	}

	UFUNCTION()
	void OnPressedMoveAxisZ()
	{
		StartMoveOrSetGizmoAxis({EAxis::Z, EBrickEditorGizmoFeature::RotateAxis}, {EAxis::None, EBrickEditorGizmoFeature::MoveAxis});
	}

	// Either changes the gizmo axis while moving or starts a move with the given axis
	void StartMoveOrSetGizmoAxis(FBrickEditorGizmoAxis GizmoAxis, const FBrickEditorGizmoAxis& ToggleAxis = {});
};

/**
 * Input component used for the brick editor
 */
UCLASS(Within = BrickEditor)
class BRICKRIGS_API UEditorInputComponent : public UBaseEditorInputComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Hold action states
	uint8 bPressedMoveCamera : 1;
	uint8 bPressedPivotCamera : 1;
	uint8 bIsMovingCamera : 1;
	// Cached mouse position before mouse capture has started
	FIntPoint MousePosBeforeCapture;
	// ~Variables

public:
	// ~Constructor
	UEditorInputComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void MouseMoveUp(float Val) override;
	virtual void MouseMoveRight(float Val) override;
	virtual bool OnPivotCamera(bool bPressed) override;
	virtual void OnEditorModeChanged(UBrickEditorMode* NewMode) override;
	virtual void SetEditorMode(UBrickEditorMode* NewMode) override;
	// ~Super Interface

	// Called by the player controller to get the desired input parameters
	bool GetInputMode(FBrickInputMode& OutInputMode) const;
	// Updates the input mode on the player controller
	void UpdateInputMode();
	// Called by the player input component
	void OnMoveCamera(bool bPressed);

protected:
	// Updates the camera move mode if needed
	void UpdateCameraMode();
	// Callback for the move mode
	void OnMoveGizmoAxisChanged(const FBrickEditorGizmoAxis& NewAxis);

	// Moving
	UFUNCTION()
	void MoveForward(float Val);
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Val);
	// Used to add movement input in any direction
	void AddMovementInput(EAxis::Type Direction, float Val);

	// Used by the mouse move events
	bool ConsumeMouseMoveInput(EAxis::Type Direction, float Val);
};
