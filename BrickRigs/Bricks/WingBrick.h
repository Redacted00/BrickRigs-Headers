// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Brick.h"
#include "WingBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UWingBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UWingBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UWingBrick : public UBrick
{
	GENERATED_BODY()

public:
	// ~Constructor
	UWingBrick();
};
