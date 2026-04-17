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

	// ~Variables
	// The associated option
	FInteractionOption InteractionOption;
	// The current color style
	EBrickUIColorStyle ColorStyle;
	bool bCanBeClicked;
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

	// Sets the associated interaction option
	void SetInteractionOption(const FInteractionOption& InOption, const FName& InActionName, EInputActionTriggerType InTriggerType, EBrickUIColorStyle InColorStyle, EHUDIconDomain InDomain);

protected:
	// Callback for the button
	UFUNCTION(BlueprintCallable)
	void OnClickedInteract();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle InColorStyle);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanBeClicked(bool bInCanBeClicked);
	// ~Blueprint Interface
};
