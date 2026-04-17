#pragma once

#include "Bricks/Misc/BrickUnits.h"
#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FBrickUnitsBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FBrickUnitsBrickProperty);

private:
	// ~Variables
	TBrickPropAttribute<FBrickUnits> MinValue;
	TBrickPropAttribute<FBrickUnits> MaxValue;
	// ~Variables

public:
	// ~Constructor
	FBrickUnitsBrickProperty(const TBrickPropAttribute<FBrickUnits>& MinValue, const TBrickPropAttribute<FBrickUnits>& MaxValue)
		: MinValue(MinValue), MaxValue(MaxValue)
	{
	}

	// ~Super Interface
	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override
	{
		auto& BrickUnits = *GetValuePtr<FBrickUnits>(Container);
		return BrickUnits.SerializeInternal(Ar, Version < BR_SAVE_BRICK_UNITS_FLOAT_VERSION);
	}

	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FBrickUnits>(A, B);
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FBrickUnitsBrickProperty, FBrickUnits>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FBrickUnitsBrickProperty, FBrickUnits>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FBrickUnits& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FBrickUnits NewValue) const
	{
		return SetValueInternal(Container, FMath::Clamp(NewValue, MinValue.Get(Container, FBrickUnits::Zero()), MaxValue.Get(Container, FBrickUnits::Zero())));
	}

	void GetValueRange(const FBrickPropertyContainer& Container, FBrickUnits& OutMin, FBrickUnits& OutMax) const
	{
		OutMin = MinValue.Get(Container, FBrickUnits::Zero());
		OutMax = MaxValue.Get(Container, FBrickUnits::Zero());
	}
};

DECLARE_BRICK_PROP_TYPE(FBrickUnitsBrickProperty, FBrickUnits);
