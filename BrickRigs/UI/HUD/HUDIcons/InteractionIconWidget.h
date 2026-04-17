// Copyright Fluppisoft, 2020

#pragma once

#include "Interaction/InteractionComponent.h"
#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "InteractionIconWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInteractionIconWidget : public UHUDIconWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	// ~Super Interface
};
