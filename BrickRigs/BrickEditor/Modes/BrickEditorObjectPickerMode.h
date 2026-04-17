// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BrickEditorMode.h"
#include "BrickEditorObjectPickerMode.generated.h"

// Base class for modes that are used to select objects
UCLASS(Abstract)
class BRICKRIGS_API UBrickEditorObjectPickerMode : public UBrickEditorMode
{
	GENERATED_BODY()

protected:
	// ~Variables
	// List of currently selected objects
	TArray<TWeakObjectPtr<UBrickEditorObject>> PickedObjects;
	// The property that is being edited
	TSharedPtr<FBrickPropertyEditInfo> PropertyInfo;
	// ~Variables

public:
	// ~Super Interface
	virtual bool EndMode(bool bRevertChanges) override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual bool CanEditBrickProperty(const UBrickEditorObject* Container, const TSharedRef<const FBrickPropertyEditInfo>& InPropertyInfo) const override;
	virtual void SelectObjects(const TArray<UBrickEditorObject*>& Objects, bool bAddToSelection) override;
	virtual bool ShouldUpdateHoveredObject() const override { return true; }
	virtual bool ShouldObjectBeHidden(UBrickEditorObject* Object) const override;
	// ~Super Interface

	// Custom begin function
	void BeginMode(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo);

private:
	// Whether the given object can be selected
	virtual bool IsObjectAllowed(const UBrickEditorObject* Object) const
	{
		return false;
	}

	// Return the maximum number of objects allowed to be picked
	virtual int32 GetMaxNumObjectsToPick() const
	{
		return INDEX_NONE;
	}

	// Marks an object as selected or unselected
	void UpdateObjectSelectionState(UBrickEditorObject* Object, bool bNewSelected);
};

// Mode used to select objects for object properties
UCLASS()
class BRICKRIGS_API UBrickEditorObjectPropertyPickerMode : public UBrickEditorObjectPickerMode
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual bool EndMode(bool bRevertChanges) override;
	virtual bool IsObjectAllowed(const UBrickEditorObject* Object) const override;
	virtual int32 GetMaxNumObjectsToPick() const override;
	// ~Super Interface

	// Custom begin function
	void BeginMode(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo);
};

// Mode used to pick objects to copy their property value
UCLASS()
class BRICKRIGS_API UBrickEditorPropertyPickerMode : public UBrickEditorObjectPickerMode
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual bool EndMode(bool bRevertChanges) override;
	virtual bool IsObjectAllowed(const UBrickEditorObject* Object) const override;

	virtual int32 GetMaxNumObjectsToPick() const override
	{
		return 1;
	}

	// ~Super Interface
};
