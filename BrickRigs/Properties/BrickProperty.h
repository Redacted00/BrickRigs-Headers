#pragma once

#include "BrickEditor/BrickEditorSaveVersion.h"
#include "UI/Misc/TooltipContent.h"
#include "UI/Misc/DisplayInfo.h"
#include "BrickPropertyChangedEvent.h"
#include "CoreMinimal.h"

class UBrickEditorObject;
class UBrickEditorInterfaceComponent;
class UBrickColor;
class UTexture2D;
class UPropertyWidget;
class ABasePlayerController;
class ABrickEditor;
class IBrickPropertyInterface;
struct FBrickProperty;
struct FBrickPropertyReflection;
struct FBrickEditorReferenceResolver;

// Context for referencing the value of a specific property instance
struct FBrickPropertyContainer
{
	friend struct FBrickPropertyInstance;

	explicit FBrickPropertyContainer(const UObject* Object = nullptr)
		: RootObject(const_cast<UObject*>(Object))
	{
	}

	FBrickPropertyContainer(const UObject* Object, const TArray<void*>& ContainerChain)
		: RootObject(const_cast<UObject*>(Object)), ContainerChain(ContainerChain)
	{
	}

	// Get the outer brick editor of this container
	ABrickEditor* GetBrickEditor() const;

	// Get the outer brick editor interface of this container
	UBrickEditorInterfaceComponent* GetBrickEditorInterface() const;

	bool IsValid() const
	{
		return RootObject != nullptr;
	}

	void Reset()
	{
		RootObject = nullptr;
		ContainerChain.Reset();
	}

	void* GetContainer() const
	{
		return ContainerChain.Num() ? ContainerChain.Last() : RootObject;
	}

	template <typename T>
	T* CastContainerChecked() const
	{
		return static_cast<T*>(GetContainer());
	}

	UObject* GetRootObject() const
	{
		return RootObject;
	}

	template <typename T>
	T* CastRootObject() const
	{
		return Cast<T>(RootObject);
	}

private:
	UObject* RootObject;
	TArray<void*> ContainerChain;
};

struct FEnumPropertyItem
{
private:
	// ~Variables
	int32 Item;
	FDisplayInfo DisplayInfo;
	FTooltipContent TooltipContent;
	bool bIsEnabled;
	// ~Variables

public:
	template <typename ItemType>
	FEnumPropertyItem(ItemType InItem = INDEX_NONE, const FText& InName = FText(), const FTooltipContent& InTooltipContent = FTooltipContent())
		: Item(static_cast<int32>(InItem)), DisplayInfo(InName), TooltipContent(InTooltipContent), bIsEnabled(true)
	{
	}

	// Version that constructs an item with an icon
	template <typename ItemType>
	FEnumPropertyItem(ItemType InItem, const FDisplayInfo& InDisplayInfo, const FTooltipContent& InTooltipContent = FTooltipContent())
		: Item(static_cast<int32>(InItem)), DisplayInfo(InDisplayInfo), TooltipContent(InTooltipContent), bIsEnabled(true)
	{
	}

	int32 GetItem() const
	{
		return Item;
	}

	const auto& GetDisplayInfo() const
	{
		return DisplayInfo;
	}

	const auto& GetTooltipContent() const
	{
		return TooltipContent;
	}

	auto& SetIsEnabled(bool bNewEnabled)
	{
		bIsEnabled = bNewEnabled;
		return *this;
	}

	bool IsEnabled() const
	{
		return bIsEnabled;
	}
};

// Special attribute class needed for property attributes (since they need the container as an attribute for the delegate)
template <class T>
struct TBrickPropAttribute
{
	DECLARE_DELEGATE_RetVal_OneParam(T, FGetter, const FBrickPropertyContainer&);

private:
	// ~Variables
	TOptional<T> Value;
	FGetter Delegate;
	// ~Variables

public:
	// ~Constructor
	TBrickPropAttribute(const T& InValue)
		: Value(InValue)
	{
	}

	// ~Constructor
	TBrickPropAttribute(const FGetter& InDelegate)
		: Delegate(InDelegate)
	{
	}

