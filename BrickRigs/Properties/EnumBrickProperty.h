#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"

DECLARE_DELEGATE_TwoParams(FGetEnumItems, const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>&);
template <typename EnumType>
using FIsEnumValueSupported = TDelegate<bool(const FBrickPropertyContainer& Container, EnumType)>;

struct FEnumBrickPropertyBase : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FEnumBrickPropertyBase);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override
	{
		auto Value = static_cast<int32>(INDEX_NONE);
		if (GetValue(Container, Value))
		{
			auto EnumItems = TArray<FEnumPropertyItem>();
			GetEnumItems(Container, EnumItems);

			const auto* FoundItem = EnumItems.FindByPredicate([&](const auto& EnumItem) { return EnumItem.GetItem() == Value; });
			if (FoundItem)
			{
				OutValue = FoundItem->GetDisplayInfo().Name;
			}
			else
			{
				OutValue = INVTEXT("Invalid");
			}

			return true;
		}

		return false;
	}

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return SetValue(Container, FCString::Atoi(*NewValue.ToString()));
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = 0;
		GetValue(Container, Value);
		return LexToString(Value);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		auto NewValue = 0;
		LexFromString(NewValue, Buffer);
		return SetValue(Container, NewValue);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const
	{
		checkNoEntry();
		return false;
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const
	{
		checkNoEntry();
		return false;
	}

	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const
	{
		checkNoEntry();
	}

	// Whether the given container supports the specific value
	virtual bool DoesContainerSupportValue(const FBrickPropertyContainer& Container, int32 Value) const
	{
		return true;
	}

	// Useful to cycle an enum property value, for example in the context menu
#define CycleEnumPropertyExternal(Object, Property, Getter, bUpdateAllProperties) CycleEnumPropertyExternalInternal(Object, GET_MEMBER_NAME_CHECKED(ThisClass, Property), Property, Getter, bUpdateAllProperties)
	// Internal version with a longer syntax
	template <typename EnumType>
	static bool CycleEnumPropertyExternalInternal(const FBrickPropertyContainer& Container, const FName& PropertyName, EnumType& Value, const FGetEnumItems& GetItemsDelegate, const bool bUpdateAllProperties = false);
	// Selects the first valid enum item
	template <typename EnumType>
	static bool ValidateEnumProperty(const FBrickPropertyContainer& Container, EnumType& Value, const FGetEnumItems& GetItemsDelegate, const FIsEnumValueSupported<EnumType>& IsValueSupportedDelegate, EnumType* FallbackValue = nullptr);
	// Useful to get the display name for the current enum value
	template <typename EnumType>
	static FText GetEnumPropertyValueText(const FBrickPropertyContainer& Container, const EnumType& Value, const FGetEnumItems& GetItemsDelegate);
};

template <typename EnumType>
struct FEnumBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FEnumBrickProperty, FEnumBrickPropertyBase);

private:
	// ~Variables
	const FGetEnumItems GetEnumItemsDelegate;
	const FIsEnumValueSupported<EnumType> IsValueSupportedDelegate;
	// ~Variables

public:
	// ~Constructor
	FEnumBrickProperty(const FGetEnumItems& InDelegate, const FIsEnumValueSupported<EnumType>& InIsSupportedDelegate = FIsEnumValueSupported<EnumType>())
		: FEnumBrickPropertyBase(), GetEnumItemsDelegate(InDelegate), IsValueSupportedDelegate(InIsSupportedDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<EnumType>(A, B);
	}

	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override
	{
		uint8 EnumValue;
		if (GetValueInternal<uint8>(Container, EnumValue))
		{
			OutValue = EnumValue;
			return true;
		}
		return false;
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override
	{
		// Make sure the value is actually supported
		return DoesContainerSupportValue(Container, NewValue) && SetValueInternal<uint8>(Container, NewValue);
	}

	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override
	{
		GetEnumItemsDelegate.ExecuteIfBound(Container, OutItems);
	}

	virtual bool DoesContainerSupportValue(const FBrickPropertyContainer& Container, int32 Value) const override
	{
		return !IsValueSupportedDelegate.IsBound() || IsValueSupportedDelegate.Execute(Container, EnumType(Value));
	}

	// ~Super Interface
};

