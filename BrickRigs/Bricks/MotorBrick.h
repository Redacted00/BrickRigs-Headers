// Copyright Fluppisoft, 2016

#pragma once

#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Bricks/AxleBrick.h"
#include "Bricks/FuelConsumerBrick.h"
#include "MotorBrick.generated.h"

class ABrickCharacter;
class USoundCue;

UCLASS(Abstract)
class BRICKRIGS_API UMotorBrickStaticInfo : public UFuelConsumerBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* MotorSoundCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* StartupSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* ShiftSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* BrakeReleaseSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* BackFireSound;

	// Time it takes until the motor is started
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	float StartupTime;

	// Angular acceleration in the first gear
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	float Acceleration;
	// Min (idle) and max RPM
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	FFloatInterval RPMRange;
	// Relative RPM at which the clutch starts to grip and is fully engaged
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	FFloatInterval ClutchRPMRange;
	// RPM ratio to shift up at 0 and 100% throttle
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	FFloatInterval ShiftUpRPMRange;
	// RPM ratio to shift down at 0 and 100% throttle
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	FFloatInterval ShiftDownRPMRange;
	// RPM at which the backfire effect is spawned
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	float BackfireRPM;

	// Time a gear shift takes
	UPROPERTY(EditDefaultsOnly, Category = Transmission)
	float ShiftDelay;
	// Minimum time to wait for between automatic shifts
	UPROPERTY(EditDefaultsOnly, Category = Transmission)
	float MinAutoShiftDelay;
	UPROPERTY(EditDefaultsOnly, Category = Transmission)
	uint8 NumForwardGears;
	UPROPERTY(EditDefaultsOnly, Category = Transmission)
	uint8 NumReverseGears;
	UPROPERTY(EditDefaultsOnly, Category = Transmission)
	float LastGearSpeed;
	// ~Properties

	// ~Constructor
	UMotorBrickStaticInfo();

	// ~Super Interface
	virtual int32 GetMaxInstances(UClass*& OutClass) const override
	{
		OutClass = StaticClass();
		return 1;
	};
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UMotorBrick : public UFuelConsumerBrick
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnExhaustThrottleChanged, float);

	// ~Variables
	// World time when the motor has been started
	float StartupTime;
	// Current RPM of the rotor
	float CurrentRPM;
	// World time when the last shift has started
	float ShiftTime;
	// Whether the current gear has been manually shifted to by the player
	uint8 bManuallyShifted : 1;
	// Whether any throttle was applied last update
	uint8 bWasOnThrottle : 1;
	// Whether the motor is up and running
	uint8 bIsRunning : 1;
	// Last state of the automatic hand brake
	uint8 bLastAutomaticHandBrake : 1;
	// Current throttle input channel value
	float ThrottleInputChannelValue;
	// Current throttle used for exhausts
	float ExhaustThrottle;
	// The cached boost factor given from all connected compressors
	float CompressorBoostFactor;
	// Cached list of connected axles
	UPROPERTY(Transient)
	TArray<UAxleBrick*> ConnectedAxles;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> StartupAC;
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> MotorAC;
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> BackFireAC;
	// ~Components

public:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	FVehicleInputChannel ThrottleInputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	float GearRatioScale;
	UPROPERTY(EditDefaultsOnly, Category = Motor)
	bool bTankDrive;
	// ~Brick Properties

	// ~Delegates
	FOnExhaustThrottleChanged OnExhaustThrottleChangedDelegate;
	FSimpleMulticastDelegate OnBackFireDelegate;
	// ~Delegates

	// ~Constructor
	UMotorBrick();

	// ~Super Interface
	virtual void PostConstructVehicle() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnHasAnyFuelChanged() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Get the seat controlling the motor
	USeatBrick* GetControllingSeat() const;

	// Returns whether the motor supports the given gear
	bool IsValidGear(int32 InGear) const;
	// Returns the min and max allowed gear
	void GetGearRange(FInt32Interval& OutRange) const
	{
		OutRange.Min = -GetStaticInfo<UMotorBrickStaticInfo>()->NumReverseGears;
		OutRange.Max = GetStaticInfo<UMotorBrickStaticInfo>()->NumForwardGears;
	}

	// Get the current motor RPM
	float GetCurrentRPM() const
	{
		return CurrentRPM;
	}

	// Get the maximum motor RPM
	float GetMaxRPM() const
	{
		return GetStaticInfo<UMotorBrickStaticInfo>()->RPMRange.Max;
	}

	// Get the RPM range which the motor should be kept in
	void GetIdealRPMRange(FFloatInterval& OutRange) const
	{
		const auto* StaticInfo = GetStaticInfo<UMotorBrickStaticInfo>();
		OutRange.Min = StaticInfo->RPMRange.Min;
		OutRange.Max = StaticInfo->ShiftUpRPMRange.Min * StaticInfo->RPMRange.Max;
	}

	// Get the current throttle amount applied to exhausts
	float GetExhaustThrottle() const
	{
		return ExhaustThrottle;
	}

private:
	// Whether automatic shifting is enabled
	bool ShouldShiftAutomatically() const;
	// Called to update automatic input
	void HandleAutomaticInput(bool& bOutFlipThrottle);
	// Sets the current RPM value
	void SetCurrentRPM(float NewRPM);
	// Sets the throttle value used by exhausts
	void SetExhaustThrottle(float NewThrottle);
	// Get the time since the last shift has been finished
	float GetTimeSinceShift() const;
	// Whether the motor is currently changing gears
	bool IsShifting() const;
	// Get the currently engaged gear
	int32 GetCurrentGear() const;
	// Get the current clutch engagement ratio (from 0-1)
	float GetClutchRatio(int32 Gear, float Throttle) const;
	// Get the reduction ratio of the given gear, 1 in first gear and 0 in top gear
	float GetGearRatio(int32 InGear) const;
	// Get the RPM the motor would be turning at the given speed
	float GetGearRPM(int32 InGear, float InSpeed) const;
	// Get the speed of the given gear at max RPM
	float GetMaxGearSpeed(int32 InGear) const;
	// Get the speed the vehicle should be travelling in the given gear at the given RPM
	float GetGearSpeed(int32 InGear, float InRPM) const;
	// Callback for the vehicle
	void OnCurrentGearChanged(int32 NewGear, bool bFromManualShift);
	// Whether the motor should currently be running
	bool ShouldMotorBeRunning() const;
	// Turns the motor on or off as needed
	void UpdateIsRunning();
	// Spawns a backfire effect on the exhausts
	void SpawnBackFire();
	// Updates the cached list of axles and compressor boost
	void UpdateConnectedAxlesAndCompressors();
	// Callback for the vehicle
	void OnPassengerEnteredOrLeftVehicle(ABrickCharacter* InCharacter, bool bEntered);
};