	// Outputs the bound value if one has been assigned
	TOptional<T> Get(const FBrickPropertyContainer& Container) const
	{
		if (Delegate.IsBound())
		{
			return Delegate.Execute(Container);
		}

		return Value;
	}

	// Version that accepts a default value
	T Get(const FBrickPropertyContainer& Container, const T& DefaultValue) const
	{
		return Get(Container).Get(DefaultValue);
	}
};

struct FBrickProperty
{
protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FBrickPropertyModified, const FBrickPropertyChangedEvent&);

	// ~Variables
	// Pointer to the property
	FProperty* Property;
	// The name of the property, needed for input mappings and external properties without an actual FProperty
	FName PropertyName;
	// ~Variables

public:
	// ~Delegates
	// Static delegate that will be called when any property has been modified
	static FBrickPropertyModified AnyPropertyModifiedDelegate;
	// ~Delegates

	static void StaticTypeHierarchy(TArray<FName>&)
	{
	}

	// Used to construct a new brick property
	template <typename PropertyType, typename... ParamTypes>
	static TSharedRef<PropertyType> ConstructBrickProperty(const ParamTypes&... Params)
	{
		return MakeShared<PropertyType>(Params...);
	}

	// Overridden by subclasses to return the exact property type name
	virtual FName GetTypeName() const
	{
		return NAME_None;
	}

	// Overridden by subclasses to return the fundamental value type name, like FBoolProperty
	virtual FName GetValueTypeName() const
	{
		return NAME_None;
	}

	virtual bool IsOfTypeInternal(const FName& TypeName) const
	{
		return false;
	}

	virtual void GetTypeHierarchyInternal(TArray<FName>& OutHierarchy) const
	{
	}

	// This macro has to used to declare new property types
