// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/FuelConsumerBrick.h"
#include "TurbineBrick.generated.h"

class UBrickEditorAudioComponent;
class USoundCue;

UCLASS(Abstract)
class BRICKRIGS_API UTurbineBrickStaticInfo : public UFuelConsumerBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	float MaxRPM;
	// Determines how fast the rotor accelerates
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	float Acceleration;
	// Maximum speed the turbine can reach along the thrust axis
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	float MaxVerticalSpeed;
	// Acceleration along the vertical axis
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	float VerticalAcceleration;
	// Roll, pitch and yaw rotation speeds at full input values
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	FVector MaxAngularVelocity;
	// How fast the angular velocity can be adjusted
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	FVector AngularAcceleration;
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	float ThrottleInputInterpRate;
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	FRotator RotationInputInterpRate;
	UPROPERTY(EditDefaultsOnly, Category = Turbine)
	USoundCue* TurbineSoundCue;
	// Max angle used when auto hover is enabled
	UPROPERTY(EditDefaultsOnly, Category = AutoHover)
	float MaxBankAngle;
	// Relative speed at which the full bank angle should be used
	UPROPERTY(EditDefaultsOnly, Category = AutoHover)
	float AutoHoverMaxBankAngleSpeed;
	// ~Properties

	// ~Constructor
	UTurbineBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UTurbineBrick : public UFuelConsumerBrick
{
	GENERATED_BODY()

	// ~Variables
	// Turbine connection instance
	TWeakObjectPtr<UTurbineConnection> TurbineConnection;
	FRotator CurrentRotationInput;
	// Current interpolated throttle and rotation input values
	float CurrentThrottleInput;
	// Current input channel values
	float PitchInputChannelValue;
	float RollInputChannelValue;
	float YawInputChannelValue;
	float ThrottleInputChannelValue;
	bool bPowerInputChannelValue;
	bool bAutoHoverInputChannelValue;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> AC;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel PowerInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel PitchInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel RollInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel YawInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel ThrottleInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Input)
	FVehicleInputChannel AutoHoverInputChannel;
	// ~Brick properties

public:
	// ~Constructor
	UTurbineBrick();

	// ~Super Interface
	virtual void PostConstructVehicle() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual UBrickConnection* CreateCustomBrickConnection(UBrick* OtherBrick, const FName& ConnectionName, FBrickConnectionParams& Params) const override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* Connection) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Get the current rotor speed
	float GetCurrentRPM() const;
};
