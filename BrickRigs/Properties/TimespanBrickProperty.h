#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"

struct FTimespanBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FTimespanBrickProperty);

private:
	// ~Variables
	// Min and max timespan that can be selected
	FTimespan MinValue;
	FTimespan MaxValue;
	// Whether infinite can be selected
	bool bAllowInfinite;
	// ~Variables

public:
	// ~Constructor
	FTimespanBrickProperty(const FTimespan& MinValue = FTimespan::Zero(), const FTimespan& MaxValue = FTimespan::MaxValue(), bool bAllowInfinite = true)
		: MinValue(MinValue), MaxValue(MaxValue), bAllowInfinite(bAllowInfinite)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FTimespan>(A, B);
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override;

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return ExportPropertyInternal<FTimespanBrickProperty, FTimespan>(Container);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return ImportPropertyInternal<FTimespanBrickProperty, FTimespan>(Container, Buffer);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FTimespan& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FTimespan NewValue) const
	{
		NewValue = bAllowInfinite && NewValue == FTimespan::MaxValue() ? NewValue : FMath::Clamp(NewValue, MinValue, MaxValue);
		return SetValueInternal(Container, NewValue);
	}

	const auto& GetMinValue() const
	{
		return MinValue;
	}

	const auto& GetMaxValue() const
	{
		return MaxValue;
	}

	auto GetAllowInfinite() const
	{
		return bAllowInfinite;
	}
};

DECLARE_BRICK_PROP_TYPE(FTimespanBrickProperty, FTimespan);
