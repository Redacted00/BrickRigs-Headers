// Copyright Fluppisoft, 2022

#pragma once

#include "Misc/BrickAssetManager.h"
#include "CoreMinimal.h"
#include "Misc/FluAsyncAssetLoader.h"
#include "World/StaticMeshProp.h"
#include "Billboard.generated.h"

// An image that can be displayed on a billboard
UCLASS()
class BRICKRIGS_API UBillboardImage : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// The texture to display
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> Texture;
	// How likely the image is to be displayed compared to other images
	UPROPERTY(EditAnywhere, Meta = (ClampMin = 0))
	float Probability = 1.f;
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API ABillboard : public AStaticMeshProp
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	// ~Super Interface

private:
	// Saved random seed
	UPROPERTY()
	uint8 BillboardRandSeed;
	// Used for async loading of the texture
	FFluAsyncAssetLoader AssetLoader_BillboardTexture;
};
