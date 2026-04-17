// Copyright Fluppisoft, 2020

#pragma once

#include "StaticInfo/BarrelStaticInfo.h"
#include "Components/FirearmComponent.h"
#include "CoreMinimal.h"
#include "Items/Attachment.h"
#include "BarrelAttachment.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API ABarrelAttachment : public AAttachment
{
	GENERATED_BODY()

public:
	// Return the desired muzzle effect
	const auto& GetMuzzleEffect() const
	{
		return GetStaticInfo<UBarrelStaticInfo>()->MuzzleEffect;
	}

	// Allows the barrel to affect projectile parameters
	void ModifyProjectileParams(FProjectileParams& InOutParams) const;
};
