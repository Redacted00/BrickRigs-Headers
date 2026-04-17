// Copyright Fluppisoft, 2019

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "BrickEditableTextBox.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickEditableTextBox : public UEditableTextBox
{
	GENERATED_BODY()

private:
	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// Custom style struct used for the delegate
	FEditableTextBoxStyle CustomStyle;
	// The style parameters used
	EBrickUIColorStyle ColorStyle;
	EBrickUITextStyle TextStyle;
	// Whether the text box is currently focused
	bool bIsFocusedOrHovered;
	// Whether the text box is currently pressed
	bool bIsPressed;
	// ~Variables

public:
	// ~Constructor
	UBrickEditableTextBox();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	// Called by the outer text box widget
	void SetColorStyle(EBrickUIColorStyle InColorStyle);
	void SetTextStyle(EBrickUITextStyle InTextStyle);
	void SetJustification(TEnumAsByte<ETextJustify::Type> NewJustification);
	void SetIsFocusedOrHovered(bool bNewFocused);
	void SetIsPressed(bool bNewPressed);

public:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Update the current style
	void UpdateTextBoxStyle();
	// Update the parts of the style that change with the current focus
	void UpdateFocusedStyle();
};
