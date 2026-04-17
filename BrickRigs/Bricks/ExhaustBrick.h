// Copyright Fluppisoft, 2017

#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "Bricks/Brick.h"
#include "ExhaustBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UExhaustBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Exhaust)
	UParticleSystem* SmokeEmitter;
	UPROPERTY(EditDefaultsOnly, Category = Exhaust)
	UParticleSystem* BackFireEmitter;
	// ~Properties

	// ~Constructor
	UExhaustBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UExhaustBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// Currently applied throttle
	float CurrentThrottle;
	// Motor that controls this exhaust
	UPROPERTY(Transient)
	UMotorBrick* ConnectedMotor;
	// Current input channel value
	float InputChannelValue;
	// ~Variables

	// ~Components
	// Particle system used for the exhaust smoke
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> SmokePSC;
	// Particle system used for the back fire effect
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> BackFirePSC;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, Category = Exhaust)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Exhaust)
	float SpawnScale;
	UPROPERTY(EditAnywhere, Category = Exhaust)
	FBrickColor SmokeColor;
	// ~Brick Properties

public:
	// ~Constructor
	UExhaustBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnSelectionStateChanged(EBrickSelectionState NewState) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~Super Interface

private:
	// Return whether this exahust is updated from the motor throttle
	bool IsDrivenByMotor() const;
	// Updates the controlling motors
	void UpdateMotors(bool bIsUninitializing = false);
	// Callbacks for the motor
	void SpawnBackFire();
	void OnMotorExhaustThrottleChanged(float NewThrottle);
	// Updates the current throttle and particle system
	void UpdateExhaustThrottle();
	// Creates or destroys and updates the particle system
	void UpdateParticleSystem();
	// Updates the particle system color
	void UpdateSmokeColor();
};
