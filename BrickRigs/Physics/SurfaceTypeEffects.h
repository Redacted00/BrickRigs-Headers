#pragma once

#include "Sound/SoundCue.h"
#include "CoreMinimal.h"
#include "SurfaceTypeEffects.generated.h"

class UParticleSystem;
class USoundBase;
class USoundCue;
class UMaterialInterface;

USTRUCT(BlueprintType)
struct FSurfaceTypeEffects
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(EditAnywhere, Category = Footsteps)
	USoundBase* FootstepSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	UParticleSystem* ImpactEmitter = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	USoundBase* ImpactSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	UParticleSystem* SlidingEmitter = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	USoundBase* SlidingSound = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	UMaterialInterface* ImpactDecalMaterial = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = Impact)
	FFloatInterval ImpactDecalSizeRange;
	// ~Properties

	// ~Constructor
	FSurfaceTypeEffects()
	{
		ImpactDecalSizeRange.Min = 5.f;
		ImpactDecalSizeRange.Max = 7.f;
	}
};
