// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "MapPopupWidget.generated.h"

class UMapWidget;
class UHUDIconComponent;

/**
 * This is used as a container for the actual map widget to display in game
 */
UCLASS(Abstract)
class BRICKRIGS_API UMapPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UMapWidget* MapWidget;
	// ~Widgets

public:
	// ~Constructor
	UMapPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;
	virtual UWidget* GetWidgetToFocus_Implementation() override;
	// ~Super Interface
};
