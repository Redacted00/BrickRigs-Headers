// Copyright Fluppisoft, 2017

#pragma once

#include "Misc/BrickDecal.h"
#include "Bricks/Brick.h"
#include "ImageplateBrick.generated.h"

class UMaterialInstanceDynamic;

UCLASS(Abstract)
class BRICKRIGS_API UImageplateBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Imageplate)
	EBrickDecalShape ImageplateShape;
	// Size of the margin to keep around the image
	UPROPERTY(EditDefaultsOnly, Category = Imageplate)
	float ImageMargin;
	// ~Properties

	// ~Constructor
	UImageplateBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UImageplateBrick : public UBrick
{
	GENERATED_BODY()

private:
	// ~Variables
	// Material instance used for the image
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* ImageMID;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Image)
	UBrickDecal* Image;
	UPROPERTY(EditDefaultsOnly, Category = Image)
	FBrickColor ImageColor;
	// ~Brick properties

public:
	// ~Constructor
	UImageplateBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void SetupCreateStaticMeshComponentParams(FBrickStaticMeshComponentParams& Params) override;
	virtual UMaterialInterface* GetEditorBrickMaterial(int32 MaterialIndex) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	// Super Interface

private:
	// Updates the displayed image
	void UpdateImage();
};
