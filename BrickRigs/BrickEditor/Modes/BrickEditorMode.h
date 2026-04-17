// Copyright Fluppisoft, 2020

#pragma once

#include "Input/EditorInputComponent.h"
#include "BrickEditor/BrickEditorTypes.h"
#include "Properties/BrickPropertyInterface.h"
#include "BrickEditor/BrickEditorSnappingOffset.h"
#include "Input/BaseInputComponent.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BrickEditorMode.generated.h"

// Actors
class ABrickEditor;
class UBrickEditorInterfaceComponent;

// Input component used for the object context menu
UCLASS()
class BRICKRIGS_API UEditorContextInputComponent : public UBaseEditorInputComponent
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface
};

UCLASS(Abstract, Within = BrickEditor)
class BRICKRIGS_API UBrickEditorMode : public UObject, public IBrickPropertyInterface
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The cached outer editor
	UPROPERTY(Transient)
	ABrickEditor* BrickEditor;
	// ~Variables

public:
	// Called from the editor after creation
	void InitializeMode();
	// Called when entering this mode (again)
	void BeginMode();
	// Get the interface component from the owning editor
	UBrickEditorInterfaceComponent* GetEditorInterface() const;
	// Opens the editor context menu
	void OpenContextMenu();

	// Gets the input actions that should be bound for the current mode
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const;
	// Whether the mouse should be permanently captured in this mode
	virtual bool ShouldCaptureMouse() const;

	// Whether this mode is the one being used in the editor
	virtual bool IsCurrentMode() const;
	// If this mode can currently be used
	virtual bool CanBeginMode() const { return true; }
	// Called to finish the mode
	virtual bool EndMode(bool bRevertChanges) { return true; }
	// Tick function called while the mode is active
	virtual void TickMode(float DeltaTime)
	{
	}

	// Used to allow modes to do some cleaning up before exiting the editor
	virtual void OnExitEditor()
	{
	}

	// Whether objects can be placed in this mode
	virtual bool CanPlaceObjects() const { return false; }
	// Whether objects should show their editor visualization
	virtual bool ShouldShowEditorVisualization() const { return false; }
	// Whether the properties panel for selected objects should be shown
	virtual bool ShouldShowSelectionProperties() const { return false; }
	// Whether the properties panel should be enabled in this mode
	virtual bool ShouldEnablePropertiesPanel() const { return true; }

	// Can be implemented to set user settings categories to display, return true to display properties
	virtual bool GetUserSettingsPropertyFilter(FBrickPropertyReflectionFilter& OutFilter) const
	{
		return false;
	}

	// Whether the given property can be edited in this mode
	virtual bool CanEditBrickProperty(const UBrickEditorObject* Container, const TSharedRef<const FBrickPropertyEditInfo>& PropertyInfo) const { return ShouldShowSelectionProperties(); }
	// Whether objects can be deleted in this mode
	virtual bool CanDeleteSelection() const { return false; }
	// Tries to delete the selected objects, goes back to the default mode
	virtual void DeleteSelection();

protected:
	// Internal version that deletes the selection without cancelling the mode
	void ForceDeleteSelection();

public:
	// Used to click objects or the gizmo
	virtual void PressObjectAtScreenPosition(const FVector2D& ScreenPosition, bool bAddToSelection);
	virtual void ReleaseObjectAtScreenPosition(const FVector2D& ScreenPosition, bool bAddToSelection);
	// Can be implemented to make use of the pressed objects
	virtual void SelectObjects(const TArray<UBrickEditorObject*>& Objects, bool bAddToSelection)
	{
	}

	// Whether the object should be hidden in this mode
	virtual bool ShouldObjectBeHidden(UBrickEditorObject* Object) const { return false; }
	// Updates the hidden state of all objects
	void UpdateObjectsHidden();
	// Whether the hovered object should be updated
	virtual bool ShouldUpdateHoveredObject() const { return false; }
	// Updates the hovered object
	void HoverObjectAtScreenPosition(const FVector2D& ScreenPosition);
	// Returns the currently desired gizmo transform
	virtual TOptional<FTransform> GetGizmoTransform() const
	{
		return {};
	}

protected:
	// Helper to get the UGC type of the editor
	EUGCType GetUGCType() const;

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface
};
