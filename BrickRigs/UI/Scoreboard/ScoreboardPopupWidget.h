// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "ScoreboardPopupWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UScoreboardPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

public:
	// ~Constructor
	UScoreboardPopupWidget(const FObjectInitializer& ObjectInitializer);
};
