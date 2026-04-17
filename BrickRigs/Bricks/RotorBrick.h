// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Brick.h"
#include "RotorBrick.generated.h"


UCLASS(Abstract)
class BRICKRIGS_API URotorBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	URotorBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API URotorBrick : public UBrick
{
	GENERATED_BODY()

private:
	// ~Variables
	// Numbers of blades attached to the part
	int32 NumBlades;
	// The cached lift factor of the rotor
	float LiftFactor;
	// ~Variables

public:
	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;
	// ~Super Interface

	// Returns the total lift factor of the rotor
	auto GetLiftFactor() const
	{
		return LiftFactor;
	}

private:
	// Returns the number of blades currently connected to the rotor
	int32 GetNumConnectedBlades();
};
