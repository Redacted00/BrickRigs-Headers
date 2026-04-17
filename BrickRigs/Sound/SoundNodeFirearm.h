// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Sound/SoundNode.h"
#include "SoundNodeFirearm.generated.h"

class USoundWave;

/**
 * 
 */
UCLASS()
class BRICKRIGS_API USoundNodeFirearm : public USoundNode
{
	GENERATED_BODY()

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Firearm)
	USoundWave* FireSound;
	UPROPERTY(EditAnywhere, Category = Firearm)
	FFloatInterval PitchModulation;
	UPROPERTY(EditAnywhere, Category = Firearm)
	FFloatInterval VolumeModulation;
	UPROPERTY(EditAnywhere, Category = Firearm)
	float ModulationPeriod;
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = Firearm)
	float EditorFireDelay;
#endif
	// ~Properties

public:
	// ~Constructor
	USoundNodeFirearm();

	// ~Super Interface
	virtual int32 GetMaxChildNodes() const override;
	virtual float GetDuration() override;
	virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;
	// ~Super Interface
};
