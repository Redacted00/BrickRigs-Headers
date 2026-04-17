// Copyright Fluppisoft, 2016

#pragma once

#include "GameFramework/DamageType.h"
#include "DamageType_Explosion.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UDamageType_Explosion : public UDamageType
{
	GENERATED_BODY()

public:
	// ~Properties
	// How likely objects are to catch fire when damaged
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float FireProbability;
	// ~Properties

	bool ShouldSetOnFire() const
	{
		return FMath::FRand() < FireProbability;
	}
};
