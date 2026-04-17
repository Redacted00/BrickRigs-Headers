// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "UI/Misc/NumericValueType.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardSliderWidget.generated.h"

class UBrickSliderWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UDashboardSliderWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The cached color style, used to avoid updates
	EBrickUIColorStyle ColorStyle;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* Slider;
	// ~Widgets

public:
	// ~Constructor
	UDashboardSliderWidget(const FObjectInitializer& ObjectInitializer);

	// Initializes the slider attributes
	void InitializeSlider(const FFloatInterval& ValueRange, ENumericValueType ValueType, int32 IconIndex);
	// Updates the slider value
	void UpdateSlider(float NewValue, EBrickUIColorStyle NewStyle);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle NewStyle);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIcon(int32 IconIndex);
	// ~Blueprint Interface
};
