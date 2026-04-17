// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "TrackWheelBrick.h"
#include "IdlerWheelBrick.generated.h"

class USprocketWheelBrick;

UCLASS(Abstract)
class BRICKRIGS_API UIdlerWheelBrickStaticInfo : public UTrackWheelBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UIdlerWheelBrickStaticInfo();
};

/**
 * Non-driven wheels of a track assembly
 */
UCLASS()
class BRICKRIGS_API UIdlerWheelBrick : public UTrackWheelBrick
{
	GENERATED_BODY()

	// ~Variables
	// Sprockets currently connected to this wheel
	TArray<TWeakObjectPtr<USprocketWheelBrick>> ConnectedSprockets;
	// ~Variables

public:
	// ~Super Interface
	virtual USprocketWheelBrick* GetSprocketWheel() const override;
	// ~Super Interface

	// Connects or disconnects a sprocket from and to the wheel
	void AddConnectedSprocket(USprocketWheelBrick* Sprocket);
	void RemoveConnectedSprocket(USprocketWheelBrick* Sprocket);
};
