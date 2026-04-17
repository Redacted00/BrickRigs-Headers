// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "Brick.h"
#include "GunBrick.h"
#include "FlareBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UFlareBrickStaticInfo : public UGunBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UFlareBrickStaticInfo();
};
