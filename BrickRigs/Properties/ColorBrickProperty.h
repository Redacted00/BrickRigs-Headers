#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FColorBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FColorBrickProperty);

private:
	// ~Variables
	TBrickPropAttribute<bool> bHasAlphaChannel;
	// ~Variables

public:
	// ~Constructor
	explicit FColorBrickProperty(const TBrickPropAttribute<bool>& bHasAlphaChannel = true)
		: bHasAlphaChannel(bHasAlphaChannel)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FColor>(A, B);
	}

	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = FColor();
		GetValue(Container, Value);
		return Value.ToString();
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		auto NewValue = FColor();
		return NewValue.InitFromString(Buffer) && SetValue(Container, NewValue);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FColor& OutValue) const
	{
		if (GetValueInternal(Container, OutValue))
		{
			// Always set the alpha channel to full if it's not used
			OutValue.A = HasAlphaChannel(Container) ? OutValue.A : 255;
			return true;
		}

		return false;
	}

	bool SetValue(const FBrickPropertyContainer& Container, FColor NewValue) const
	{
		// Don't allow changing the alpha channel if it's not used
		NewValue.A = HasAlphaChannel(Container) ? NewValue.A : 255;
		return SetValueInternal(Container, NewValue);
	}

	bool HasAlphaChannel(const FBrickPropertyContainer& Container) const
	{
		return bHasAlphaChannel.Get(Container).Get(false);
	}
};

DECLARE_BRICK_PROP_TYPE(FColorBrickProperty, FColor);
