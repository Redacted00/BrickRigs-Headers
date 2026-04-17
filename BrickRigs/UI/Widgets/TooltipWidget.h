// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "TooltipWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UTooltipWidget : public UBrickUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The currently displayed content
	FTooltipContent CurrentTooltipContent;
	// ~Variables

public:
	// ~Constructor
	UTooltipWidget(const FObjectInitializer& ObjectInitializer);

	// Initializes the tooltip
	void NativeUpdateTooltipContent(const FTooltipContent& InContent);

	// Public accessor to the tooltip content
	const auto& GetCurrentTooltipContent() const
	{
		return CurrentTooltipContent;
	}

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UpdateTooltipContent(const FTooltipContent& InContent);
	// ~Blueprint Interface
};
