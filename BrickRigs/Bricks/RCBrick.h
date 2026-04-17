// Copyright Fluppisoft, 2017

#pragma once

#include "Bricks/Brick.h"
#include "RCBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API URCBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	URCBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API URCBrick : public UBrick
{
	GENERATED_BODY()

public:
	// ~Constructor
	URCBrick();

	// ~Super Interface
	virtual bool IsRCBrick() const override
	{
		return true;
	}

	// ~Super Interface
};
