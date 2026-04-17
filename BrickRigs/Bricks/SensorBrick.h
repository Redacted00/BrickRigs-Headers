// Copyright Fluppisoft, 2018

#pragma once

#include "Bricks/SensorBrickBase.h"
#include "SensorBrick.generated.h"

class UBrickEditorArrowComponent;

UENUM()
enum struct ESensorType : uint8
{
	Speed,
	Acceleration,
	Distance,
	Time,
	Proximity,
	DistanceToGround,
	Altitude,
	Pitch,
	Yaw,
	Roll,
	AngularSpeed,
	NormalSpeed,
	NormalAcceleration,
	NormalAngularSpeed,
	NumSeekingProjectiles,
	SeekingProjectileDistance
};

UENUM()
enum struct EProximitySensorMask : uint8
{
	All,
	Vehicles,
	OtherVehicles,
	Pawn,
	Static,
	Water
};

struct FSensorBrickEditorParams : FBrickEditorParams
{
	// Shows the proximity sensor range
	TBrickEditorComponentPtr<UBrickEditorArrowComponent> ArrowComponent;
};

UCLASS(Abstract)
class BRICKRIGS_API USensorBrickStaticInfo : public USensorBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	USensorBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API USensorBrick : public USensorBrickBase
{
	GENERATED_BODY()

	// ~Variables
	// Current value of the enabled input channel
	uint8 bEnabledInputValue : 1;
	// Whether the sensor is currently enabled and updating
	uint8 bIsSensorActive : 1;
	// Whether the sensor value needs to be updated at least once
	uint8 bNeedsInitialSensorUpdate : 1;
	// The world time the sensor has been activated
	float ActivationTime;
	// Used to measure the relative movement between ticks or the absolute movement since activation
	FVector LastLocation;
	FQuat LastRotation;
	FVector LastVelocity;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	FVehicleInputChannel EnabledInputChannel;
	UPROPERTY(EditAnywhere, Category = Actuator)
	ESensorType SensorType;
	UPROPERTY(EditAnywhere, Category = Actuator)
	EProximitySensorMask TraceMask;
	// Whether the sensor value should return to 0 after being disabled
	UPROPERTY(EditAnywhere, Category = Actuator)
	bool bReturnToZero;
	// ~Brick Properties

public:
	// ~Constructor
	USensorBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UpdateEditorVisualization() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeUnique<FSensorBrickEditorParams>();
	}

	// ~Super Interface

private:
	// Whether the sensor should currently be active
	bool ShouldSensorBeActive() const
	{
		return bEnabledInputValue && IsControllable();
	}

	// Used to activate or deactiate the sensor
	void UpdateSensorActive(bool bReset = false);
	// Sets the active flag
	void SetSensorActive(bool bNewActive, bool bReset = false);
	// Sets the activation time to the current time
	void ResetActivationTime();
	// Get the current input of the sensor
	float UpdateSensorInput(float DeltaTime);
	// Returns the ray parameters for sensor traces
	bool GetSensorTraceRay(const FTransform& WorldTransform, FVector& OutStart, FVector& OutDir) const;
	// Returns the minimum trace distance
	float GetMinSensorTraceDistance() const
	{
		// Convert from meters to centimeters
		return FMath::Min(OutputChannel.MinIn, OutputChannel.MaxIn) * 100.f;
	}

	// Returns the trace distance to use
	float GetMaxSensorTraceDistance() const
	{
		// Convert from meters to centimeters
		return FMath::Max(OutputChannel.MinIn, OutputChannel.MaxIn) * 100.f;
	}

	// Executes the sensor trace, returns the distance
	float DoSensorTrace() const;

	// Property callbacks
	static void GetSensorTypeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetTraceMaskItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
