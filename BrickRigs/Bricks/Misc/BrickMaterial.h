// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "BrickMaterial.generated.h"

class UBrickPhysicalMaterial;
class UMaterialInterface;

/**
 * 
 */
UCLASS(BlueprintType, HideCategories = Object)
class BRICKRIGS_API UBrickMaterial : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	FText DisplayName;
	// Factor applied to the price of a brick
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	float PriceFactor = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	UBrickPhysicalMaterial* PhysMaterial;

	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (EditCondition = "MRAOTexture == nullptr", ClampMin = 0.f, ClampMax = 1.f))
	float Metallic = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (EditCondition = "MRAOTexture == nullptr", ClampMin = 0.f, ClampMax = 1.f))
	float Roughness = 0.4f;
	// If this is set to < 1 a translucent material will be required for the brick
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float Opacity = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (ClampMin = 0.f))
	float Brightness = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters)
	float BaseTiling = 0.01f;
	// How much of the materials metallic value to use on top of paint and patterns
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PaintMetallicAlpha = 0.f;
	// Highlight/flare brightness multiplier to use when applied to a light
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (ClampMin = 0.f))
	float LightBrightness = 40.f;
	// Background brightness when used on light bricks
	UPROPERTY(EditDefaultsOnly, Category = MaterialParameters, Meta = (ClampMin = 0.f))
	float BrickBrightness = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = Textures)
	UTexture2D* DiffuseTexture;
	UPROPERTY(EditDefaultsOnly, Category = Textures)
	UTexture2D* NormalTexture;
	UPROPERTY(EditDefaultsOnly, Category = Textures)
	UTexture2D* MRAOTexture;
	UPROPERTY(EditDefaultsOnly, Category = Textures)
	UTexture2D* EmissiveTexture;
	// ~Properties

	// Returns the user-friendly name
	FText GetDisplayName() const
	{
		return DisplayName;
	}

	// Whether the material is translucent
	bool IsTranslucentMaterial() const
	{
		return Opacity < 1.f;
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const override;
	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~IObjectPropertyItemInterface
};
