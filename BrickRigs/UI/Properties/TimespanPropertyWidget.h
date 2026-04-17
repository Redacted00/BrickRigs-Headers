// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "TimespanPropertyWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UTimespanPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	// Sets the property value
	UFUNCTION(BlueprintCallable)
	void SetTimespanPropertyValue(EValueChangedEventType EventType, const FTimespan& NewValue);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimespanProperty(const FTimespan& Timespan, const FTimespan& MinValue, const FTimespan& MaxValue, bool bAllowInfinite);
	// ~Blueprint Interface
};
