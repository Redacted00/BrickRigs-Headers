// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "WheelBrick.h"
#include "TrackWheelBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UTrackWheelBrickStaticInfo : public UWheelBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UTrackWheelBrickStaticInfo();
};

/**
 * Base class for idler and sprocket wheels
 */
UCLASS(Abstract)
class BRICKRIGS_API UTrackWheelBrick : public UWheelBrick
{
	GENERATED_BODY()

	// ~Super Interface
	virtual UBrickPhysicalMaterial* GetBrickPhysMaterial(bool bAllowOverrideMaterial) const override;
	virtual float GetBodySetupTargetRadius() const override;
	// ~Super Interface

protected:
	// To be called when the track was connected or disconnected
	void OnConnectedSprocketChanged();

private:
	// Helper to get the track thickness from the connected sprocket
	float GetConnectedTrackThickness() const;
};
