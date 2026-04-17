// Copyright Fluppisoft, 2018

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "CharacterCapsuleComponent.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UCharacterCapsuleComponent : public UCapsuleComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached response container, since we need to return a reference we need this
	FCollisionResponseContainer TempCollisionResponse;
	// Saved immersion depth into the default physics volume
	float DefaultWaterVolumeImmersionDepth;
	// ~Variables

public:
	// ~Super Interface
	virtual void OnRegister() override;
	virtual void InitSweepCollisionParams(FCollisionQueryParams& OutParams, FCollisionResponseParams& OutResponseParam) const override;
	virtual const FCollisionResponseContainer& GetCollisionResponseToChannels() const override;
	virtual void UpdatePhysicsVolume(bool bTriggerNotifiers) override;
	// ~Super Interface

	// This is called when the character hasn't moved to update overlaps vs vehicles
	void GetBlockingOverlaps(TArray<FOverlapResult>& OutOverlaps, float Inflation = 0.f) const;

	auto GetDefaultWaterVolumeImmersionDepth() const
	{
		return DefaultWaterVolumeImmersionDepth;
	}
};
