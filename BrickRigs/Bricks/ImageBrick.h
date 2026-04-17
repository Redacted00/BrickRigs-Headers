// Copyright Fluppisoft, 2017

#pragma once

#include "ScalableBrick.h"
#include "Bricks/Brick.h"
#include "ImageBrick.generated.h"

class UMaterialInstanceDynamic;

UCLASS(Abstract)
class BRICKRIGS_API UImageBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Size of the margin to keep around the image
	UPROPERTY(EditDefaultsOnly, Category = Image)
	float ImageMargin;
	// Mesh to  use to display the image
	UPROPERTY(EditDefaultsOnly, Category = Image)
	UStaticMesh* ImageMesh;
	// ~Properties

	// ~Constructor
	UImageBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UImageBrick : public UScalableBrick
{
	GENERATED_BODY()

	// ~Variables
	TBrickEditorComponentPtr<UStaticMeshComponent> DecalMeshComponent;
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
	UImageBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	// Super Interface

private:
	// Updates the displayed image
	void UpdateImage();
};
