// Copyright Fluppisoft, 2016

#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Bricks/FuelConsumerBrick.h"
#include "ThrusterBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UThrusterBrickStaticInfo : public UFuelConsumerBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float Thrust;
	// Speed at which accumulated input is added
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float ThrottleInputRate;
	// The socket where effects are spawned and the thrust is applied
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	FName NozzleSocketName;
	// Cached transform of the nozzle socket
	UPROPERTY(VisibleDefaultsOnly)
	FTransform NozzleTransform;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	UParticleSystem* ParticleEmitter;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	USoundBase* ThrusterSound;
	// ~Properties

	// ~Constructor
	UThrusterBrickStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BuildCachedData() override;
#endif
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UThrusterBrick : public UFuelConsumerBrick
{
	GENERATED_BODY()

	// ~Variables
	// Current input channel value
	float InputChannelValue;
	// Separate input state from the input channel so it can be accumulated
	float AccumulatedInput;
	// Current throttle value
	float CurrentThrottle;
	// Compressed input for replication
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepAccumulatedInput)
	int8 RepAccumulatedInput;
	UFUNCTION()
	void OnRep_RepAccumulatedInput();
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UParticleSystemComponent> ThrusterPSC;
	TBrickEditorComponentPtr<UAudioComponent> ThrusterAC;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Thruster)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float InputScale;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Thruster)
	bool bAccumulated;
	// ~Brick Properties

public:
	// ~Constructor
	UThrusterBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool ShouldReplicate() const override
	{
		// Only need to replicate accumulated input, since that could get out of sync
		return bAccumulated;
	}

	virtual void RepairBrick() override;

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
	// Updates the current throttle value
	void SetCurrentThrottle(float NewThrottle, bool bReset = false);
	// Updates the accumulated input value
	void SetAccumulatedInput(float NewInput);
	// Called whenever the accumulated input value has changed
	void OnAccumulatedInputChanged();
	// Whether the local player has authority over the throttle value
	bool HasThrottleAuthority() const
	{
		return !IsReplicated() || !IsNetMode(NM_Client);
	}

	// To be called when the input channel value has changed
	void OnInputChannelChanged();
};
