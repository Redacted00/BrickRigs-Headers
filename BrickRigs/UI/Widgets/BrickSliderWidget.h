// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Misc/UIFunctionLibrary.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "DraggableButtonWidgetBase.h"
#include "BrickSliderWidget.generated.h"

class UBrickTextBoxWidget;

UENUM(BlueprintType)
enum class EBrickSliderGridSnapMode : uint8
{
	None,
	MinRelative,
	MaxRelative,
	Absolute
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickSliderWidget : public UDraggableButtonWidgetBase
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSliderValueChanged, float, Value, EValueChangedEventType, EventType);

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBoxWidget* TextBox;
	// ~Widgets

	// ~Properties
	// The current value displayed on the slider
	UPROPERTY(EditAnywhere, Category = Slider)
	float Value;
	// Minimum allowed value
	UPROPERTY(EditAnywhere, Category = Slider)
	float MinValue;
	// The maximum enterable value
	UPROPERTY(EditAnywhere, Category = Slider)
	float MaxValue;
	// The increment in which to add or remove when using arrow keys, also affects the drag speed. Zero means a default value will be used
	UPROPERTY(EditAnywhere, Category = Slider)
	float ValueStep;
	// If and how the value should be snapped to the value step
	UPROPERTY(EditAnywhere, Category = Slider)
	EBrickSliderGridSnapMode GridSnapMode;
	// Determines how the value is displayed
	UPROPERTY(EditAnywhere, Category = Slider)
	ENumericValueType ValueType;
	// Max number of fractional digits to be displayed
	UPROPERTY(EditAnywhere, Category = Slider, meta = (ClampMin = "-1"))
	int32 MaxFractionalDigits;
	// Optional format to apply to the displayed text (the value will be passed in as {Value})
	UPROPERTY(EditAnywhere, Category = Slider)
	FText CustomTextFormat;
	// Whether the slider should appear as read only
	UPROPERTY(EditAnywhere, Category = Slider)
	bool bIsReadOnly;
	// Whether the slider should be displayed as a marquee
	UPROPERTY(EditAnywhere, Category = Slider)
	bool bIsMarquee;
	// ~Properties

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnSliderValueChanged OnValueChangedDelegate;
	// ~Delegates

	// ~Constructor
	UBrickSliderWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool SetIsPressed(bool bNewPressed) override;
	virtual void SetIsDraggingContent(bool bNewDragging) override;
	virtual EBrickUIStyleState GetButtonStyleState() const override;
	virtual void OnUpdateContentStyle_Implementation(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InStyleState) override;
	virtual float GetTargetAnimPosition() const override;
	virtual float MouseMovementToAnimPosition(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, bool& bOutLockCursorToWidget) override;
	virtual void ApplyAnimPosition(float Pos, EValueChangedEventType EventType) override;
	virtual void CommitClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, bool bDoubleClick) override;
	virtual void UpdateAnimPosition() override;
	// ~Super Interface

	// Change the current value
	UFUNCTION(BlueprintCallable)
	void SetValue(float NewValue);
	// Return the current value
	UFUNCTION(BlueprintPure)
	float GetValue() const
	{
		return Value;
	}

	// Set the min and max value
	UFUNCTION(BlueprintCallable)
	void SetValueRange(float InMinValue, float InMaxValue);
	// Change the type of value displayed
	UFUNCTION(BlueprintCallable)
	void SetValueType(ENumericValueType InType);
	// Changes the grid snapping mode
	UFUNCTION(BlueprintCallable)
	void SetGridSnapMode(const EBrickSliderGridSnapMode NewMode);
	// Sets the maximum number of fractional digits to display
	UFUNCTION(BlueprintCallable)
	void SetMaxFractionalDigits(int32 InMaxDigits = -1);
	// Changes the custom text format
	UFUNCTION(BlueprintCallable)
	void SetCustomTextFormat(const FText& InFormat);
	UFUNCTION(BlueprintCallable)
	void SetIsReadOnly(bool bInReadOnly);
	UFUNCTION(BlueprintCallable)
	void SetIsMarquee(bool bInMarquee);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSliderPosition(float NewValue);
	// ~Blueprint Interface

private:
	// Handles key up and down events
	FReply HandleKeyEvent(const FKeyEvent& InKeyEvent, bool bWasPressed);
	// Callback for the user settings
	UFUNCTION()
	void OnMeasurementSystemChanged(EMeasurementSystem NewSystem);
	// Callback for the text box
	UFUNCTION()
	void OnTextChanged(const FText& NewText, EValueChangedEventType EventType);
	// Conver the given value to an anim position
	float ValueToAnimPosition(float InValue) const;
	// Convert the anim position to an absolute value
	float AnimPositionToValue(float InPos) const;
	// Update the displayed text only
	void UpdateSliderText(bool bOverrideTyping = false, bool bIsTyping = false);
	// Commit a new value, call all notifies
	bool CommitValue(float NewValue);
	// Internally set and commit a new value
	bool SetValueInternal(float NewValue, bool bClampValue, bool bCallNotify, EValueChangedEventType EventType);
	// Returns the value step to use
	float GetEffectiveValueStep() const
	{
		if (ValueStep > 0.f)
		{
			return ValueStep;
		}
		return UUIFunctionLibrary::GetDefaultValueStep(ValueType);
	}
};
