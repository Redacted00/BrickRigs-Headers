// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Widgets/BrickComboBoxWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "EnumPropertyWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UEnumPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// List of enum entries
	TArray<FEnumPropertyItem> EnumItems;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBox;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void InitializeProperty() override;
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

private:
	// Callbacks for the combo box
	UFUNCTION()
	void InitializeItem(int32 Item, FBrickComboBoxItemParams& OutParams);
	UFUNCTION()
	void OnItemSelected(int32 Item, EValueChangedEventType EventType);
};
