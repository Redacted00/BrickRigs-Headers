// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickComboBoxWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "ObjectPropertyWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UObjectPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

	// ~Variables
	// Cached flags whether the icons and names should be shown for the objects
	bool bShowIcons;
	bool bShowNames;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickComboBoxWidget* BrickComboBox;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly)
	float IconSize = 128.f;
	// ~Properties

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void InitializeProperty() override;
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

protected:
	// Callbacks for the combo box
	UFUNCTION()
	virtual void InitializeItem(int32 Item, FBrickComboBoxItemParams& OutParams);
	UFUNCTION()
	void OnItemSelected(int32 Item, EValueChangedEventType EventType);
};
