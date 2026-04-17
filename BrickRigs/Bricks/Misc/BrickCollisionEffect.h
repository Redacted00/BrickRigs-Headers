#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "CoreMinimal.h"

struct FBrickCollisionEffect
{
	// Audio component used to play the collision sound
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> AC;
	// World time when the effect was spawned
	float StartTime;
};

struct FBrickSlidingEffect
{
	// Components used for the audio visual effects
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> PSC;
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> AC;
	// List of all locations where particles should be spawned this frame
	// NOTE: We can't use this to count the number of active sliding points per frame, since it needs to persist when the particle system is ticking
	TArray<FVector> ParticleSpawnLocations;
	// Location of all sliding points accumulated over one frame
	FVector AccumulatedLocation;
	// Accumulated number of sliding points per frame
	uint16 NumSlidingPoints;
	// The maximum sliding speed accumulated over one frame
	float SlidingSpeed;

	// ~Constructor
	FBrickSlidingEffect()
		: NumSlidingPoints(0), SlidingSpeed(0.f)
	{
	}
};
