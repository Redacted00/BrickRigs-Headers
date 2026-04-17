// Copyright Fluppisoft, 2015

#pragma once

#include "Bricks/Brick.h"
#include "AxleBrick.generated.h"

class UBrickEditorArrowComponent;

struct FAxleBrickEditorParams : public FBrickEditorParams
{
	// Arrow components used to visualize the steering angle and drive direction
	TArray<TBrickEditorComponentPtr<UBrickEditorArrowComponent>> ArrowComponents;
};

UCLASS(Abstract)
class BRICKRIGS_API UAxleBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float SuspensionStiffness;
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float SuspensionDamping;
	// ~Properties

	// ~Constructor
	UAxleBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UAxleBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// List of all wheel connections involving this axle
	UPROPERTY(Transient)
	TArray<UWheelConnection*> WheelConnections;
	// Cached input channel values
	float BrakeInput;
	float SteeringInput;
	bool bDisableSteeringInput;
	bool bInvertSteeringInput;
	bool bHandBrakeInput;
	// ~Variables

public:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	FVehicleInputChannel BrakeInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	FVehicleInputChannel SteeringInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float SuspensionLength;
	// Factor for suspension stiffness
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float SuspensionStiffness;
	// Factor for suspension damping
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float SuspensionDamping;
	// Whether this axle receives torque
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	bool bDriven;
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	bool bInvertDrive;
	// Whether this axle has brakes
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	bool bHasBrake;
	// Whether this axle is affected by handbrake
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	bool bHasHandBrake;
	UPROPERTY(EditDefaultsOnly, Category = Axle)
	float BrakeStrength;
	// Angle (in degrees) the axle can steer in either direction
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	float SteeringAngle;
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	float SteeringSpeed;
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	bool bCanDisableSteering;
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	bool bCanInvertSteering;
	// ~Brick Properties

	// ~Constructor
	UAxleBrick(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UpdateEditorVisualization() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual UBrickConnection* CreateCustomBrickConnection(UBrick* OtherBrick, const FName& ConnectionName, FBrickConnectionParams& Params) const override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* InConnection) override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeUnique<FAxleBrickEditorParams>();
	}

	// ~Super Interface

	// Allows the axle to modify the vehicle bounds, in order to take the suspension position length into account
	void AdjustVehicleBounds(const FVector& BoundsMin, const FVector& BoundsMax, FVector& OutBoundsMin, FVector& OutBoundsMax) const;
	// Get the desired suspension length
	float GetSuspensionLength() const;
	// Get the effective suspension stiffness
	float GetSuspensionStiffness() const;
	// Get the effective suspension damping
	float GetSuspensionDamping() const;
	// Whether this axle is driven
	bool IsDriven() const
	{
		return bDriven;
	}

	// Get whether the drive direction should be inverted
	bool IsDriveInverted() const
	{
		return bInvertDrive;
	}

	// Whether this axle is affected by the hand brake
	bool HasHandBrake() const
	{
		return bHasHandBrake;
	}

	// Whether this axle has a steering angle
	bool HasSteering() const
	{
		return !FMath::IsNearlyZero(SteeringAngle);
	}

	// Get the current steering input
	float GetSteeringInput() const
	{
		return SteeringInput;
	}

	// Get the maximum steering angle
	float GetMaxSteeringAngle() const
	{
		return SteeringAngle;
	}

	// Allows the motor to access the cached array of wheel connections
	const TArray<UWheelConnection*>& GetWheelConnections() const
	{
		return WheelConnections;
	}
};
