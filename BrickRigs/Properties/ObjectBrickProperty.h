#pragma once

#include "Serialization/SerializationHelper.h"
#include "BrickProperty.h"

class IObjectPropertyItemInterface;

struct FObjectBrickPropertyBase : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FObjectBrickPropertyBase);

	DECLARE_DELEGATE_RetVal_TwoParams(bool, FIsObjectSupported, const FBrickPropertyContainer&, UObject*);

private:
	// ~Variables
	const FIsObjectSupported IsObjectSupportedDelegateDelegate;
	// ~Variables

public:
	// ~Constructor
	FObjectBrickPropertyBase(const FIsObjectSupported& InDelegate)
		: IsObjectSupportedDelegateDelegate(InDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override;

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return false;
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto* Value = static_cast<UObject*>(nullptr);
		GetValue(Container, Value);
		return GetNameSafe(Value);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return SetValue(Container, FSerializationHelper::LoadObjectFromName(Buffer, GetSupportedObjects(Container), true));
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, UObject*& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, UObject* NewValue) const
	{
		// Make sure the value is actually supported, when multiple objects are selected it might not be
		return DoesContainerSupportObject(Container, NewValue) && SetValueInternal(Container, NewValue);
	}

	// Implement to return the array of supported objects
	virtual const TArray<UObject*>& GetSupportedObjects(const FBrickPropertyContainer& Container) const;
	// Return the object as an object property item interface, virtual so it can be overridden for class properties
	virtual const IObjectPropertyItemInterface* GetItemInterface(const UObject* Object) const;
	// Invokes the delegate to check if the given value is supported
	bool DoesContainerSupportObject(const FBrickPropertyContainer& Container, UObject* Object) const;

	static FText GetNoObjectDisplayText()
	{
		return NSLOCTEXT("Misc", "NoObject", "None");
	}
};

template <typename ObjectType, typename = typename TEnableIf<TPointerIsConvertibleFromTo<ObjectType, const UObject>::Value>::Type>
struct FObjectBrickProperty : FObjectBrickPropertyBase
{
	DECLARE_BRICK_PROP(FObjectBrickProperty, FObjectBrickPropertyBase);

	DECLARE_DELEGATE_RetVal(const TArray<ObjectType*>&, FGetObjects);

private:
	// ~Variables
	const FGetObjects GetSupportedObjectsDelegate;
	// ~Variables

public:
	// ~Constructor
	FObjectBrickProperty(const FGetObjects& InDelegate, const FIsObjectSupported& InSupportedDelegate = FIsObjectSupported())
		: Super(InSupportedDelegate), GetSupportedObjectsDelegate(InDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<ObjectType*>(A, B);
	}

	virtual const TArray<UObject*>& GetSupportedObjects(const FBrickPropertyContainer& Container) const override
	{
		if (GetSupportedObjectsDelegate.IsBound())
		{
			return reinterpret_cast<const TArray<UObject*>&>(GetSupportedObjectsDelegate.Execute());
		}
		return FObjectBrickPropertyBase::GetSupportedObjects(Container);
	}

	// ~Super Interface
};

template <typename ObjectType, typename... ParamTypes, typename = typename TEnableIf<TPointerIsConvertibleFromTo<ObjectType, const UObject>::Value>::Type>
DECLARE_BRICK_PROP_TYPE_CUSTOM(FObjectBrickProperty<ObjectType>, ObjectType* const*);

// Base struct for blueprint class properties
struct FClassBrickProperty : FObjectBrickPropertyBase
{
	DECLARE_BRICK_PROP(FClassBrickProperty, FObjectBrickPropertyBase);

	DECLARE_DELEGATE_RetVal(const TArray<UClass*>&, FGetClasses);

	// ~Variables
	const FGetClasses GetSupportedClassesDelegate;
	// ~Variables

	// ~Constructor
	FClassBrickProperty(const FGetClasses& InDelegate, const FIsObjectSupported& InSupportedDelegate = FIsObjectSupported())
		: Super(InSupportedDelegate), GetSupportedClassesDelegate(InDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<TSubclassOf<UObject>>(A, B);
	}

	virtual const TArray<UObject*>& GetSupportedObjects(const FBrickPropertyContainer& Container) const override final;
	virtual const IObjectPropertyItemInterface* GetItemInterface(const UObject* Object) const override final;
	// ~Super Interface
};

template <typename ObjectType, typename... ParamTypes, typename = typename TEnableIf<TPointerIsConvertibleFromTo<ObjectType, const UObject>::Value>::Type>
DECLARE_BRICK_PROP_TYPE_CUSTOM(FClassBrickProperty, const TSubclassOf<ObjectType>*);
