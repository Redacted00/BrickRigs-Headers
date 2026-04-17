// Copyright Fluppisoft, 2020

#pragma once

#include "GenericTeamAgentInterface.h"
#include "UI/HUD/HUDWidgetInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchEndWidget.generated.h"

class UMenuButtonWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UMatchEndWidget : public UUserWidget, public IHUDWidgetInterface
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

protected:
	UFUNCTION(BlueprintCallable)
	void OnClickedContinue();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatchWinner(UBrickTeam* WinnerTeam, ETeamAttitude::Type TeamAttitude);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateContinueButton(int32 TimeRemaining, bool bIsHost);
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

private:
	// Callback for the game state
	void OnMatchTimerChanged(int32 NewTime);

	// ~IHUDWidgetInterface
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	// ~IHUDWidgetInterface
};
