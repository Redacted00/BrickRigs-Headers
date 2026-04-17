// Copyright Fluppisoft, 2019

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWidgetBase.generated.h"

class UBrickBorder;

/**
 * Implements some shared button functionality, for actual buttons and sliders for example
 */
UCLASS(Abstract)
class BRICKRIGS_API UButtonWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Whether the button currently has user focus or is hovered
	uint8 bIsFocused : 1;
	// Helper flag to indicate if this button currently has user focus
	uint8 bHasUserFocus : 1;
	// Whether manual focus was assigned
	uint8 bHasCustomFocus : 1;
	// Whether the custom focus flag should be used
	uint8 bUseCustomFocus : 1;
	// Whether the button is pressed down
	uint8 bIsPressed : 1;
	// Whether the button has been pressed down by a double click
	uint8 bIsPressedByDoubleClick : 1;
	// Whether dragging should currently be detected
	uint8 bShouldDetectDrag : 1;
	// Position the button has been clicked at
	FVector2D InitialClickPosition;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickBorder* Border;
	// ~Widgets

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Button)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, Category = Button)
	EBrickUIBrushStyle BrushStyle;
	// Whether the button should only be drawn while it is visible
	UPROPERTY(EditAnywhere, Category = Button)
	bool bHiddenWhileUnfocused;
	// Whether the button should appear selected
	UPROPERTY(EditAnywhere, Category = Button)
	bool bIsSelected;
	// ~Properties

	// ~Constructor
	UButtonWidgetBase(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativePreConstruct() override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable, Category = Button)
	void SetColorStyle(EBrickUIColorStyle NewColorStyle);
	UFUNCTION(BlueprintCallable, Category = Button)
	void SetBrushStyle(EBrickUIBrushStyle NewBrushStyle);
	// Can be used to mark the button as focused or unfocused manually
	UFUNCTION(BlueprintCallable, Category = Button)
	void SetCustomFocus(bool bNewUseCustomFocus, bool bNewFocused = false);
	UFUNCTION(BlueprintCallable, Category = Button)
	void SetHiddenWhileUnfocused(bool bNewHidden);
	UFUNCTION(BlueprintCallable, Category = Button)
	void SetIsSelected(bool bNewSelected);

	// Return whether the button is currently focused
	bool IsFocused() const
	{
		return bIsFocused;
	}

protected:
	// Updates the focused state
	void UpdateIsFocused();
	// Called whenever the focused state has changed
	virtual void OnIsFocusedChanged();
	// Set the pressed state
	virtual bool SetIsPressed(bool bNewPressed);
	// Press virtual hooks
	virtual void OnPressed(const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bDoubleClick);
	virtual void OnReleased(const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bDoubleClick, bool bCancelled);
	// Called when the button was double clicked
	virtual void OnDoubleClicked(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	// Called when the button has been dragged
	virtual FReply OnDragged(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
	// Whether capturing should active high precision mouse movement
	virtual bool ShouldCaptureHighPrecisionMouseMovement() const;
	// Whether the mouse should be captured while the button is pressed
	virtual bool ShouldCaptureMouseWhenPressed() const;
	// Called when the button is pressed via the keyboard or the mouse
	FReply& PressButton(FReply& Reply, const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bDoubleClick);
	// Called when the button has been released
	FReply& ReleaseButton(FReply& Reply, const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bCancelled);
	// Returns the required reply for capturing the mouse
	virtual FReply& StartMouseCapture(FReply& Reply, const FPointerEvent* InMouseEvent);
	// Returns the reply to cancel mouse capture
	FReply& StopMouseCapture(FReply& Reply);

	// Returns whether the given pointer event is above the border
	bool IsPointerOverBorder(const FPointerEvent& PointerEvent) const;
	// Updates the style according to current focused and pressed state
	void UpdateButtonStyle();
	void UpdateButtonStyleInternal();

public:
	// Calls the delegate to update the content style
	UFUNCTION(BlueprintCallable)
	void UpdateContentStyle();
	// Get the buttons current style state
	UFUNCTION(BlueprintPure)
	virtual EBrickUIStyleState GetButtonStyleState() const;
	// Get the currently desired style state for content
	UFUNCTION(BlueprintPure)
	EBrickUIStyleState GetContentStyleState() const;

protected:
	// Can be implemented to customize the style or update content widgets
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateContentStyle(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InStyleState);
};
