// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Brick.h"
#include "CompressorBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UCompressorBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Compressor)
	float BoostFactor;
	// ~Properties

	// ~Constructor
	UCompressorBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UCompressorBrick : public UBrick
{
	GENERATED_BODY()

public:
	// ~Constructor
	UCompressorBrick();

	// Get the effective boost factor of this compressor
	float GetBoostFactor() const;
};
