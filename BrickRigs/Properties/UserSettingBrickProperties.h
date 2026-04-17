#pragma once

#include "NumericBrickProperty.h"
#include "EnumBrickProperty.h"

struct FLanguageBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FLanguageBrickProperty, FEnumBrickPropertyBase);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override;
	// ~Super Interface

private:
	// Returns an array of localized culture identifiers
	TArray<FString> GetLocalizedCultures() const;
	// Get the name to display for the given culture
	FText GetCultureDisplayName(const FString& InCulture) const;
};

struct FApplicationScaleBrickProperty : FNumericBrickPropertyBase
{
	DECLARE_BRICK_PROP(FApplicationScaleBrickProperty, FNumericBrickPropertyBase);

	// ~Constructor
	FApplicationScaleBrickProperty(const FFloatInterval& InValueRange)
		: FNumericBrickPropertyBase(InValueRange, FGetValueRange(), ENumericValueType::Percent)
	{
	}

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, float& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, float NewValue) const override;
	// ~Super Interface
};
