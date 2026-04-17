// Copyright Fluppisoft, 2020

#pragma once

#include "InteractionOptionWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionWidget.generated.h"

class UHUDIconCanvasWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The owning containerwidget
	TWeakObjectPtr<UHUDIconCanvasWidget> HUDIconPanel;
	UPROPERTY(Transient)
	TArray<UInteractionOptionWidget*> OptionWidgets;
	// The current color style
	EBrickUIColorStyle ColorStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	TSubclassOf<UInteractionOptionWidget> OptionWidgetClass;
	// ~Properties

public:
	// Sets the owning widget
	void SetHUDIconPanel(UHUDIconCanvasWidget* InWidget);
	// Updates the interaction options
	void SetInteractionOptions(const FInteractionOptions& InOptions, EBrickUIColorStyle InColorStyle);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddOptionWidget(UInteractionOptionWidget* Widget, int32 OptionIndex);
	// ~Blueprint Interface
};
