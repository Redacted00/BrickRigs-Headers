// Copyright Fluppisoft, 2018

#pragma once

#include "CoreMinimal.h"
#include "Bricks/FuelConsumerBrick.h"
#include "FlamethrowerBrick.generated.h"

class UBrickEditorParticleComponent;
class UBrickEditorAudioComponent;

UCLASS(Abstract)
class BRICKRIGS_API UFlamethrowerBrickStaticInfo : public UFuelConsumerBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Effects for the fire
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	UParticleSystem* FireEmitter;
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	USoundBase* FireSound;
	// How far the flame reaches
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	float FlameLength;
	// The maximum radius of the flame
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	float FlameRadius;
	// The damage to apply to objects being hit by the flame
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	float FlameDamage;
	// How often damage is applied and stuff is ignited
	UPROPERTY(EditDefaultsOnly, Category = Flamethrower)
	float DamageInterval;
	// ~Properties

	// ~Constructor
	UFlamethrowerBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UFlamethrowerBrick : public UFuelConsumerBrick
{
	GENERATED_BODY()

	// ~Variables
	// The current relative transform of the muzzle, should only be accessed through GetMuzzleTransform
	FTransform MuzzleTransformPrivate;
	// The last time fire damage has been applied
	float LastDamageTime;
	// The current target throttle value
	float TargetThrottle;
	// The current throttle amount
	float CurrentThrottle;
	// Set to true when the muzzle transform has to be updated
	uint8 bMuzzleTransformDirty : 1;
	// Current input channel value
	uint8 bInputChannelValue : 1;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> PSC;
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> AC;
	// ~Components

public:
	// ~Brick properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Flamethrower)
	FVehicleInputChannel InputChannel;
	// ~Brick properties

	// ~Constructor
	UFlamethrowerBrick();

	// ~Super Interface
	virtual void PostConstructVehicle() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

private:
	// To be called when the muzzle transform has to be recalculated
	void MarkMuzzleTransformDirty();
	// Returns the relative muzzle transform, calculates it first if needed
	const FTransform& GetMuzzleTransform();
	// Updates the throttle target value
	void UpdateTargetThrottle();
	// Updates the current throttle value
	void SetCurrentThrottle(float NewThrottle);
};
