// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "NumericPropertyWidget.generated.h"

class UBrickSliderWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UNumericPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* Slider;
	// ~Widgets

public:
	// ~Constructor
	UNumericPropertyWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void InitializeProperty() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

private:
	// Callback for the slider
	UFUNCTION()
	void OnSliderValueChanged(float NewValue, EValueChangedEventType EventType);
};
