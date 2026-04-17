// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "CheatMenuWidget.generated.h"

class UPropertiesPanelWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UCheatMenuWidget : public UPopupWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual FText GetPopupTitle_Implementation() const override;
	// ~Super Interface
};
