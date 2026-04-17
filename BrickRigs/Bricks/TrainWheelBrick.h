// Copyright Fluppisoft, 2017

#pragma once

#include "Bricks/WheelBrick.h"
#include "TrainWheelBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UTrainWheelBrickStaticInfo : public UWheelBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UTrainWheelBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UTrainWheelBrick : public UWheelBrick
{
	GENERATED_BODY()

public:
	// ~Constructor
	UTrainWheelBrick();
};