template <typename EnumType, typename... ParamTypes, typename = typename TEnableIf<TIsEnumClass<EnumType>::Value>::Type>
DECLARE_BRICK_PROP_TYPE_CUSTOM(FEnumBrickProperty<EnumType>, const EnumType*);

template <typename EnumType, typename... ParamTypes>
DECLARE_BRICK_PROP_TYPE_CUSTOM(FEnumBrickProperty<EnumType>, const TEnumAsByte<EnumType>*);

template <typename EnumType>
bool FEnumBrickPropertyBase::CycleEnumPropertyExternalInternal(const FBrickPropertyContainer& Container, const FName& PropertyName, EnumType& Value, const FGetEnumItems& GetItemsDelegate, const bool bUpdateAllProperties)
{
	TArray<FEnumPropertyItem> EnumItems;
	GetItemsDelegate.Execute(Container, EnumItems);

	int32 NewIndex = INDEX_NONE;
	for (auto i = 0; i < EnumItems.Num(); ++i)
	{
		if (EnumItems[i].GetItem() == static_cast<int32>(Value))
		{
			NewIndex = i;
			break;
		}
	}

	NewIndex++;

	// Wrap around
	if (NewIndex >= EnumItems.Num())
	{
		NewIndex = 0;
	}

	if (EnumItems.IsValidIndex(NewIndex))
	{
		const EnumType NewValue = EnumType(EnumItems[NewIndex].GetItem());
		return SetPropertyValueExternalInternal(Container, PropertyName, Value, NewValue, bUpdateAllProperties);
	}

	return false;
}

template <typename EnumType>
bool FEnumBrickPropertyBase::ValidateEnumProperty(const FBrickPropertyContainer& Container, EnumType& Value, const FGetEnumItems& GetItemsDelegate, const FIsEnumValueSupported<EnumType>& IsValueSupportedDelegate, EnumType* FallbackValue)
{
	TArray<FEnumPropertyItem> EnumItems;
	GetItemsDelegate.Execute(Container, EnumItems);

	// Try to find the selected item in the array
	const int32 SelectedItemIndex = EnumItems.FindLastByPredicate([&](const FEnumPropertyItem& Item)
	{
		return Item.GetItem() == static_cast<int32>(Value);
	});

	// Check if the selected item is already supported
	if (
		SelectedItemIndex != INDEX_NONE
		&& (!IsValueSupportedDelegate.IsBound() || IsValueSupportedDelegate.Execute(Container, Value))
	)
	{
		return true;
	}

	if (FallbackValue)
	{
		Value = *FallbackValue;
		return true;
	}
	// Search for an allowed item
	for (auto i = 0; i < EnumItems.Num(); ++i)
	{
		if (
			i != SelectedItemIndex
			&& (!IsValueSupportedDelegate.IsBound() || IsValueSupportedDelegate.Execute(Container, static_cast<EnumType>(EnumItems[i].GetItem())))
		)
		{
			Value = static_cast<EnumType>(EnumItems[i].GetItem());
			return true;
		}
	}

	return false;
}

template <typename EnumType>
FText FEnumBrickPropertyBase::GetEnumPropertyValueText(const FBrickPropertyContainer& Container, const EnumType& Value, const FGetEnumItems& GetItemsDelegate)
{
	TArray<FEnumPropertyItem> EnumItems;
	GetItemsDelegate.Execute(Container, EnumItems);

	for (const auto& Item : EnumItems)
	{
		if (Item.GetItem() == static_cast<int32>(Value))
		{
			return Item.GetDisplayInfo().Name;
		}
	}

	return FText::FromString("Invalid");
}
