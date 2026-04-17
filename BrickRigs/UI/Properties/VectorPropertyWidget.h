// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickSliderWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "VectorPropertyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UVectorPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* XSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* YSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* ZSlider;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

private:
	// Widget callbacks
	UFUNCTION()
	void OnXSliderValueChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnYSliderValueChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnZSliderValueChanged(float NewValue, EValueChangedEventType EventType);
	// Used to unify code for all sliders
	void OnSliderValueChanged(float NewValue, EValueChangedEventType EventType, EAxis::Type Axis);
};
