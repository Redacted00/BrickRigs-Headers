// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Input/InputActionTriggerType.h"
#include "Interaction/InteractionOptions.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractionOptionWidget.generated.h"

class UControlHintWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInteractionOptionWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The associated option
	FInteractionOption InteractionOption;
	// Whether the keybind for the action should be shown
	bool bShowKeybind;
	// The current color style
	EBrickUIColorStyle ColorStyle;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UControlHintWidget* ControlHintWidget;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Initializes the widget
	void InitializeInteractionOption(EHUDIconDomain InDomain);
	// Sets the associated interaction option
	void SetInteractionOption(const FInteractionOption& InOption, const FName& InActionName, EInputActionTriggerType InTriggerType, EBrickUIColorStyle InColorStyle);

protected:
	// Callback for the button
	UFUNCTION(BlueprintCallable)
	void OnClickedInteract();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle InColorStyle);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanBeClicked(bool bCanBeClicked);
	// ~Blueprint Interface
};
