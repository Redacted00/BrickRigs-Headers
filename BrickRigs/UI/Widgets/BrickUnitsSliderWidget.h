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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnValueChanged, const FBrickUnits&, NewValue, EValueChangedEventType, EventType);

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* Slider;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Slider)
	FBrickUnits Value;
	UPROPERTY(EditDefaultsOnly, Category = Slider)
	FBrickUnits MinValue;
	UPROPERTY(EditDefaultsOnly, Category = Slider)
	FBrickUnits MaxValue;
	// ~Properties

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnValueChanged OnSliderValueChangedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetValue(const FBrickUnits& NewValue);
	UFUNCTION(BlueprintCallable)
	void SetValueRange(const FBrickUnits& InMinValue, const FBrickUnits& InMaxValue);
	UFUNCTION(BlueprintCallable)
	void CycleUnitsDisplayMode();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUnitsDisplayMode(const EBrickUnitsDisplayMode NewMode);
	// ~Blueprint Interface

private:
	// Updates the displayed value
	void UpdateValue();
	// Widget callbacks
	UFUNCTION()
	void OnSliderValueChanged(float NewValue, EValueChangedEventType EventType);
};
