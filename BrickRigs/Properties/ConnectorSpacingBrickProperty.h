#pragma once

#include "Bricks/Misc/ScalableBrickConnectorSpacing.h"
#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FConnectorSpacingBrickProperty : public FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FConnectorSpacingBrickProperty);

public:
	// ~Constructor
	FConnectorSpacingBrickProperty()
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FScalableBrickConnectorSpacing>(A, B);
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FConnectorSpacingBrickProperty, FScalableBrickConnectorSpacing>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FConnectorSpacingBrickProperty, FScalableBrickConnectorSpacing>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FScalableBrickConnectorSpacing& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FScalableBrickConnectorSpacing NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}
};

DECLARE_BRICK_PROP_TYPE(FConnectorSpacingBrickProperty, FScalableBrickConnectorSpacing);
