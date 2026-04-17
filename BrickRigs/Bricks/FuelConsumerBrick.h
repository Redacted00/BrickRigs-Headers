// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/Brick.h"
#include "FuelConsumerBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UFuelConsumerBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Fuel)
	FFuelTankParams FuelTankParams;
	// Amount of fuel in liters to consume per second
	UPROPERTY(EditDefaultsOnly, Category = Fuel)
	float FuelConsumption;
	// ~Properties

	// ~Constructor
	UFuelConsumerBrickStaticInfo();

	// ~Super Interface
	virtual FFuelTankParams GetFuelTankParams() const override;
	// ~Super Interface
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UFuelConsumerBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	FFuelTankRuntimeParams FuelTankRuntimeParams;
	// ~Variables

public:
	// ~Super Interface
	virtual const FFuelTankRuntimeParams* GetFuelTankRuntimeParams() const override;
	// ~Super Interface
};
