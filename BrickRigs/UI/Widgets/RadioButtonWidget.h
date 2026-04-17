// Copyright Fluppisoft 2018

#pragma once

#include "BrickButtonWidget.h"
#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "RadioButtonWidget.generated.h"

class UBrickButtonWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API URadioButtonWidget : public UBrickUserWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Binds a custom on clicked function to the buttton
	void BindOnClicked(const UBrickButtonWidget::FOnClicked::FDelegate& Delegate);
	// This can be implemented by the blueprint to customize the selected appearance
	UFUNCTION(BlueprintNativeEvent)
	void SetIsSelected(bool bNewSelected);

protected:
	// This has to be called when the check box or button was pressed or released
	UFUNCTION(BlueprintCallable)
	void OnRadioButtonClicked();
};
