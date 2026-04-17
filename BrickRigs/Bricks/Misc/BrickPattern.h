#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "UObject/NoExportTypes.h"
#include "BrickPattern.generated.h"

class UTexture2D;

/**
*
*/
UCLASS(Blueprintable)
class BRICKRIGS_API UBrickPattern : public UObject, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	// Default color of the pattern
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pattern)
	FLinearColor Color;
	// Pattern texture applied to the brick
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pattern)
	UTexture2D* Texture;
	// Tiling scale of the texture
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Pattern)
	float Tiling;
	// ~Properties

	// ~Constructor
	UBrickPattern();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~Super Interface

	// ~IObjectPropertyItemInterface
	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const override;
	// ~IObjectPropertyItemInterface
};
