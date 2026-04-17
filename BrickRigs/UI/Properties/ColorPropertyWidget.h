// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "ColorPropertyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UColorPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetColorPropertyValue(const FColor Color, const EValueChangedEventType EventType);
	UFUNCTION(BlueprintPure)
	bool HasAlphaChannel() const;

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorPropertyValue(const FColor& NewColor, const bool bValueChanged);
	// ~Blueprint Interface
};
