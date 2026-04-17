// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickComboBoxWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "ConnectorSpacingPropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UConnectorSpacingPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxXPos;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxXNeg;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxYPos;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxYNeg;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxZPos;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBoxZNeg;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual FName GetFocusedSubProperty(const FWidgetPath& FocusedWidgetPath) const override;
	// ~Super Interface

private:
	// Callbacks for the combo boxes
	UFUNCTION()
	void InitializeComboBoxItem(int32 Item, FBrickComboBoxItemParams& OutParams);
	UFUNCTION()
	void OnComboBoxXPos(int32 Item, EValueChangedEventType EventType);
	UFUNCTION()
	void OnComboBoxXNeg(int32 Item, EValueChangedEventType EventType);
	UFUNCTION()
	void OnComboBoxYPos(int32 Item, EValueChangedEventType EventType);
	UFUNCTION()
	void OnComboBoxYNeg(int32 Item, EValueChangedEventType EventType);
	UFUNCTION()
	void OnComboBoxZPos(int32 Item, EValueChangedEventType EventType);
	UFUNCTION()
	void OnComboBoxZNeg(int32 Item, EValueChangedEventType EventType);
	// Combined event that is used by all combo boxes
	void OnComboBoxItemSelected(int32 Item, EValueChangedEventType EventType, EAxis::Type Axis, bool bPositiveAxis);
};
