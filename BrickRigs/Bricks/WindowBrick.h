// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/Brick.h"
#include "WindowBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UWindowBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UWindowBrickStaticInfo();
};

UCLASS()
class UWindowBrick : public UBrick
{
	GENERATED_BODY()

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	// ~Super Interface
};
