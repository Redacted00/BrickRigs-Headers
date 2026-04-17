// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Items/StaticInfo/AttachmentStaticInfo.h"
#include "SightStaticInfo.generated.h"

class UMaterialInterface;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API USightStaticInfo : public UAttachmentStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Material used to replace the lense while aiming
	UPROPERTY(EditDefaultsOnly, Category = Sight)
	TSoftObjectPtr<UMaterialInterface> ReticleMaterial;
	// Needed for lense occlusion
	UPROPERTY(EditDefaultsOnly, Category = LenseOcclusion)
	float LenseRadius;
	// Material used to mask out the lense while aiming
	UPROPERTY(EditDefaultsOnly, Category = LenseOcclusion)
	TSoftObjectPtr<UMaterialInterface> OcclusionMaterial;

	// Length of the part that connects to the rail
	UPROPERTY(EditDefaultsOnly, Category = Rail)
	float RailFlangeLength;
	// Amount the scope body overhangs the flange out the rear of the sight
	UPROPERTY(EditDefaultsOnly, Category = Rail)
	float RearOverhang;
	// Overhang to the front
	UPROPERTY(EditDefaultsOnly, Category = Rail)
	float FrontOverhang;
	// ~Properties

	// ~Constructor
	USightStaticInfo();
};
