#pragma once

#include "BrickProperty.h"

struct FDateTimeBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FDateTimeBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FDateTime>(A, B);
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override
	{
		auto Value = FDateTime();
		if (GetValue(Container, Value))
		{
			OutValue = FText::AsDate(Value);
			return true;
		}

		return false;
	}

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		// NOTE: There is no default function to convert an FText to an FDateTime
		return false;
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FDateTimeBrickProperty, FDateTime>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FDateTimeBrickProperty, FDateTime>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FDateTime& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FDateTime NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}
};

DECLARE_BRICK_PROP_TYPE(FDateTimeBrickProperty, FDateTime);
