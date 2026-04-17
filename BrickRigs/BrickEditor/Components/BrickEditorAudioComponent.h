// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "BrickEditorAudioComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorAudioComponent : public UAudioComponent, public IBrickEditorComponentInterface
{
	GENERATED_BODY()

public:
	// ~Constructor
	UBrickEditorAudioComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	// ~Super Interface

private:
	// Callback for our own delegate
	void OnAudioFinishedCallback(UAudioComponent* InAudioComponent);
};

struct FBrickEditorAudioComponentParams : public FBrickEditorSceneComponentParams
{
	USoundBase* Sound;
	float VolumeMultiplier;
	float PitchMultiplier;
	FSoundAttenuationSettings AttenuationOverrides;
	bool bOverrideAttenuation;

	FBrickEditorAudioComponentParams(const UBrickEditorAudioComponent* Default = GetDefault<UBrickEditorAudioComponent>())
		: FBrickEditorSceneComponentParams(Default),
		  Sound(Default->Sound),
		  VolumeMultiplier(Default->VolumeMultiplier),
		  PitchMultiplier(Default->PitchMultiplier),
		  bOverrideAttenuation(Default->bOverrideAttenuation)
	{
	}

	void InitializeComponent(UBrickEditorAudioComponent* Comp)
	{
		FBrickEditorSceneComponentParams::InitializeComponent(Comp);

		Comp->Sound = Sound;
		Comp->VolumeMultiplier = VolumeMultiplier;
		Comp->PitchMultiplier = PitchMultiplier;
		Comp->AttenuationOverrides = AttenuationOverrides;
		Comp->bOverrideAttenuation = bOverrideAttenuation;
	}
};
