// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "UI/UMG/BrickMultiLineTextBox.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickTextBoxWidget.generated.h"

class UBrickMultiLineTextBox;
class UBrickButtonWidget;

/**
 * This widget is used as a wrapper for the editable text box
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickTextBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTextBoxChanged, const FText&, NewText, EValueChangedEventType, EventType);

private:
	// ~Variables
	// Helper flag to indicate if this button currently has user focus
	uint8 bIsFocused : 1;
	// Whether focus was set manually
	uint8 bHasCustomFocus : 1;
	// Whether the password is currently readable
	uint8 bIsPasswordVisible : 1;
	// Set to true during the SetText function to avoid calling the delegate
	uint8 bIsSettingText : 1;
	// Whether the child text box has focus
	uint8 bIsTyping : 1;
	// The currently displayed text
	FText CurrentText;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickMultiLineTextBox* MultiLineTextBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* ShowPasswordButton;
	// ~Widgets

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnTextBoxChanged OnTextChangedDelegate;
	// ~Delegates

	// ~Properties
	// Maximum length of the entered text
	UPROPERTY(EditAnywhere, Category = TextBox, meta = (ClampMin = 0))
	int32 MaxTextLength;
	// Whether multiple lines can be entered
	UPROPERTY(EditAnywhere, Category = TextBox)
	bool bAllowMultiLine;
	// Whether this is a password field
	UPROPERTY(EditAnywhere, Category = TextBox)
	bool bIsPassword;
	// Whether this text box is used for numeric input
	UPROPERTY(EditAnywhere, Category = TextBox)
	bool bIsNumeric;
	// ~Properties

	// ~Constructor
	UBrickTextBoxWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	// Set the color scheme of the text box
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle InColorStyle);
	// Set the brush style of the text box border
	UFUNCTION(BlueprintCallable)
	void SetBrushStyle(EBrickUIBrushStyle InBrushStyle);
	// Set the padding style of the text box border
	UFUNCTION(BlueprintCallable)
	void SetPaddingStyle(EBrickUIPaddingStyle InPaddingStyle);
	// Set the text style of the text box
	UFUNCTION(BlueprintCallable)
	void SetTextStyle(EBrickUITextStyle InTextStyle);
	// Sets the text box justification
	UFUNCTION(BlueprintCallable)
	void SetJustification(TEnumAsByte<ETextJustify::Type> NewJustification);
	// Set the displayed text, optionally validates it to make sure it fits the required display criteria
	UFUNCTION(BlueprintCallable)
	void SetText(const FText& InText, bool bValidateText = false);
	// Get the displayed text
	UFUNCTION(BlueprintPure)
	FText GetText() const;
	// Sets the text displayed when nothing is entered
	UFUNCTION(BlueprintCallable)
	void SetHintText(const FText& InText);
	// Set the maximum allowed length of entered text
	UFUNCTION(BlueprintCallable)
	void SetMaxTextLength(int32 InMaxLength);
	// Set whether multiple line can be entered
	UFUNCTION(BlueprintCallable)
	void SetAllowMultiLine(bool bInAllowMultiLine);
	// Set whether the text should be displayed as a password
	UFUNCTION(BlueprintCallable)
	void SetIsPassword(bool bNewIsPassword);
	// Shows or hides the password
	UFUNCTION(BlueprintCallable)
	void SetPasswordVisible(bool bNewVisible);
	// Whether the password is currently being shown
	UFUNCTION(BlueprintPure)
	bool IsPasswordVisible() const;
	// Shows or hides the password
	UFUNCTION(BlueprintCallable)
	void TogglePasswordVisible();
	// Changes the read only state
	UFUNCTION(BlueprintCallable)
	void SetIsReadOnly(bool bNewReadOnly);
	// Whether read only mode is enabled
	UFUNCTION(BlueprintPure)
	bool IsReadOnly() const;
	// Turns the text box into a marquee
	UFUNCTION(BlueprintCallable)
	void SetIsMarquee(bool bNewMarquee);
	// Whether marquee mode is enabled
	UFUNCTION(BlueprintPure)
	bool IsMarquee() const;
	// Returns whether the inner text box has user focus
	UFUNCTION(BlueprintPure)
	bool IsTyping() const;
	// Version that takes a focus path
	bool IsTyping(const FWidgetPath& FocusPath) const;
	// Can be used to mark the text box as focused or unfocused manually
	UFUNCTION(BlueprintCallable, Category = Button)
	void SetCustomFocus(bool bNewUseCustomFocus, bool bNewFocused = false);
	// Set whether the text box should appear as pressed
	void SetIsPressed(bool bNewPressed);
	// Can be called by this classes' key down event and container widgets like the slider
	FReply HandleKeyDown(const FKeyEvent& InKeyEvent);
	// Gives user focus to the next box
	UFUNCTION(BlueprintCallable)
	void StartTyping(bool bSelectAllText = false);
	// Version that populates an event reply
	FReply& StartTyping(FReply& InReply);
	// Get the subwidget that should be focused in order to be able to type
	UWidget* GetWidgetToFocusForTyping() const;

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsPassword(bool bNewIsPassword);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePasswordVisible(bool bNewVisible);
	// ~Blueprint Interface

private:
	// Updates the appearance
	void UpdateIsPassword();
	// Updates the text box and invokes the blueprint event
	void UpdatePasswordVisible();
	// Called to synchronize the multi line parameter
	void UpdateAllowMultiline();
	// Sets whether the text box should appear focused
	void SetIsFocused(bool bNewFocused);
	// Returns whether the given text is valied, optionally returning a validated text
	bool ValidateText(FText& InOutText) const;
	// To be called when the text has been changed or committed, returns true if the entered text is valid
	bool OnTextChangedInternal(const FText& NewText);
	// Callbacks for the text box
	UFUNCTION()
	void OnTextChanged(const FText& NewText);
};
