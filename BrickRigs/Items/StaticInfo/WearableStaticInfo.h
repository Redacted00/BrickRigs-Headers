// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Items/StaticInfo/ItemStaticInfo.h"
#include "WearableStaticInfo.generated.h"

UENUM(BlueprintType)
enum class EWearableTag : uint8
{
	Pants,
	Shirt
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UWearableStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The type of wearable, i.e. a shirt, pants etc.
	UPROPERTY(EditDefaultsOnly, Category = Wearable)
	EWearableTag WearableType;
	// ~Properties

	// ~Constructor
	UWearableStaticInfo();
};