#define DECLARE_BRICK_PROP(PropertyType, ParentType) \
	typedef ParentType Super; \
	static FName StaticType() { EnsureBaseDeclared(); return #PropertyType; } \
	static void StaticTypeHierarchy(TArray<FName>& OutHierarchy) \
	{ \
		OutHierarchy.Add(StaticType()); \
		ParentType::StaticTypeHierarchy(OutHierarchy); \
	} \
	virtual FName GetTypeName() const override { return StaticType(); } \
	virtual bool IsOfTypeInternal(const FName& TypeName) const override \
	{ \
		return TypeName == StaticType() || Super::IsOfTypeInternal(TypeName); \
	} \
	virtual void GetTypeHierarchyInternal(TArray<FName>& OutHierarchy) const \
	{ \
		StaticTypeHierarchy(OutHierarchy); \
	}

	// Version for properties that derive from FBrickProperty directly
#define DECLARE_BRICK_PROP_BASE(PropertyType) \
	static void EnsureBaseDeclared() { } \
	DECLARE_BRICK_PROP(PropertyType, FBrickProperty) \
	virtual FName GetValueTypeName() const override { return StaticType(); }

	// ~Constructor
	FBrickProperty()
		: Property(nullptr)
	{
	}

	// ~Destructor
	virtual ~FBrickProperty()
	{
	}

	void SetPropertyParams(FProperty* InProperty, const FName& InPropertyName)
	{
		Property = InProperty;
		PropertyName = InPropertyName;
	}

	// Whether this struct is of the same type or a subtype
	template <typename T>
	bool IsOfType() const
	{
		return IsOfTypeInternal(T::StaticType());
	}

	// Get the property type and all parent types
	virtual void GetTypeHierarchy(TArray<FName>& OutHierarchy) const
	{
		GetTypeHierarchyInternal(OutHierarchy);
	}

	// Get the associated property
	FProperty* GetProperty() const
	{
		return Property;
	}

	const FName& GetPropertyName() const
	{
		return PropertyName;
	}

	// Whether this points to the same property
	bool IsSameProperty(const FBrickProperty& Other) const
	{
		// IMPORTANT: Also check for the other property name, otherwise this would fail for external properties and input mappings
		return this == &Other || PropertyName == Other.PropertyName;
	}

	// Returns true if both values are identical
	virtual bool ComparePropertyValues(const void* A, const void* B) const = 0;

protected:
	// Can be used to implement comparison more easily
	template <typename T>
	static bool CompareInternal(const void* A, const void* B)
	{
		return *static_cast<const T*>(A) == *static_cast<const T*>(B);
	}

public:
	// Whether the ComparePropertyInContainer function can be used
	bool CanComparePropertyInContainer() const
	{
		return Property != nullptr;
	}

	// Returns true if the values in both containers are identical, assumes a valid FProperty exists
	bool ComparePropertyInContainer(const FBrickPropertyContainer& ContainerA, const FBrickPropertyContainer& ContainerB) const;

	// Whether this property can be copied
	bool CanCopyProperty() const
	{
		return Property != nullptr;
	}

	// Whether this property can be copied from the given object
	bool CanCopyPropertyFrom(const UBrickEditorObject* Object) const;

	// Returns the property that should be copied from the other object, could be the same as this or another one with the same name and type
	TUniquePtr<FBrickPropertyInstance> GetPropertyToCopyFrom(const UBrickEditorObject* Object) const;

	// Copies the property value from a different property
	bool CopyFPropertyFrom(const FBrickPropertyContainer& Dest, const FBrickPropertyContainer& Src, const FProperty* SrcProperty) const;

	// Copies the value of one container to another, assuming both containers support the property
	bool CopyFProperty(const FBrickPropertyContainer& Dest, const FBrickPropertyContainer& Src) const;

	// Used to serialize the property value in a given container
	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver);

	// Whether the property can be set on the given object
	virtual bool DoesObjectContainPropertyInternal(const UObject* InObject) const
	{
		return true;
	}

	// Can be implemented for compatibility with the generic property widget class
	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const
	{
		return false;
	}

	// Can be implemented for compatibility with the generic property widget class
	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const
	{
		return false;
	}

	// Whether the text has been entered by a user and is thus untrustworthy
	virtual bool IsUserText() const
	{
		return false;
	}

	// Implement to export the property value as a string
	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const
	{
		ensure(false);
		return FString();
	}

	// Whether a value can be exported from the property
	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const
	{
		return false;
	}

	// Implement to set the property value from a string
	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const
	{
		ensure(false);
		return false;
	}

	// Whether the given buffer is legal to import
	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const
	{
		return false;
	}

	// Copies the property value to the clipboard
	bool CopyPropertyToClipboard(const FBrickPropertyContainer& Container) const;

	// Whether the property value can be copied
	bool CanCopyPropertyToClipboard(const FBrickPropertyContainer& Container) const;

	// Pastes the property value from the clipboard
	bool PastePropertyFromClipboard(const FBrickPropertyContainer& Container) const;

	// Whether the current clipboard content can be pasted
	bool CanPastePropertyFromClipboard(const FBrickPropertyContainer& Container) const;

private:
	// Version that outputs the buffer
	bool CanPastePropertyFromClipboard(const FBrickPropertyContainer& Container, FString& OutBuffer) const;

public:
	// Get the actual property value pointer
	template <typename ValueType>
	ValueType* GetValuePtr(const FBrickPropertyContainer& Container) const;

