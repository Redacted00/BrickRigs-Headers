// Copyright Fluppisoft, 2016

#pragma once

#include "Sound/SoundNode.h"
#include "SoundNodeMotor.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API USoundNodeMotor : public USoundNode
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Motor)
	TArray<USoundWave*> MotorSounds;
	// Ratio of step at which the first wave is fully faded in, 0-1
	UPROPERTY(EditAnywhere, Category = Motor, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float FadeInRatio;
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient, EditAnywhere, Category = Motor, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float EditorRPMRatio;
#endif
	// ~Properties

	// ~Constructor
	USoundNodeMotor();

	// ~Super Interface
	virtual int32 GetMaxChildNodes() const override { return 0; }
	virtual float GetDuration() override { return INDEFINITELY_LOOPING_DURATION; }
	virtual int32 GetNumSounds(const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound) const override;
	virtual void ParseNodes(FAudioDevice* AudioDevice, const UPTRINT NodeWaveInstanceHash, FActiveSound& ActiveSound, const FSoundParseParameters& ParseParams, TArray<FWaveInstance*>& WaveInstances) override;
	// ~Super Interface
};
