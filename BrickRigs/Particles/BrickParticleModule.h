// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Particles/ParticleModule.h"
#include "BrickParticleModule.generated.h"

/**
 * 
 */
UCLASS(Abstract, EditInlineNew, CollapseCategories, HideCategories = Object, Meta = (DisplayName = "Brick Rigs"))
class BRICKRIGS_API UBrickParticleModule : public UParticleModule
{
	GENERATED_BODY()
};

UCLASS(EditInlineNew, CollapseCategories, HideCategories = Object, Meta = (DisplayName = "Brick Fire"))
class BRICKRIGS_API UParticleModuleBrickFire : public UBrickParticleModule
{
	GENERATED_BODY()

public:
	// ~Constructor
	UParticleModuleBrickFire();

	// ~Super Interface
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	// ~Super Interface
};

UCLASS(EditInlineNew, CollapseCategories, HideCategories = Object, Meta = (DisplayName = "Brick Sliding"))
class BRICKRIGS_API UParticleModuleBrickSliding : public UBrickParticleModule
{
	GENERATED_BODY()

public:
	// ~Constructor
	UParticleModuleBrickSliding();

	// ~Super Interface
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	// ~Super Interface
};

// Used on precipitation emitters to wrap the particles around into the bounding volume
UCLASS(EditInlineNew, CollapseCategories, HideCategories = Object, Meta = (DisplayName = "Precipitation"))
class BRICKRIGS_API UParticleModulePrecipitation : public UBrickParticleModule
{
	GENERATED_BODY()

public:
	// ~Constructor
	UParticleModulePrecipitation();

	// ~Super Interface
	virtual void Update(FParticleEmitterInstance* Owner, int32 Offset, float DeltaTime) override;
	// ~Super Interface
};

// Used to spawn particles around a collapsing building
UCLASS(EditInlineNew, CollapseCategories, HideCategories = Object, Meta = (DisplayName = "Building Collapse"))
class BRICKRIGS_API UParticleModuleBuildingCollapse : public UBrickParticleModule
{
	GENERATED_BODY()

public:
	// ~Constructor
	UParticleModuleBuildingCollapse();

	// ~Super Interface
	virtual void Spawn(FParticleEmitterInstance* Owner, int32 Offset, float SpawnTime, FBaseParticle* ParticleBase) override;
	// ~Super Interface
};
