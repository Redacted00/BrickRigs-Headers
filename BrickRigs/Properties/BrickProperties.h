#pragma once

#include "UGC/UGCTypes.h"
#include "ObjectBrickProperty.h"
#include "EnumBrickProperty.h"
#include "CoreMinimal.h"

class USirenSequence;

struct FDriverSeatBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FDriverSeatBrickProperty, FEnumBrickPropertyBase);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override;
	virtual bool DoesObjectContainPropertyInternal(const UObject* InObject) const override;
	// ~Super Interface
};

struct FUseExitLocationBrickProperty : FBoolBrickProperty
{
	DECLARE_BRICK_PROP(FUseExitLocationBrickProperty, FBoolBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, bool& bOutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, bool bNewValue) const override;
	// ~Super Interface
};

struct FOutputChannelChildrenBrickProperty : FBrickEditorObjectBrickProperty
{
	DECLARE_BRICK_PROP(FOutputChannelChildrenBrickProperty, FBrickEditorObjectBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, TArray<FBrickEditorObjectPtr>& OutObjects) const override;
	virtual bool GetDefaultValue(const FBrickPropertyContainer& Container, TArray<FBrickEditorObjectPtr>& OutObjects, FDisplayInfo& OutDisplayInfo) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, const TArray<FBrickEditorObjectPtr>& NewObjects) const override;
	// ~Super Interface

#define LOCTEXT_NAMESPACE "BrickProperties"

	static auto GetDisplayName()
	{
		return LOCTEXT("OutputChannelChildren", "Controlled Input Channels (Read Only)");
	}

	static auto GetDescription()
	{
		return LOCTEXT("OutputChannelChildrenDesc", "This cannot be modified! Lists other bricks which have an input channel referencing this brick");
	}
#undef LOCTEXT_NAMESPACE
};

struct FFlashSequenceBrickProperty : FClassBrickProperty
{
	DECLARE_BRICK_PROP(FFlashSequenceBrickProperty, FClassBrickProperty);

	// ~Constructor
	FFlashSequenceBrickProperty(const FGetClasses& InDelegate)
		: Super(InDelegate)
	{
	}
};

struct FUGCTagsBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FUGCTagsBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FUGCTags>(A, B);
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override
	{
		auto Value = FUGCTags();
		if (GetValue(Container, Value))
		{
			OutValue = Value.AsText();
			return true;
		}

		return false;
	}

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return false;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, FUGCTags& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, const FUGCTags& NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}
};

DECLARE_BRICK_PROP_TYPE(FUGCTagsBrickProperty, FUGCTags);
