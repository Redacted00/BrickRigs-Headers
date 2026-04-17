// Copyright Fluppisoft, 2016

#pragma once

#include "ScalableBrick.h"
#include "World/ExplosiveMaterial.h"
#include "TankBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UTankBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = FuelTank)
	FFuelTankParams FuelTankParams;
	// ~Properties

	// ~Constructor
	UTankBrickStaticInfo();

	// ~Super Interface
	virtual FFuelTankParams GetFuelTankParams() const override;
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UTankBrick : public UScalableBrick
{
	GENERATED_BODY()

	// ~Variables
	FFuelTankRuntimeParams FuelTankRuntimeParams;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, Category = Tank)
	TSubclassOf<UExplosiveMaterial> FuelType;
	// ~Brick Properties

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual const FFuelTankRuntimeParams* GetFuelTankRuntimeParams() const override;
	virtual TSubclassOf<UExplosiveMaterial> GetFuelType() const override;
	// ~Super Interface
};
