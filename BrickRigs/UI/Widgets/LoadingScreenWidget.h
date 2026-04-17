// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "MainWidgetBase.h"
#include "LoadingScreenWidget.generated.h"

/**
 * Widgets of this type can be displayed as the loading screen from the game instance
 */
UCLASS(Abstract)
class BRICKRIGS_API ULoadingScreenWidget : public UMainWidgetBase
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativePreConstruct() override;
	// ~Super Interface

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void SetLoadingText(const FText& Text);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void SetBackgroundColor(const FLinearColor& InColor);
	// ~Blueprint Interface
};
