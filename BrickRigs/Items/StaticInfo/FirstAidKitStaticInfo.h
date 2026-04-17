// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Items/StaticInfo/ItemStaticInfo.h"
#include "FirstAidKitStaticInfo.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UFirstAidKitStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Number of bandages per first aid kit
	UPROPERTY(EditAnywhere, Category = FirstAidKit)
	int32 NumBandages;
	// The amount of health each bandage can provide
	UPROPERTY(EditAnywhere, Category = FirstAidKit)
	float HealthPerBandage;
	// How long healing should be delayed after receiving damage
	UPROPERTY(EditAnywhere, Category = FirstAidKit)
	float HealDelay;
	// How long it would take to fully heal a player from 0 to 100%
	UPROPERTY(EditAnywhere, Category = FirstAidKit)
	float HealTime;
	// ~Properties

	// ~Constructor
	UFirstAidKitStaticInfo();
};
