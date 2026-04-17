// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/MultiLineEditableTextBox.h"
#include "BrickMultiLineTextBox.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickMultiLineTextBox : public UMultiLineEditableTextBox
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	FEditableTextBoxStyle CustomStyle;
	FTextBlockStyle CustomTextStyle;
	// The color and brush style used for the text box
	EBrickUIColorStyle ColorStyle;
	EBrickUIBrushStyle BrushStyle;
	EBrickUIPaddingStyle PaddingStyle;
	EBrickUITextStyle BrickTextStyle;
	uint8 bIsFocused : 1;
	uint8 bIsPressed : 1;
	uint8 bIsMarquee : 1;
	// ~Variables

public:
	// ~Constructor
	UBrickMultiLineTextBox();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	// Enables or disables auto wrapping
	void SetAutoWrapText(bool bInAutoTextWrap);
	// Used by the outer text box widget to sync the style
	void SetColorStyle(EBrickUIColorStyle InColorStyle);
	void SetBrushStyle(EBrickUIBrushStyle InBrushStyle);
	void SetPaddingStyle(EBrickUIPaddingStyle InPaddingStyle);
	void SetTextStyle(EBrickUITextStyle InTextStyle);
	void SetIsFocused(bool bNewFocused);
	void SetIsPressed(bool bNewPressed);
	void SetIsMarquee(bool bNewMarquee);

	bool IsMarquee() const
	{
		return bIsMarquee;
	}

	// Selects all text in the box
	void SelectAllText();

	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Update the current style
	void UpdateTextBoxStyle();
	// Update the parts of the style that change with the current focus
	void UpdateFocusedStyle();
	// Updates the text style only
	void UpdateTextStyle();
};
