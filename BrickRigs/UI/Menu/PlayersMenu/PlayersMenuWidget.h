// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Menu/Popups/KickPlayerPopupWidget.h"
#include "CoreMinimal.h"
#include "UI/Menu/MenuPageWidget.h"
#include "PlayersMenuWidget.generated.h"

class UScoreboardWidget;
class ABrickPlayerState;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPlayersMenuWidget : public UMenuPageWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UScoreboardWidget* Scoreboard;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	// ~Super Interface

	// Show the profile of the selected player
	UFUNCTION(BlueprintCallable)
	void ShowPlayerProfile();
	// Kick the currently selected player
	UFUNCTION(BlueprintCallable)
	void KickPlayer();

protected:
	// Used to update the buttons
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanShowPlayerProfile(bool bCanShow);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanKickPlayer(bool bCanKick);

private:
	// Callback for the scoreboard
	void OnSelectedPlayerChanged(ABrickPlayerState* NewSelectedPlayer);
	// Get the currently selected player from the scoreboard
	ABrickPlayerState* GetSelectedPlayerState() const;
	// Whether the owning player could kick the other one
	bool CanKickPlayer(ABrickPlayerState* OtherPlayer) const;
};
