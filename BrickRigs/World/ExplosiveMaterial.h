// Copyright Fluppisoft, 2017

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ExplosiveMaterial.generated.h"

class AExplosion;
class UParticleSystem;
class USoundBase;

/**
 *
 */
UCLASS(Blueprintable, Abstract)
class BRICKRIGS_API UExplosiveMaterial : public UObject, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	// Name of the material to display on the UI
	UPROPERTY(EditAnywhere, Category = Explosive)
	FText DisplayName;
	// Price of this material per liter of volume
	UPROPERTY(EditAnywhere, Category = Explosive)
	float Price;
	// Determines how strong an impact has to be in order to ignite the material
	UPROPERTY(EditAnywhere, Category = Explosive)
	float ImpactResistance;
	// Determines how much damage has to be applied in order to ignite the material
	UPROPERTY(EditAnywhere, Category = Explosive)
	float DamageResistance;
	// Determines the explosive damage and radius
	UPROPERTY(EditAnywhere, Category = Explosive)
	float VolumeIncrease;
	// Allows adjusting the damage
	UPROPERTY(EditAnywhere, Category = Explosive)
	float DamageScale;
	// Damage type to use when applying explosion damage
	UPROPERTY(EditAnywhere, Category = Explosive)
	TSubclassOf<UDamageType> DamageType;
	// Whether the material can be used as fuel
	UPROPERTY(EditAnywhere, Category = Explosive)
	uint8 bIsFuel : 1;
	UPROPERTY(EditAnywhere, Category = Explosive)
	TSubclassOf<AExplosion> ExplosionClass;
	UPROPERTY(EditAnywhere, Category = Explosive)
	UParticleSystem* ExplosionEmitter;
	UPROPERTY(EditAnywhere, Category = Explosive)
	USoundBase* ExplosionSound;
	// ~Properties

	// ~Constructor
	UExplosiveMaterial();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~Super Interface

	// Returns the user friendly name
	FText GetDisplayName() const
	{
		return DisplayName;
	}

	// Return the radial damage params for the given volume
	void GetRadialDamageParams(float Volume, FRadialDamageParams& OutParams) const;

private:
	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~IObjectPropertyItemInterface
};
