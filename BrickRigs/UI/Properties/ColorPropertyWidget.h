// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "ColorPropertyWidget.generated.h"

class UColorWheelWidget;
class UColorPaletteWidget;
class UBrickSliderWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UColorPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Whether the alpha slider is currently visible
	bool bIsAlphaSliderVisible;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UColorWheelWidget* ColorWheel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UColorPaletteWidget* ColorPalette;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* HueSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* SaturationSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* ValueSlider;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* AlphaSlider;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCurrentColor(const FLinearColor& NewColor);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateAlphaSliderVisibility(bool bNewVisible);
	// ~Blueprint Interface

private:
	// Updates the selected color
	void UpdateCurrentColor(const FBrickColorWithAlpha& NewColor);
	// Used to change the color property value
	void SetColorPropertyValue(EValueChangedEventType EventType, const FBrickColorWithAlpha& NewColor);

	// Widget callbacks
	UFUNCTION()
	void OnWheelColorChanged(const FLinearColor& NewColor, EValueChangedEventType EventType);
	UFUNCTION()
	void OnPaletteColorChanged(const FLinearColor& NewColor, EValueChangedEventType EventType);
	UFUNCTION()
	void OnHueChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnSaturationChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnValueChanged(float NewValue, EValueChangedEventType EventType);
	UFUNCTION()
	void OnAlphaChanged(float NewValue, EValueChangedEventType EventType);

	// Useful to set hue, satuation or the value
	template <typename FuncType>
	void SetColorChannel(float NewValue, EValueChangedEventType EventType, FuncType SetFunction)
	{
		FBrickColorWithAlpha NewColor;
		if (GetPropertyValue<FColorBrickProperty, FBrickColorWithAlpha>(NewColor))
		{
			(NewColor.*SetFunction)(NewValue);
			SetColorPropertyValue(EventType, NewColor);
		}
	}
};
