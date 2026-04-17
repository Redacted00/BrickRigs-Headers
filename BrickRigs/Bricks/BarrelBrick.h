// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/Brick.h"
#include "BarrelBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UBarrelBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UBarrelBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBarrelBrick : public UBrick
{
	GENERATED_BODY()
};
