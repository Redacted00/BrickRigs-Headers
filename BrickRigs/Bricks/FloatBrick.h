// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Bricks/ScalableBrick.h"
#include "FloatBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UFloatBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UFloatBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UFloatBrick : public UScalableBrick
{
	GENERATED_BODY()

private:
	// ~Variables
	uint8 NumPontoonsX : 2;
	uint8 NumPontoonsY : 2;
	uint8 NumPontoonsZ : 2;
	// Bounds size of every pontoon
	FVector PontoonBoundsSize;
	// ~Variables

public:
	// ~Super Interface
	virtual void OnUpdatePartRoot(FOnUpdatePartRootParams& Params) override;
	virtual void GetFluidDynamicElements(FFluidDynamicElements& OutElements, const FTransform& Transform) const override;

	virtual bool ShouldShowGenerateLiftProperty() const override
	{
		return false;
	}

	// ~Super Interface

private:
	// Initializes pontoon data if needed
	void InitializePontoons();
};
