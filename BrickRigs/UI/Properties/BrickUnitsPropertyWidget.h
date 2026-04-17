// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickUnitsSliderWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "BrickUnitsPropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickUnitsPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickUnitsSliderWidget* Slider;
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
	void OnSliderValueChanged(const FBrickUnits& NewValue, EValueChangedEventType EventType);
};
