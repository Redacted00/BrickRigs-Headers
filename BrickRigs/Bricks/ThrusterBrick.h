// Copyright Fluppisoft, 2016

#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "ScalableBrick.h"
#include "ThrusterBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UThrusterBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	FFuelTankParams FuelTankParams;
	// Amount of fuel in liters to consume per second at full throttle at a nozzle size of 1x1 brick units
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float FuelConsumption = 0.5f;
	// Thrust produced at a nozzle size of 1x1 brick units
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float Thrust = 200000.f;
	// Min and max value for the input scale
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float MaxInputScale = 25.f;
	// Speed at which accumulated input is added
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float ThrottleInputRate = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float GlowInterpSpeed = 4.f;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float AfterglowInterpSpeedUp = 0.2f;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	float AfterglowInterpSpeedDown = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	FLinearColor GlowColor;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	FLinearColor AfterglowColor;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	UStaticMesh* NozzleMesh;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	UParticleSystem* ThrusterParticleSystem;
	UPROPERTY(EditDefaultsOnly, Category = Thruster)
	USoundBase* ThrusterSound;
	// ~Properties

	// ~Constructor
	UThrusterBrickStaticInfo();

	// ~Super Interface
	virtual FFuelTankParams GetFuelTankParams() const override;
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UThrusterBrick : public UScalableBrick
{
	GENERATED_BODY()

	enum class EThrusterPrimitiveData : uint8
	{
		GlowBrightness = Max,
		AfterglowBrightness = GlowBrightness + 1,
		Max = AfterglowBrightness + 1
	};

	// ~Variables
	// Current input channel value
	float InputChannelValue;
	// Separate input state from the input channel so it can be accumulated
	float AccumulatedInput;
	// Current throttle value
	float CurrentThrottle;
	// Current brightness of the glow and afterglow effect
	float GlowBrightness;
	float AfterglowBrightness;
	// Compressed input for replication
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepAccumulatedInput)
	int8 RepAccumulatedInput;
	UFUNCTION()
	void OnRep_RepAccumulatedInput();

	FFuelTankRuntimeParams FuelTankRuntimeParams;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UParticleSystemComponent> ThrusterPSC;
	TBrickEditorComponentPtr<UAudioComponent> ThrusterAC;
	TBrickEditorComponentPtr<UBrickEditorStaticMeshComponent> NozzleSMC;
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
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void PostConstructVehicle() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool ShouldReplicate() const override
	{
		// Only need to replicate accumulated input, since that could get out of sync
		return bAccumulated;
	}

	virtual void UpdateCustomDepth(bool bEnable, uint8 Stencil) override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual const FFuelTankRuntimeParams* GetFuelTankRuntimeParams() const override;
	virtual EFluAxisLock GetBrickSizeAxisLock() const override;
	virtual FVector GetMaxBrickSize() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~Super Interface

	// Returns the relative location of the nozzle
	FVector GetNozzleRelativeLocation() const;

	// Returns the thrust force at 100% input scale
	float GetDefaultThrustForce() const;
	// Returns the maxmum thrust force
	float GetThrustForce() const;
	// Sets the input scale to match the desired thrust force
	void SetThrustForce(const float NewThrustForce);
	// Returns the minimum and maximum thrust force
	FFloatInterval GetThrustForceRange() const;

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

	// Returns the thrust factor based on the thruster size
	float GetThrustFactor() const;

	// Whether a nozzle mesh should be used
	bool ShouldHaveNozzleMesh() const;
	// Returns the relative 3D scale to use for the nozzle mesh
	FVector GetNozzleMeshScale() const;
	// Creates, updates or removes the nozzle mesh as needed
	void UpdateNozzleMesh();
	// Updates the glow brightness effect
	void SetGlowBrightness(const float NewGlowBrightness, const float NewAfterglowBrightness);

	// To be called when the input channel value has changed
	void OnInputChannelChanged();
};
