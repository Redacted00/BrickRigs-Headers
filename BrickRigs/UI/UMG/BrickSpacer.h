// Copyright Fluppisoft, 2018

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/Image.h"
#include "BrickSpacer.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickSpacer : public UImage
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Image)
	EBrickUIColorStyle ColorStyle;
	// ~Properties

public:
	// ~Constructor
	UBrickSpacer();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Callbacks for the slate widget
	const FSlateBrush* GetImage() const;
	FSlateColor GetImageColor() const;
};
