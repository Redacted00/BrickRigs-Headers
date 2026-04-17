// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "NumericPropertyWidget.generated.h"

enum class EBrickUnitsDisplayMode : uint8;
class UBrickSliderWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UNumericPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	UPROPERTY(Transient)
	TArray<UBrickSliderWidget*> Sliders;
	// Stores value indices for all sliders
	TArray<int32> ValueIndices;
	// Whether axes are locked together
	bool bAxesLocked = false;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = NumericProperty)
	TSubclassOf<UBrickSliderWidget> SliderClass;
	// ~Properties

	// ~Constructor
	UNumericPropertyWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	virtual void InitializeProperty() override;
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual FName GetFocusedSubProperty(const FWidgetPath& FocusedWidgetPath) const override;
	// ~Super Interface

	UFUNCTION(BlueprintPure)
	bool AreAxesLocked() const
	{
		return bAxesLocked;
	}

protected:
	// Allows locking or unlocking the axes
	UFUNCTION(BlueprintCallable)
	void LockAxes(const bool bLock);

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddSliderWidget(UBrickSliderWidget* Slider);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateNumericProperty(const ENumericValueType ValueType, const int32 NumAxes);
	// ~Blueprint Interface

private:
	// Callback for the slider
	UFUNCTION()
	void OnSliderValueChanged(const float NewValue, const EValueChangedEventType EventType, const int32 Index);
};
