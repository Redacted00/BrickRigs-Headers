// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Brick.h"
#include "PropellerBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UPropellerBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Radius of the propeller blades
	UPROPERTY(EditDefaultsOnly, Category = Propeller)
	float PropellerRadius;
	// Multiplied with the angular speed to determine the thrust force
	UPROPERTY(EditDefaultsOnly, Category = Propeller)
	float Thrust;
	// ~Properties

	// ~Constructor
	UPropellerBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UPropellerBrick : public UBrick
{
	GENERATED_BODY()

public:
	// ~Constructor
	UPropellerBrick();

	// ~Super Interface
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void TickBrick(float DeltaTime) override;
	// ~Super Interface
};
