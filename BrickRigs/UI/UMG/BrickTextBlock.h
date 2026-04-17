// Copyright Fluppisoft, 2019

#pragma once

#include "UI/BrickUIStyle.h"
#include "Components/TextBlock.h"
#include "BrickTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickTextBlock : public UTextBlock
{
	GENERATED_BODY()

private:
	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIStyleState StyleState;
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUITextStyle TextStyle;
	// ~Properties

public:
	// ~Constructor
	UBrickTextBlock();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	// Sets the current color style
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	// Sets the current color state
	UFUNCTION(BlueprintCallable)
	void SetStyleState(EBrickUIStyleState NewState);
	// Sets the current text style
	UFUNCTION(BlueprintCallable)
	void SetTextStyle(EBrickUITextStyle NewStyle);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Callbacks for the slate widget
	FSlateFontInfo GetFont() const;
	FSlateColor GetTextColor() const;
	FLinearColor GetShadowColor() const;
	FVector2D GetShadowOffset() const;
};
