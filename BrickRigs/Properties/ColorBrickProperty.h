#pragma once

#include "Misc/BrickColor.h"
#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FColorBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FColorBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return HasAlpha() ? CompareInternal<FBrickColorWithAlpha>(A, B) : CompareInternal<FBrickColor>(A, B);
	}

	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FColorBrickProperty, FBrickColorWithAlpha>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FColorBrickProperty, FBrickColorWithAlpha>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FBrickColorWithAlpha& OutValue) const
	{
		if (HasAlpha())
		{
			return GetValueInternal(Container, OutValue);
		}
		FBrickColor ColorWithoutAlpha;
		if (GetValueInternal(Container, ColorWithoutAlpha))
		{
			OutValue.SetHVS(ColorWithoutAlpha);
			return true;
		}

		return false;
	}

	bool SetValue(const FBrickPropertyContainer& Container, FBrickColorWithAlpha NewValue) const
	{
		if (HasAlpha())
		{
			return SetValueInternal(Container, NewValue);
		}
		const FBrickColor NewColorWithoutAlpha = NewValue;
		return SetValueInternal(Container, NewColorWithoutAlpha);
	}

	bool HasAlpha() const
	{
		const FStructProperty* StructProp = CastFieldChecked<FStructProperty>(Property);
		return StructProp->Struct == FBrickColorWithAlpha::StaticStruct();
	}
};

DECLARE_BRICK_PROP_TYPE(FColorBrickProperty, FBrickColor);
DECLARE_BRICK_PROP_TYPE(FColorBrickProperty, FBrickColorWithAlpha);
