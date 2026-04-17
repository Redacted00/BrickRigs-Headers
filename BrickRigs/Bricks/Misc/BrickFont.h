// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Properties/ObjectPropertyItemInterface.h"
#include "BrickFont.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, HideCategories = Object)
class BRICKRIGS_API UBrickFont : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	UFont* Font;
	// Material to use on the text render component
	UPROPERTY(EditDefaultsOnly, Category = BrickMaterial)
	UMaterialInterface* FontMaterial;
#if WITH_EDITORONLY_DATA
	// Prints the unicode ranges of shared compatible characters between the two fonts when set
	UPROPERTY(Transient, EditDefaultsOnly, Category = BrickMaterial)
	UFont* ComparisonFont;
	// Unicode ranges shared between this font and the comparison font
	UPROPERTY(Transient, EditDefaultsOnly, Category = BrickMaterial)
	FString SharedUnicodeRanges;
#endif
	// ~Properties

	// ~Constructor
	UBrickFont();

	// ~Super Interface
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	// ~Super Interface

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override;
	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~IObjectPropertyItemInterface

	// Useful to iterate over each supported character of a font
	static void ForEachSupportedCharacter(const UFont* Font, const TFunction<void(const UTF32CHAR)>& Func);
};
