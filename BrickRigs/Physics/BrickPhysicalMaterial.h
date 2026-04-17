// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "BrickPhysicalMaterial.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickPhysicalMaterial : public UPhysicalMaterial
{
	GENERATED_BODY()

public:
	// ~Properties
	// Determines how resistant the material is to damage
	UPROPERTY(EditDefaultsOnly, Category = PhysicalMaterial, meta = (ClampMin = 0.f))
	float Strength;
	// Hardness of the material, used to determine if projectiles can penetrate and the impact effects to use upon a collision
	UPROPERTY(EditDefaultsOnly, Category = PhysicalMaterial, meta = (ClampMin = 0.f))
	float Hardness;
	// Determines how flammabile the material is
	UPROPERTY(EditDefaultsOnly, Category = PhysicalMaterial, meta = (ClampMin = 0.f, UIMax = 1.f))
	float Flammability;
	// ~Properties

	// ~Constructor
	UBrickPhysicalMaterial();
};
