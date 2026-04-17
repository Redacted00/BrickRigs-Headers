#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FBoolBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FBoolBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<bool>(A, B);
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override;

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return SetValue(Container, FCString::ToBool(*NewValue.ToString()));
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto bValue = false;
		GetValue(Container, bValue);
		return LexToString(bValue);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		auto bNewValue = false;
		LexFromString(bNewValue, Buffer);
		return SetValue(Container, bNewValue);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, bool& bOutValue) const
	{
		return GetValueInternal(Container, bOutValue);
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, bool bNewValue) const
	{
		return SetValueInternal(Container, bNewValue);
	}
};

DECLARE_BRICK_PROP_TYPE(FBoolBrickProperty, bool);
