// Copyright Fluppisoft, 2018

#pragma once

#include "GenericTeamAgentInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Misc/BrickUIStyleTypes.h"
#include "ScoreboardTeamWidget.generated.h"

class ABrickPlayerState;
class UScoreboardPlayerWidget;
class UBrickTextBlock;
class UBrickGridPanel;
class UBrickTeam;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UScoreboardTeamWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The team this widget represents
	FGenericTeamId TeamId;
	// The associated team
	UPROPERTY(Transient)
	UBrickTeam* BrickTeam;
	// All player widgets added to this team
	UPROPERTY(Transient)
	TArray<UScoreboardPlayerWidget*> PlayerWidgets;
	// Whether the team should still be displayed when its empty
	uint8 bKeepEmptyTeam : 1;
	// Whether the join button can ever be used on this widget
	uint8 bCanEverJoinTeam : 1;
	// Whether the player can currently join the team
	uint8 bCanJoinTeam : 1;
	// Whether players have to be resorted
	uint8 bPlayerOrderDirty : 1;
	// Last time the players have been sorted
	float LastSortTime;
	// Last time the join button state has been updated
	float LastJoinButtonUpdateTime;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* TeamNameTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* ScoreTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickGridPanel* PlayersPanel;
	// ~Widgets

	// ~Properties
	// Number of players to add per row
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	int32 NumPlayersPerRow;
	// Minimum delay between sorting the players list
	UPROPERTY(EditDefaultsOnly, Category = Scoreboard)
	float SortPlayersDelay;
	// ~Properties

public:
	// ~Constructor
	UScoreboardTeamWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	// ~Super Interface

	// Sets whether joining is possible within the scoreboard
	void SetCanEverJoinTeam(bool bCanJoin);
	// Initializes the team widget
	void InitializeTeam(const FGenericTeamId& InTeamId, UBrickTeam* InBrickTeam, bool bInKeepIfEmpty);
	// Get the associated team
	const FGenericTeamId& GetTeamId() const;
	// Update the team attitude appearance
	void UpdateTeamAttitude(const FGenericTeamId& OtherTeam);
	// Adds a player widget to the team
	void AddPlayerWidget(UScoreboardPlayerWidget* PlayerWidget, bool bUpdateSlots);
	// Removes a player from the team
	void RemovePlayerState(ABrickPlayerState* PlayerState, bool bUpdateSlots);
	// Called after all players have been added initially
	void PostInitializePlayers();
	// Get the number of players on this widget
	int32 GetNumPlayers() const;
	// Whether the team should be removed (is empty and not permanent)
	bool ShouldRemoveTeam() const;
	// Queues a player sorting update
	void MarkPlayerOrderDirty();
	// Sort the player list
	void SortPlayers();
	// Used to find the sort order for teams
	bool SortTeam(const UScoreboardTeamWidget& Other) const;
	// Updates the selection state of all players
	void UpdateSelectedPlayer(ABrickPlayerState* PlayerState);

	// Lets the player join this team
	UFUNCTION(BlueprintCallable)
	void JoinTeam();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanEverJoinTeam(bool bCanJoin);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanJoinTeam(bool bCanJoin);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamAttitude(bool bIsOwnTeam, ETeamAttitude::Type NewAttitude);
	// ~Blueprint Interface

private:
	// Sets the associated brick team and binds/unbinds delegates
	void SetBrickTeam(UBrickTeam* InBrickTeam);
	// Callbacks for the brick team
	void OnScoreChanged(int32 NewScore);
	void OnScoreChangedInternal(int32 NewScore);
	// Update the displayed score
	void UpdateScore();
	// Updates the join button
	void UpdateJoinButton();
	// Update all players widget slots from the given index
	void UpdatePlayerSlots(int32 FromIndex);
};