protected:
	// Used to get the property value
	template <typename ValueType>
	bool GetValueInternal(const FBrickPropertyContainer& Container, ValueType& OutValue) const;
	// Used to set the property value
	template <typename ValueType>
	bool SetValueInternal(const FBrickPropertyContainer& Container, const ValueType& NewValue) const;

	// Used to export structs with type trait operators
	template <typename ThisType, typename StructType>
	FString ExportPropertyInternal(const FBrickPropertyContainer& Container) const
	{
		auto Value = StructType();
		static_cast<const ThisType*>(this)->GetValue(Container, Value);
		auto Buffer = FString();
		Value.ExportTextItem(Buffer, StructType(), nullptr, 0, nullptr);
		return Buffer;
	}

	// Used to import structs with type trait operators
	template <typename ThisType, typename StructType>
	bool ImportPropertyInternal(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const
	{
		auto Value = StructType();
		Value.ImportTextItem(Buffer, 0, nullptr, nullptr);
		return static_cast<const ThisType*>(this)->SetValue(Container, Value);
	}

public:
	// Version that allows for custom value types
#define DECLARE_BRICK_PROP_TYPE_CUSTOM(PropertyType, ValueType) \
	FORCEINLINE TSharedRef<PropertyType> AutoConstructBrickProp(ValueType, const ParamTypes& ...Params) { \
		return FBrickProperty::ConstructBrickProperty<PropertyType>(Params...); }
	// This macro has to be implemented in the header file after the struct
	// IMPORTANT: We can't use a const reference for the value since that would allow implicit conversion
#define DECLARE_BRICK_PROP_TYPE(PropertyType, ValueType) \
	template<typename ...ParamTypes> \
	DECLARE_BRICK_PROP_TYPE_CUSTOM(PropertyType, const ValueType* InValue)

	// Macro that should be called when a property has been modified externally
#define OnPropertyModifiedExternally(Object, Property) OnPropertyModified(FBrickPropertyChangedEvent(GET_MEMBER_NAME_CHECKED(ThisClass, Property), Object))
	// To be called when an external property has been modified externally, for example when the screen resolution has been reverted
#define OnExternalPropertyModifiedExternally(Object, PropertyType) OnPropertyModified(FBrickPropertyChangedEvent(#PropertyType, Object))
	// Called when a property has been modified through the property interface or externally
	static void OnPropertyModified(const FBrickPropertyChangedEvent& InEvent);

	// Useful to set brick property values from an external source
#define SetPropertyValueExternal(Container, Property, NewValue) SetPropertyValueExternalInternal(Container, GET_MEMBER_NAME_CHECKED(ThisClass, Property), Property, NewValue)
	// Internal version that takes the property name
	template <typename ValueType>
	static bool SetPropertyValueExternalInternal(const FBrickPropertyContainer& Container, const FName& PropertyName, ValueType& Value, const ValueType& NewValue);
};

template <typename ValueType>
bool FBrickProperty::SetPropertyValueExternalInternal(const FBrickPropertyContainer& Container, const FName& PropertyName, ValueType& Value, const ValueType& NewValue)
{
	if (NewValue != Value)
	{
		Value = NewValue;
		OnPropertyModified(FBrickPropertyChangedEvent(PropertyName, Container.GetRootObject()));
		return true;
	}
	return false;
}

// Specialized version for the case this is called on the base class
template <>
FORCEINLINE bool FBrickProperty::IsOfType<FBrickProperty>() const
{
	return true;
}

template <typename ValueType>
ValueType* FBrickProperty::GetValuePtr(const FBrickPropertyContainer& Container) const
{
	check(Property && Container.IsValid());
	return Property->ContainerPtrToValuePtr<ValueType>(Container.GetContainer());
}

template <typename ValueType>
bool FBrickProperty::GetValueInternal(const FBrickPropertyContainer& Container, ValueType& OutValue) const
{
	check(Property && Container.IsValid());

	const ValueType* ValuePtr = GetValuePtr<ValueType>(Container);
	if (ValuePtr)
	{
		OutValue = *ValuePtr;
		return true;
	}

	return false;
}

template <typename ValueType>
bool FBrickProperty::SetValueInternal(const FBrickPropertyContainer& Container, const ValueType& NewValue) const
{
	check(Property && Container.IsValid());

	// Set the actual value
	ValueType* ValuePtr = GetValuePtr<ValueType>(Container);
	if (ValuePtr)
	{
		*ValuePtr = NewValue;
		return true;
	}

	return false;
}

struct FBrickPropertyInstance
{
protected:
	// ~Variables
	// The associated brick property
	const TSharedRef<FBrickProperty> BrickProperty;
	// The full name of the property, including parent properties separated with dots
	FString FullPropertyName;
	// Chain of struct parent properties
	// IMPORTANT: This CAN'T be stored on the FBrickProperty, since the parent properties differ for different properties in different containers
	const TArray<FStructProperty*> ParentPropertyChain;
	// ~Variables

public:
	// ~Constructor
	FBrickPropertyInstance(const TSharedRef<FBrickProperty>& InBrickProperty, const FString& InFullPropertyName, const TArray<FStructProperty*>& InParentPropertyChain)
		: BrickProperty(InBrickProperty), FullPropertyName(InFullPropertyName), ParentPropertyChain(InParentPropertyChain)
	{
		// Add the parent properties to the full property name
		for (const auto* ParentProperty : ParentPropertyChain)
		{
			check(ParentProperty);
			FullPropertyName = ParentProperty->GetName() + static_cast<TCHAR>('.') + FullPropertyName;
		}
	}

