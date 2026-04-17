#pragma once

#include "Bricks/Misc/BrickUnits.h"
#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FBrickSizeBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FBrickSizeBrickProperty);

private:
	// ~Variables
	FBrickSize MinSize;
	FBrickSize MaxSize;
	// ~Variables

public:
	// ~Constructor
	FBrickSizeBrickProperty(const FBrickSize& InMinSize, const FBrickSize& InMaxSize)
		: MinSize(InMinSize), MaxSize(InMaxSize)
	{
	}

	// ~Super Interface
	virtual bool SerializeProperty(FArchive& Ar, const FBrickPropertyContainer& Container, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override
	{
		auto& BrickSize = *GetValuePtr<FBrickSize>(Container);
		const auto bOutdated = Version < BR_SAVE_BRICK_UNITS_FLOAT_VERSION;
		BrickSize.X.SerializeInternal(Ar, bOutdated);
		BrickSize.Y.SerializeInternal(Ar, bOutdated);
		BrickSize.Z.SerializeInternal(Ar, bOutdated);
		return true;
	}

	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FBrickSize>(A, B);
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FBrickSizeBrickProperty, FBrickSize>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FBrickSizeBrickProperty, FBrickSize>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FBrickSize& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FBrickSize NewValue) const
	{
		// Clamp the value
		for (auto i = 0; i < 3; ++i)
		{
			NewValue[i] = FMath::Clamp(NewValue[i], MinSize[i], MaxSize[i]);
		}
		return SetValueInternal(Container, NewValue);
	}

	void GetValueRange(const FBrickPropertyContainer& Container, FBrickSize& OutMin, FBrickSize& OutMax) const
	{
		OutMin = MinSize;
		OutMax = MaxSize;
	}
};

DECLARE_BRICK_PROP_TYPE(FBrickSizeBrickProperty, FBrickSize);
