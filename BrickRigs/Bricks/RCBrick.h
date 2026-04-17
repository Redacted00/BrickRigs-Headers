// Copyright Fluppisoft, 2017

#pragma once

#include "ScalableBrick.h"
#include "RCBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API URCBrickStaticInfo : public UScalableBrickStaticInfo
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
class BRICKRIGS_API URCBrick : public UScalableBrick
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;

	virtual bool IsRCBrick() const override
	{
		return true;
	}

	// ~Super Interface
};
