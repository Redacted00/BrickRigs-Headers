// Copyright Fluppisoft, 2018

#pragma once

#include "UI/Menu/Popups/KickPlayerPopupWidget.h"
#include "ScoreboardPlayerWidget.h"
#include "ScoreboardTeamWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardWidget.generated.h"

class ABrickPlayerState;
class UGridPanel;
class UBrickTeam;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UScoreboardWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectedPlayerChanged, ABrickPlayerState*);

	// ~Variables
	// The team widgets currently in use, also ordered same as the display order
	UPROPERTY(Transient)
	TArray<UScoreboardTeamWidget*> TeamWidgets;
	// The player that is currently selected
	TWeakObjectPtr<ABrickPlayerState> SelectedPlayerState;
	// Cached team of the local player
	FGenericTeamId OwnTeam;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* TeamsPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	TSubclassOf<UScoreboardTeamWidget> TeamWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	TSubclassOf<UScoreboardPlayerWidget> PlayerWidgetClass;
	// Number of teams displayed in a single row on the grid panel
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	int32 NumTeamsPerRow;
	// Popup to open when a kick was requested
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	TSoftClassPtr<UKickPlayerPopupWidget> KickPopupClass;

	// Whether the player can join the teams through this scoreboard
	UPROPERTY(EditAnywhere, Category = Scoreboard)
	bool bCanJoinTeams;
	// ~Properties

public:
	// ~Delegates
	FOnSelectedPlayerChanged OnSelectedPlayerChangedDelegate;
	// ~Delegates

	// ~Constructor
	UScoreboardWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Get the player state from the selected widget
	ABrickPlayerState* GetSelectedPlayerState() const
	{
		return SelectedPlayerState.Get();
	}

private:
	// Update all team widget slots from the given index
	void UpdateTeamSlots(int32 FromIndex);
	// Returns the team widget associated with the given player
	UScoreboardTeamWidget* GetTeamWidgetForPlayer(const ABrickPlayerState* PlayerState) const;
	// Add or remove a player, internal versions to be called from the delegates or initialization
	void AddPlayerState(ABrickPlayerState* PlayerState, bool bUpdateSlots);
	void RemovePlayerState(ABrickPlayerState* PlayerState, bool bUpdateSlots);
	// Removes the player fromm a specific team
	void RemovePlayerStateFromTeam(ABrickPlayerState* PlayerState, UScoreboardTeamWidget* TeamWidget, bool bUpdateSlots);
	// Adds a team widget
	UScoreboardTeamWidget* AddTeamWidget(const FGenericTeamId& TeamId, UBrickTeam* Team, bool bIsPermanentTeam, bool bUpdateSlots);
	// Set the currently selected player, takes care of updating the style
	void SetSelectedPlayerState(ABrickPlayerState* NewPlayer, bool bIsDestructing = false);

	// Callback for the owning player state
	void OnOwnTeamChanged(const FGenericTeamId& NewTeam);
	// Callbacks for the game state
	void OnPlayerAddedOrRemoved(ABrickPlayerState* PlayerState, bool bAdded);
	// Callbacks for the player widgets
	void OnPlayerClicked(UScoreboardPlayerWidget* Widget);
	void OnPlayerChangedTeam(UScoreboardPlayerWidget* Widget);
};
