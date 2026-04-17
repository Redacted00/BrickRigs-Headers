// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickSliderWidget.h"
#include "Bricks/Misc/BrickUnits.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickUnitsSliderWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickUnitsSliderWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, const FBrickUnits&, NewValue, EValueChangedEventType, EventType);

	// ~Variables
	FBrickUnits Value;
	FBrickUnits MinValue;
	FBrickUnits MaxValue;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* UnitsSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* SubUnitsSlider;
	// ~Widgets

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnValueChanged OnSliderValueChangedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetValue(const FBrickUnits& NewValue);
	UFUNCTION(BlueprintCallable)
	void SetValueRange(const FBrickUnits& InMinValue, const FBrickUnits& InMaxValue);

private:
	// Updates the range of the sliders
	void UpdateValueRange();
	// Widget callbacks
	UFUNCTION()
	void OnUnitsSliderValueChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnSubUnitsSliderValueChanged(float NewValue, EValueChangedEventType EventType);
};
