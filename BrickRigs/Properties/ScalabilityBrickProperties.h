#pragma once

#include "NumericBrickProperty.h"
#include "BoolBrickProperty.h"
#include "EnumBrickProperty.h"
#include "BrickProperty.h"
#include "CoreMinimal.h"

struct FScalabilityBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FScalabilityBrickProperty, FEnumBrickPropertyBase);

	// ~Super Interface
	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override;
	// ~Super Interface
};

struct FOverallQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FOverallQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FResolutionScaleBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FResolutionScaleBrickProperty, FScalabilityBrickProperty);

private:
	// ~Variables
	TArray<int32> ResolutionScaleOptions;
	// ~Variables

public:
	// ~Constructor
	FResolutionScaleBrickProperty()
	{
		// Copied over from scalability .cpp, we use the same config array so this setting lines up with the overall scalability setting
		TArray<FString> ResolutionValueStrings;
		GConfig->GetSingleLineArray(TEXT("ScalabilitySettings"), TEXT("PerfIndexValues_ResolutionQuality"), ResolutionValueStrings, GScalabilityIni);

		ResolutionScaleOptions.Reserve(ResolutionValueStrings.Num());
		for (auto i = 0; i < ResolutionValueStrings.Num(); ++i)
		{
			ResolutionScaleOptions.Add(FCString::Atof(*ResolutionValueStrings[i]));
		}
	}

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FAAQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FAAQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FViewDistanceQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FViewDistanceQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FPostProcessingQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FPostProcessingQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FShadowQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FShadowQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FTextureQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FTextureQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FEffectsQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FEffectsQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FFoliageQualityBrickProperty : FScalabilityBrickProperty
{
	DECLARE_BRICK_PROP(FFoliageQualityBrickProperty, FScalabilityBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	// ~Super Interface
};

struct FResolutionBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FResolutionBrickProperty, FEnumBrickPropertyBase);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override;
	// ~Super Interface

private:
	// Returns the selectable screen resolutions
	void GetSupportedResolutions(TArray<FIntPoint>& OutResolutions) const;
};

struct FWindowModeBrickProperty : FEnumBrickPropertyBase
{
	DECLARE_BRICK_PROP(FWindowModeBrickProperty, FEnumBrickPropertyBase);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, int32& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, int32 NewValue) const override;
	virtual void GetEnumItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems) const override;
	// ~Super Interface
};

struct FVSyncBrickProperty : FBoolBrickProperty
{
	DECLARE_BRICK_PROP(FVSyncBrickProperty, FBoolBrickProperty);

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, bool& bOutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, bool bNewValue) const override;
	// ~Super Interface
};

struct FFrameRateLimitBrickProperty : FNumericBrickPropertyBase
{
	DECLARE_BRICK_PROP(FFrameRateLimitBickProperty, FNumericBrickPropertyBase);

	// ~Constructor
	FFrameRateLimitBrickProperty(const FNumericBrickPropertyRange& ValueRange)
		: FNumericBrickPropertyBase(ENumericValueType::Integer, ValueRange)
	{
	}

	// ~Super Interface
	virtual bool GetValue(const FBrickPropertyContainer& Container, FNumericBrickPropertyValue& OutValue) const override;
	virtual bool SetValue(const FBrickPropertyContainer& Container, const FNumericBrickPropertyValue& NewValue) const override;
	// ~Super Interface
};
