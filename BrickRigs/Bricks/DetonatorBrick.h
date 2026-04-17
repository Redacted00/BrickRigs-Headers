// Copyright Fluppisoft, 2017

#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Bricks/Brick.h"
#include "Damage/DamageType_Fire.h"
#include "DetonatorBrick.generated.h"

UCLASS()
class BRICKRIGS_API UDamageType_Detonator : public UDamageType_Fire
{
	GENERATED_BODY()
};

UCLASS(Abstract)
class BRICKRIGS_API UDetonatorBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Damage applied upon detonation
	UPROPERTY(EditDefaultsOnly, Category = Detonator)
	float Damage;
	// Particle system spawned when the detonator is activated
	UPROPERTY(EditDefaultsOnly, Category = Detonator)
	UParticleSystem* TriggerEmitter;
	// Maximum distance at which the trigger emitter should be spawned
	UPROPERTY(EditDefaultsOnly, Category = Detonator)
	float MaxTriggerEmitterDrawDistance;
	// ~Properties

	// ~Constructor
	UDetonatorBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UDetonatorBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	bool bInputChannelValue;
	// ~Variables

	// ~Components
	// Particle system used for the trigger effect
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> DetonatorPSC;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Detonator)
	FVehicleInputChannel InputChannel;
	// ~Brick Properties

public:
	// ~Constructor
	UDetonatorBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void RepairBrick() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Whether the detonator can currently detonate
	bool CanDetonate() const;
	// Activates the detonator
	void Detonate(AController* EventInstigator);

private:
	// Whether it should be possible to trigger the detonator via interaction
	bool CanDetonateViaInteraction() const
	{
		return !InputChannel.IsValidChannel();
	}

	// To be called when the input channel value has changed
	void OnInputChannelChanged();

	// Interaction callbacks
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_Detonate(ABrickPlayerController* PC);
};
