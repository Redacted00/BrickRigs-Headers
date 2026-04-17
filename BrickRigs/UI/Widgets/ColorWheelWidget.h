// Copyright Fluppisoft, 2020

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColorWheelWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UColorWheelWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FColorChanged, const FLinearColor&, NewColor, EValueChangedEventType, EventType);

	// ~Variables
	// The currently selected color in HSV format
	FLinearColor SelectedColor;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = ColorWheel)
	FSlateBrush CursorBrush;
	// ~Properties

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FColorChanged OnColorChangedDelegate;
	// ~Delegates

	// ~Constructor
	UColorWheelWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	// ~Super Interface

	// Set the selected color
	UFUNCTION(BlueprintCallable)
	void SetColor(const FLinearColor& NewColor);

private:
	// Get the position of the cursor based on the selected color
	FVector2D GetRelativeSelectorPosition() const;
	// Apply the value at the current mouse position, return true if the event was valid
	bool ProcessMouseEvent(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, bool bProcessWhenOutsideColorWheel);
};
