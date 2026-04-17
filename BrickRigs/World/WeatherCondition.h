// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeatherCondition.generated.h"

UENUM(BlueprintType)
enum class EPrecipitationType : uint8
{
	None,
	Rain,
	Snow,
	Max UMETA(Hidden)
};

/**
 * Defines all parameters to simulate any weather condition
 */
USTRUCT(BlueprintType)
struct FWeatherConditionParams
{
	GENERATED_BODY()

	// NOTE: Defaults should be set up to work in space

	// Sound played in the background
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> AmbientSound;
	// Temperature in °C
	UPROPERTY(EditAnywhere)
	float Temperature = 20.f;
	// The average wind speed in cm/s
	UPROPERTY(EditAnywhere)
	float WindSpeed = 0.f;
	// The maximum relative variance in speed
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f))
	float WindSpeedVariance = 0.75f;
	// The maximum random deviation from the wind direction
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 180.f))
	float WindDirectionVariance = 20.f;
	// How often the wind direction and speed is changed, 1 means every second
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f))
	float WindPeriod = 5.f;
	// 0 means no clouds, 1 means closed cloud layer
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float CloudDensity = 0.f;
	// Height of the clouds above the ground
	UPROPERTY(EditAnywhere)
	float CloudHeight = 100000.f;
	// Maximum fog opacity, 0 to turn it off
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float MaxFogOpacity = 0.f;
	// The type of precipitation, i.e. rain, snow etc.
	UPROPERTY(EditAnywhere)
	EPrecipitationType PrecipitationType = EPrecipitationType::None;
	// How intensely it is ranining, snowing etc.
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PrecipitationIntensity = 1.f;
	// How much of the ground is covered in snow
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float SnowCoverage = 0.f;
	// How much of the ground is covered in rain
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float RainCoverage = 0.f;
	// The average delay between lightnings, 0 means lightning are turned off
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f))
	float LightningFrequency = 0.f;
	// Minimum pause between lightnings
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f))
	float MinLightningDelay = 1.5f;
};

/**
 * Preset for a weather condition
 */
UCLASS()
class BRICKRIGS_API UWeatherCondition : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

protected:
	// ~Properties
	// Name and icon for this asset
	UPROPERTY(EditAnywhere)
	FDisplayInfo DisplayInfo;

public:
	UPROPERTY(EditAnywhere)
	FWeatherConditionParams Weather;
	// ~Properties

	// ~Constructor
	UWeatherCondition()
	{
		DisplayInfo.IconAtlas = EBrickUIIconAtlas::WeatherIcons;
	}

	// Used for sorting weather conditions for displaying
	bool SortWeatherCondition(const UWeatherCondition* Other) const;

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return DisplayInfo;
	}

	// ~IObjectPropertyItemInterface
};
