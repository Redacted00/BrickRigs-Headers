// Copyright Fluppisoft, 2020

#pragma once

#include "StaticInfo/WearableStaticInfo.h"
#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "Wearable.generated.h"

class UMaterialInstanceDynamic;

/**
 * Base class for all wearable items (such as pants, shirts, gloves etc)
 */
UCLASS(Abstract)
class BRICKRIGS_API AWearable : public AInventoryItem
{
	GENERATED_BODY()

protected:
	// ~Properties
	// Name displayed for the type of color
	UPROPERTY(EditDefaultsOnly, Category = Wearable)
	FText ColorDisplayName;
	// Color of the wearable
	UPROPERTY(EditDefaultsOnly, Category = Wearable)
	FLinearColor Color;
	// Texture applied to for the wearable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Wearable)
	UTexture2D* Texture;
	// Tiling of the texture
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Wearable)
	float TextureTiling;
	// ~Properties

public:
	// ~Constructor
	AWearable();

	// ~Super Interface
	virtual FText GetDisplayName() const override;
	virtual void GetItemTags(FGameplayTagContainer& OutTags) const override;
	virtual bool NeedsThumbnailMID_Implementation() const override;
	virtual void UpdateThumbnailMID_Implementation(UMaterialInstanceDynamic* InMID) const override;
	// ~Super Interface

	// Adds or removes the wearable color/pattern to the character material instance
	void ApplyWearable(UMaterialInstanceDynamic* CharacterMID, bool bApply) const;
};
