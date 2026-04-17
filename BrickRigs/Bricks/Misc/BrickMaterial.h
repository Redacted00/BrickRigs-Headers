// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "BrickMaterial.generated.h"

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
	UPROPERTY(EditDefaultsOnly)
	float PriceFactor;
	// The optional override material
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	UMaterialInterface* MaterialOverride;
	// Whether the material used is translucent
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	bool bIsTranslucentMaterial;
	// Whether the material uses a diffuse map
	UPROPERTY(VisibleAnywhere)
	bool bHasDiffuseMap;
	// Default diffuse color of the material
	UPROPERTY(VisibleAnywhere)
	FLinearColor DiffuseColor;
	// ~Properties

	// ~Constructor
	UBrickMaterial();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
#endif
	// ~Super Interface

	// Returns the user friendly name
	FText GetDisplayName() const
	{
		return DisplayName;
	}

	// Whether the material is translucent
	bool IsTranslucentMaterial() const
	{
		return bIsTranslucentMaterial;
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const override;
	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~IObjectPropertyItemInterface

private:
#if WITH_EDITOR
	// Callback for the PIE delegate
	void OnBeginPIE(bool bIsSimulating);
	// Calculate cached material parameters
	void CacheMaterialParameters();
#endif
};
