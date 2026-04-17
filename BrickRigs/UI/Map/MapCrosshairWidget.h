// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapCrosshairWidget.generated.h"

/**
 * Represents a single line of the crosshair
 */
UCLASS(Abstract)
class BRICKRIGS_API UMapCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Updates the color style
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	// Updates the style state
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStyleState(EBrickUIStyleState NewState);
};
