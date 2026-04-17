// Copyright Fluppisoft, 2020

#pragma once

#include "GameplayTags.h"
#include "Projectiles/AmmoType.h"
#include "CoreMinimal.h"
#include "Items/StaticInfo/ItemStaticInfo.h"
#include "AmmoBoxStaticInfo.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UAmmoBoxStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Number of rounds contained in one box
	UPROPERTY(EditAnywhere, Category = AmmoBox)
	int32 AmmoCapacity;
	// The supported calibers
	UPROPERTY(EditAnywhere, Category = AmmoBox)
	FGameplayTagContainer CompatibleCalibers;
	// ~Properties

	// ~Constructor
	UAmmoBoxStaticInfo();
};
