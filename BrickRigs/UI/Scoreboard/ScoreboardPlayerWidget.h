// Copyright Fluppisoft, 2018

#pragma once

#include "GenericTeamAgentInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreboardPlayerWidget.generated.h"

class ABrickPlayerState;
class UScoreboardTeamWidget;
class UBrickButtonWidget;
class UBrickTextBlock;
class UPingIndicatorWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UScoreboardPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FScoreboardPlayerDelegate, UScoreboardPlayerWidget*);

	// ~Variables
	// The owning team widget
	TWeakObjectPtr<UScoreboardTeamWidget> TeamWidget;
	// The index at which the widget is sorted into the team
	int32 SortIndex;
	// Whether the player is currently selected
	bool bIsSelected;
	// Player this widget represents
	UPROPERTY(Transient)
	ABrickPlayerState* PlayerState;
	// The team id of the player
	FGenericTeamId TeamId;
	// Team id of the local player
	FGenericTeamId OwnTeamId;
	// The cached player name, used for sorting
	FString PlayerName;
	// The cached score
	float Score;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* PositionTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* KillsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* DeathsTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* ScoreTextBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UPingIndicatorWidget* PingIndicator;
	// ~Widgets

public:
	// ~Delegates
	FScoreboardPlayerDelegate OnPlayerClickedDelegate;
	FScoreboardPlayerDelegate OnPlayerChangedTeamDelegate;
	// ~Delegates

	// ~Constructor
	UScoreboardPlayerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Sets the owning team widget
	void SetTeamWidget(UScoreboardTeamWidget* InWidget);
	// Get the outer team widget
	UScoreboardTeamWidget* GetTeamWidget() const;
	// Set the player state associated with this widget
	void InitializePlayer(ABrickPlayerState* InPlayerState, const FGenericTeamId& InOwnTeamId);
	// Get our corrosponding player state
	ABrickPlayerState* GetPlayerState() const;
	// Get the team id of the owning player
	const FGenericTeamId& GetTeamId() const;
	// Update the team attitude appearance
	void UpdateTeamAttitude(const FGenericTeamId& OtherTeam);
	// Sets the current sort index, updates the rank etc.
	void SetSortIndex(int32 NewIndex);
	// Get the current sort index
	int32 GetSortIndex() const;
	// Used by the team widget to sort the players list
	bool SortPlayer(const UScoreboardPlayerWidget& Other) const;
	// Sets the selected state and updates the appearance
	void SetIsSelected(bool bNewSelected);

	// Callback for the button
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsAlive(bool bIsAlive);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsAdmin(bool bIsAdmin);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateButtonStyle(bool bNewSelected, bool bIsLocalPlayer, ETeamAttitude::Type TeamAttitude);
	// ~Blueprint Interface

private:
	// Sets the associated player state
	void SetPlayerState(ABrickPlayerState* InPlayerState);
	// Calls the blueprint version with all the parameters
	void UpdateButtonStyle();
	// Callbacks for the player state
	void OnPlayerNameChanged(const FString& NewName);
	void OnPlayerNameChangedInternal(const FString& NewName);
	void OnScoreChanged(float NewScore);
	void OnScoreChangedInternal(float NewScore);
	void OnKillsChanged(int32 NewKills);
	void OnDeathsChanged(int32 NewDeaths);
	void OnTeamChanged(const FGenericTeamId& NewTeam);
	void OnTeamChangedInternal(const FGenericTeamId& NewTeam);
	void OnIsAliveChanged(bool bNewAlive);
};
