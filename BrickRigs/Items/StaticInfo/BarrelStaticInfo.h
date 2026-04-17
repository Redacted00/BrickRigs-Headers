// Copyright Fluppisoft, 2020

#pragma once

#include "Components/FirearmComponent.h"
#include "CoreMinimal.h"
#include "Items/StaticInfo/AttachmentStaticInfo.h"
#include "BarrelStaticInfo.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBarrelStaticInfo : public UAttachmentStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The muzzle effect to use when attached to a firearm
	UPROPERTY(EditDefaultsOnly, Category = Barrel)
	FMuzzleEffect MuzzleEffect;
	UPROPERTY(EditDefaultsOnly, Category = Barrel)
	float InitialSpeedScale = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = Barrel)
	float RangeScale = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = Barrel)
	float DamageScale = 1.f;
	// ~Properties

	// ~Constructor
	UBarrelStaticInfo();
};
