// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "GenericPropertyWidget.generated.h"

class UBrickTextBoxWidget;

/**
 * Used to display simple properties like user names and dates, for all property types that don't have a specific widget class
 */
UCLASS(Abstract)
class BRICKRIGS_API UGenericPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickTextBoxWidget* TextBox;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void UpdateIsReadOnly_Implementation(bool bNewReadOnly) override;
	// ~Super Interface

protected:
	// Callback for the text box
	UFUNCTION()
	void OnTextChanged(const FText& Text, EValueChangedEventType EventType);
};
