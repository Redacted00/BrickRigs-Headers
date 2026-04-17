// Copyright Fluppisoft, 2019

#pragma once

#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "UI/Menu/MenuPageWidget.h"
#include "MenuSettingsPageWidget.generated.h"

class UPropertiesPanelWidget;

/**
 * Implements some shared functionality for settings pages
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuSettingsPageWidget : public UMenuPageWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Page)
	FGameplayTagContainer SettingsCategories;
	// ~Properties

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	// ~Super Interface

protected:
	// Updates the list of displayed properties
	void UpdateProperties();
	// Called to add all properties to the properties panel
	UFUNCTION(BlueprintNativeEvent)
	void AddProperties();
};
