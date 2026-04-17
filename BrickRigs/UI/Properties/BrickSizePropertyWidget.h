// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickUnitsSliderWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "BrickSizePropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickSizePropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickUnitsSliderWidget* XSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickUnitsSliderWidget* YSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickUnitsSliderWidget* ZSlider;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual FName GetFocusedSubProperty(const FWidgetPath& FocusedWidgetPath) const override;
	// ~Super Interface

private:
	// Widget callbacks
	UFUNCTION()
	void OnXSliderValueChanged(const FBrickUnits& NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnYSliderValueChanged(const FBrickUnits& NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnZSliderValueChanged(const FBrickUnits& NewValue, EValueChangedEventType EventType);
	// Used to unify code for all sliders
	void OnSliderValueChanged(const FBrickUnits& NewValue, EValueChangedEventType EventType, EAxis::Type Axis);
};
