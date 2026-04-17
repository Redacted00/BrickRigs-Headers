// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/TankBrick.h"
#include "FuelConsumerBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UFuelConsumerBrickStaticInfo : public UTankBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The amount of fuel in liters to consume per second
	UPROPERTY(EditDefaultsOnly, Category = Fuel)
	float FuelConsumption;
	// ~Properties

	// ~Constructor
	UFuelConsumerBrickStaticInfo();
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UFuelConsumerBrick : public UTankBrickBase
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual UClass* GetExplosiveMaterial() const override;
	// ~Super Interface

	// Consumes the given amount of fuel instantly
	bool ConsumeFuelInstant(float Amount);

	// Consumes fuel at the given rate
	bool ConsumeFuelRate(float Rate, float DeltaTime);
};
