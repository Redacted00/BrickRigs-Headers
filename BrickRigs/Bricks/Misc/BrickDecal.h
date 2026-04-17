// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "BrickDecal.generated.h"

UENUM(BlueprintType)
enum class EBrickDecalShape : uint8
{
	Square,
	Triangle,
	Circle
};

/**
 * 
 */
UCLASS(BlueprintType, HideCategories = Object)
class BRICKRIGS_API UBrickDecal : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	UTexture2D* Texture;
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	EBrickDecalShape Shape;
	// ~Properties

	// ~IObjectPropertyItemInterface
	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const override;
	// ~IObjectPropertyItemInterface
};
