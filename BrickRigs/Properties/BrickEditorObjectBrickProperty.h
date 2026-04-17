#pragma once

#include "BrickProperty.h"

class UBrickEditorObject;
struct FBrickEditorObjectPtr;

DECLARE_DELEGATE_RetVal_TwoParams(bool, FIsBrickEditorObjectAllowed, const FBrickPropertyContainer&, const UBrickEditorObject*);
DECLARE_DELEGATE_RetVal_ThreeParams(bool, FGetBEOPropertyDefaultValue, const FBrickPropertyContainer&, TArray<FBrickEditorObjectPtr>&, FDisplayInfo&);
DECLARE_DELEGATE_RetVal_OneParam(int32, FGetMaxNumBrickEditorObjects, const FBrickPropertyContainer&);

struct FBrickEditorObjectBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FBrickEditorObjectBrickProperty);

private:
	// ~Variables
	FIsBrickEditorObjectAllowed AllowedDelegate;
	FGetBEOPropertyDefaultValue GetDefaultValueDelegate;
	FGetMaxNumBrickEditorObjects GetMaxNumDelegate;
	// ~Variables

public:
	// ~Constructor
	FBrickEditorObjectBrickProperty(const FIsBrickEditorObjectAllowed& AllowedDelegate = FIsBrickEditorObjectAllowed(), const FGetBEOPropertyDefaultValue& GetDefaultValueDelegate = FGetBEOPropertyDefaultValue(), const FGetMaxNumBrickEditorObjects& GetMaxNumDelegate = FGetMaxNumBrickEditorObjects())
		: AllowedDelegate(AllowedDelegate), GetDefaultValueDelegate(GetDefaultValueDelegate), GetMaxNumDelegate(GetMaxNumDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override;
	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override;

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override;

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, TArray<FBrickEditorObjectPtr>& OutObjects) const;

	virtual bool GetDefaultValue(const FBrickPropertyContainer& Container, TArray<FBrickEditorObjectPtr>& OutObjects, FDisplayInfo& OutDisplayInfo) const;

	virtual bool SetValue(const FBrickPropertyContainer& Container, const TArray<FBrickEditorObjectPtr>& NewObjects) const;

	bool IsArrayProperty() const
	{
		return Property && Property->IsA<FArrayProperty>();
	}

	// Returns whether an object is supported by this property
	bool IsObjectAllowed(const FBrickPropertyContainer& Container, const UBrickEditorObject* Object) const
	{
		return !Object || !AllowedDelegate.IsBound() || AllowedDelegate.Execute(Container, Object);
	}

	// Returns the maximum number of objects that can be selected
	int32 GetMaxNumObjects(const FBrickPropertyContainer& Container) const
	{
		return GetMaxNumDelegate.IsBound() ? GetMaxNumDelegate.Execute(Container) : IsArrayProperty() ? -1 : 1;
	}
};

DECLARE_BRICK_PROP_TYPE(FBrickEditorObjectBrickProperty, FBrickEditorObjectPtr);
DECLARE_BRICK_PROP_TYPE(FBrickEditorObjectBrickProperty, TArray<FBrickEditorObjectPtr>);
