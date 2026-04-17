// Copyright Fluppisoft, 2018

#pragma once

#include "ScalableBrick.h"
#include "SensorBrickBase.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API USensorBrickBaseStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	USensorBrickBaseStaticInfo();
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API USensorBrickBase : public UScalableBrick
{
	GENERATED_BODY()

public:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, Category = Input)
	FSensorOutputChannel OutputChannel;
	// ~Brick Properties

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PreRepairBrick(const FTransform& SpawnTransform) override;

	virtual bool HasAnyOutputChannel() const override
	{
		return true;
	}

	virtual FVehicleOutputChannel* GetOutputChannel() const override
	{
		return const_cast<FSensorOutputChannel*>(&OutputChannel);
	}

	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual bool ResolveRemovedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	// ~Super Interface
};
