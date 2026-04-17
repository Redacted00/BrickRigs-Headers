// Copyright Fluppisoft, 2018

#pragma once

#include "Bricks/SensorBrickBase.h"
#include "SwitchBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API USwitchBrickStaticInfo : public USensorBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	USwitchBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API USwitchBrick : public USensorBrickBase
{
	GENERATED_BODY()

	enum ESwitchPrimitiveData
	{
		SwitchValue = Max,
		Max = SwitchValue + 1
	};

	// ~Variables
	// Stores the current input channel value
	float InputChannelValue;
	// Unmodified input channel value
	float InputChannelValueRaw;
	// Stores the current interaction input
	UPROPERTY(Transient, ReplicatedUsing = OnRep_InteractionValue)
	int8 InteractionValue;
	UFUNCTION()
	void OnRep_InteractionValue();
	// Last rounded interaction value received from input
	int8 LastRoundedInteractionValue;
	// Last locally calculated interaction value
	int8 LastLocalInteractionValue;
	// Whether the switch value currently needs to be updated
	bool bNeedsSwitchValueUpdate;
	// Last frame where the switch value has been updated
	uint64 LastSwitchValueUpdateFrame;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Switch)
	FString SwitchName;
	UPROPERTY(EditDefaultsOnly, Category = Switch)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Switch)
	bool bReturnToZero;
	// ~Brick Properties

public:
	// ~Constructor
	USwitchBrick();

	// ~Super Interface
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void PostConstructVehicle() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual bool ShouldReplicate() const override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Whether the switch can be controlled via interaction
	bool CanSwitchViaInteraction() const
	{
		return !InputChannel.IsValidChannel();
	}

	// Called from the player controller
	void SetSwitchValueFromInteraction(int8 NewValue);

private:
	// Updates the switch output value
	void UpdateSwitchValue(bool bImmediate);
	// Returns the desired parameter value
	float GetSwitchValueMaterialParameter() const
	{
		return FMath::Clamp(OutputChannel.GetValue(), -1.f, 1.f);
	}

	// Interaction callbacks
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_Switch(ABrickPlayerController* OtherPC, float Val);
	UFUNCTION()
	void Interact_Deactivate(ABrickPlayerController* OtherPC);
};
