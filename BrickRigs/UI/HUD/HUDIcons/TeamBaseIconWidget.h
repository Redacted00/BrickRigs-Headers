// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "TeamBaseIconWidget.generated.h"

class ATeamBase;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UTeamBaseIconWidget : public UHUDIconWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	UPROPERTY(Transient)
	ATeamBase* TeamBase;
	// ~Variables

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual FText GetIconDisplayName_Implementation() const override;
	// ~Super Interface
};