	bool IsSameProperty(const FBrickPropertyInstance& Other) const
	{
		return BrickProperty->IsSameProperty(Other.BrickProperty.Get()) && FullPropertyName == Other.FullPropertyName && ParentPropertyChain == Other.ParentPropertyChain;
	}

	FName GetPropertyName() const
	{
		return BrickProperty->GetPropertyName();
	}

	const FString& GetFullPropertyName() const
	{
		return FullPropertyName;
	}

	FName GetPropertyType() const
	{
		return BrickProperty->GetTypeName();
	}

	const TSharedRef<FBrickProperty>& GetBrickProperty() const
	{
		return BrickProperty;
	}

	FProperty* GetProperty() const
	{
		return BrickProperty->GetProperty();
	}

	FStructProperty* GetParentProperty() const
	{
		return ParentPropertyChain.Num() ? ParentPropertyChain.Last() : nullptr;
	}

	FProperty* GetRootProperty() const
	{
		return ParentPropertyChain.Num() ? ParentPropertyChain[0] : BrickProperty->GetProperty();
	}

	template <typename PropertyType>
	const PropertyType* GetBrickProperty() const
	{
		if (BrickProperty->IsOfType<PropertyType>())
		{
			return static_cast<const PropertyType*>(&BrickProperty.Get());
		}

		return nullptr;
	}

	// Whether this property is part of the given object
	bool IsInObject(const UObject* InObject) const
	{
		check(InObject);

		// Check if the object contains the FProperty
		const auto* RootProperty = GetRootProperty();
		if (RootProperty && !InObject->GetClass()->IsChildOf(RootProperty->GetOwnerStruct()))
		{
			return false;
		}

		// Ask the brick property as well
		return BrickProperty->DoesObjectContainPropertyInternal(InObject);
	}

	// Whether this property is part of the container
	bool IsInContainer(const FBrickPropertyContainer& Container) const
	{
		return IsInObject(Container.GetRootObject());
	}

	// Get the property container info for the given object, asserting it is supported
	void GetPropertyContainer(const UObject* InObject, FBrickPropertyContainer& OutContainer) const
	{
		check(InObject);
		check(IsInObject(InObject));
		check(!OutContainer.IsValid());

		OutContainer.RootObject = const_cast<UObject*>(InObject);

		// Add the container chain
		OutContainer.ContainerChain.Reserve(ParentPropertyChain.Num());
		auto* ActualContainer = static_cast<void*>(const_cast<UObject*>(InObject));
		for (const auto* ParentProperty : ParentPropertyChain)
		{
			ActualContainer = ParentProperty->ContainerPtrToValuePtr<void>(ActualContainer);
			OutContainer.ContainerChain.Add(ActualContainer);
		}
	}

	FBrickPropertyContainer GetPropertyContainer(const UObject* InObject) const
	{
		auto OutContainer = FBrickPropertyContainer();
		GetPropertyContainer(InObject, OutContainer);
		return OutContainer;
	}

	template <typename PropertyType, typename ValueType>
	bool GetPropertyValueForContainer(const FBrickPropertyContainer& Container, ValueType& OutValue) const
	{
		const auto* CastedProperty = GetBrickProperty<PropertyType>();
		if (CastedProperty && IsInContainer(Container))
		{
			return CastedProperty->GetValue(Container, OutValue);
		}

		return false;
	}

	// Version that uses the FGetBrickPropertyValueParams struct
	template <typename PropertyType, typename ValueType>
	bool GetPropertyDefaultValueForContainer(const FBrickPropertyContainer& Container, ValueType& OutValue, FDisplayInfo& OutDisplayInfo) const
	{
		const auto* CastedProperty = GetBrickProperty<PropertyType>();
		if (CastedProperty && IsInContainer(Container))
		{
			return CastedProperty->GetDefaultValue(Container, OutValue, OutDisplayInfo);
		}

		return false;
	}
};
