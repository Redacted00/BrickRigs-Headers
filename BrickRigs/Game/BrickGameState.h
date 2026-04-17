// Copyright Fluppisoft, 2016

#pragma once

#include "Player/PlayerSpawnRequest.h"
#include "Misc/BrickTimer.h"
#include "Misc/BrickTeam.h"
#include "MatchWinner.h"
#include "Properties/BrickPropertyInterface.h"
#include "GameFramework/GameState.h"
#include "Settings/MatchSettings.h"
#include "BrickGameState.generated.h"

// Actors
class ABrickPlayerController;
class ABrickPlayerState;
// Components
class ULoadoutInventoryComponent;

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickGameState : public AGameState, public IBrickPropertyInterface
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPlayerAddedOrRemoved, ABrickPlayerState*, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchStateChanged, const FName&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchTimerChanged, int32);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMatchSettingsChanged, const FMatchSettings&);

	// ~Variables
	FBrickTimer Timer_RevertNextMatchSettings;
	// World time when the elapsed time variable has been reset
	float ElapsedTimeResetTime;
	// Number of game mode specific sub levels that are still pending load
	int32 NumGameModeSubLevelsLoading;

	// Settings being used in the current round
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MatchSettings)
	FMatchSettings MatchSettings;
	UFUNCTION()
	void OnRep_MatchSettings();
	// Settings that are going to be used next round
	UPROPERTY(Transient, ReplicatedUsing = OnRep_NextMatchSettings)
	FMatchSettings NextMatchSettings;
	UFUNCTION()
	void OnRep_NextMatchSettings();
	// Settings as they have been replicated, saved so they can be restored
	FMatchSettings SavedNextMatchSettings;
	// Per match random seed, used to sync randomized weather etc. for all clients
	UPROPERTY(Transient, Replicated)
	uint8 MatchRandomSeed;

	// List of replicated teams
	UPROPERTY(Transient)
	TArray<UBrickTeam*> Teams;
	// Instance of the default team
	UPROPERTY(Transient)
	UBrickTeam* DefaultTeam;
	// Instance of the zombie team
	UPROPERTY(Transient)
	UBrickTeam* ZombieTeam;
	// Instance of the dummy team
	UPROPERTY(Transient)
	UBrickTeam* DummyTeam;

	// The team that won the current match/round
	UPROPERTY(ReplicatedUsing = OnRep_MatchWinner)
	FMatchWinner MatchWinner;
	UFUNCTION()
	void OnRep_MatchWinner();

	// The index of the current round
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentRound)
	uint16 CurrentRound;
	UFUNCTION()
	void OnRep_CurrentRound();

	// This is the array of registered spawn actors in the level, they need to report themselves
	UPROPERTY(Transient)
	TArray<AActor*> SpawnPointArray;

	// Whether the teams have been initialized
	uint8 bInitializedTeams : 1;
	// ~Variables

	// ~Components
	// The loadout inventory component used for the entire level, for loadout containers and for accessing the loadout anywhere in sandbox mode
	UPROPERTY(Transient)
	ULoadoutInventoryComponent* LoadoutInventoryComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = GameState)
	float ExitMatchDelay;
	// ~Properties

	// ~Delegates
	FOnPlayerAddedOrRemoved OnPlayerAddedOrRemovedDelegate;
	FOnMatchStateChanged OnMatchStateChangedDelegate;
	FOnMatchTimerChanged OnMatchTimerChangedDelegate;
	FOnMatchSettingsChanged OnMatchSettingsChangedDelegate;
	FOnMatchSettingsChanged OnNextMatchSettingsChangedDelegate;
	// ~Delegates

	// ~Constructor
	ABrickGameState();

	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void PreNetReceiveSubobjects(const FReplicationFlags& RepFlags) override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void HandleBeginPlay() override;
	virtual void Destroyed() override;
	virtual void AddPlayerState(APlayerState* PlayerState) override;
	virtual void RemovePlayerState(APlayerState* PlayerState) override;
	virtual void DefaultTimer() override;
	virtual void OnRep_MatchState() override;
	virtual void OnRep_ElapsedTime() override;
	virtual void OnRep_ReplicatedWorldTimeSeconds() override;
	virtual void ReceivedGameModeClass() override;
	virtual void ReceivedSpectatorClass() override;
	virtual float GetPlayerRespawnDelay(AController* Controller) const override;
	// ~Super Interface

	// ~Inventory
	// Get the loadout inventory instance
	ULoadoutInventoryComponent* GetLoadoutInventoryComponent() const
	{
		return LoadoutInventoryComponent;
	}

	// ~Inventory

	// ~Game State
	// Get the brick game state instance from the world
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Game State"))
	static ABrickGameState* Get(const UObject* WorldContextObject);

	// Called from the game mode upon initialization
	void InitGameState(const FMatchSettings& InMatchSettings);
	// Returns the per match random seed, can be used to sync random events for all clients
	UFUNCTION(BlueprintPure)
	uint8 GetMatchRandomSeed() const
	{
		return MatchRandomSeed;
	}

	// Get the current match/round winner
	UFUNCTION(BlueprintPure)
	const FMatchWinner& GetMatchWinner() const
	{
		return MatchWinner;
	}

	// Sets the replicated match winner, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetMatchWinner(const FMatchWinner& InWinner);
	// Get the match settings for the current round
	UFUNCTION(BlueprintPure)
	const FMatchSettings& GetMatchSettings() const
	{
		return MatchSettings;
	}

	// Get the match settings for the next round
	UFUNCTION(BlueprintPure)
	const FMatchSettings& GetNextMatchSettings() const
	{
		return NextMatchSettings;
	}

	// Sets the current match settings, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetMatchSettings(const FMatchSettings& NewSettings);
	// Sets the settings for the next match, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetNextMatchSettings(const FMatchSettings& NewSettings);

	// Verifies match settings according to the desired net mode
	FMatchSettings VerifyMatchSettings(const FMatchSettings& NewSettings, bool bIsMultiplayer) const;

private:
	void OnMatchSettingsChanged(bool bChangedRemotely);
	void OnNextMatchSettingsChanged(bool bChangedRemotely);

	// Reverts next match settings to the replicated version on the client
	void RevertNextMatchSettings();

public:
	// Get the seconds remaining for the current timer
	UFUNCTION(BlueprintPure)
	int32 GetMatchTimerRemaining() const;

protected:
	// Get the duration the timer is currently counting down to
	UFUNCTION(BlueprintNativeEvent)
	int32 GetMatchTimerRate() const;
	// Called when the elapsed time has passed GetMatchTimerRate()
	UFUNCTION(BlueprintNativeEvent)
	void OnMatchTimerElapsed();
	// Setup the elapsed time timer
	void SetupDefaultTimer();
	// Called when the elapsed timer has been reset to 0, or has been replicated
	void OnElapsedTimeReset();
	// Called whenever the elapsed time variable has changed
	void OnElapsedTimeChanged();
	// Match state handlers
	virtual void HandleRoundWaitingToStart();
	virtual void HandleRoundHasStarted();
	virtual void HandleRoundHasEnded();
	virtual void HandleIntermission();
	// Callback for the game mode specific sub levels
	UFUNCTION()
	void OnGameModeSubLevelLoaded();

public:
	// Whether all of the game mode specific sub levels have been loaded yet
	bool AreAllGameModeSubLevelsLoaded();
	// Return the time the player should remain dead before unpossessing
	float GetPlayerUnpossessDelay(AController* Controller) const;
	// Whether players and vehicles are allowed to use infinite ammo
	bool IsInfiniteAmmoAllowed() const;
	// Returns the number of players needed for warmup to start
	UFUNCTION(BlueprintPure)
	int32 GetNumPlayersToStartWarmup() const;
	// ~Game State

	// ~Rounds
	// Get the current round index
	UFUNCTION(BlueprintPure)
	int32 GetCurrentRound() const
	{
		return CurrentRound;
	}

	// Updates the round counter
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetCurrentRound(int32 NewRound);

private:
	void OnCurrentRoundChanged();
	// ~Rounds

public:
	// ~Vehicles
	// Get the cost for a vehicle with the given metrics
	UFUNCTION(BlueprintPure)
	float GetVehiclePrice(const FVehicleSpawnProperties& Props) const;
	// Returns the maximum number of vehicles each player is allowed to spawn
	int32 GetMaxVehiclesPerPlayer(const ABrickPlayerController* PC) const;
	// Returns the maximum number of bricks the player is allowed to spawn per vehicle
	int32 GetMaxBricksPerVehicle(const ABrickPlayerController* PC) const;
	// Returns the maximum size in any dimension
	float GetMaxVehicleSize(const ABrickPlayerController* PC) const;
	// Returns the maximum mass allowed to spawn
	int32 GetMaxVehicleMass(const ABrickPlayerController* PC) const;
	// Whether the player is allowed to spawn the given vehicle, server and client
	EPlayerSpawnResult CanPlayerSpawnVehicle(ABrickPlayerController* PC, const FUGCFileInfo& FileInfo, bool bRemoveCurrent) const;
	// Internal version that requires spawn information about the vehicle
	EPlayerSpawnResult CanPlayerSpawnVehicleInternal(ABrickPlayerController* PC, const FVehicleSpawnProperties& Props) const;
	// Whether vehicles are allowed to consume infinite fuel
	bool IsInfiniteFuelAllowed() const;
	// ~Vehicles

	// ~Teams
	// Get all present teams
	UFUNCTION(BlueprintPure)
	const TArray<UBrickTeam*>& GetTeams() const
	{
		return Teams;
	}

	// Get the default team instance
	UFUNCTION(BlueprintPure)
	UBrickTeam* GetDefaultTeam() const
	{
		return DefaultTeam;
	}

	// Get the default zombie team instance, if it exists
	UFUNCTION(BlueprintPure)
	UBrickTeam* GetZombieTeam() const
	{
		return ZombieTeam;
	}

	// Get the default dummy team instance, if it exists
	UFUNCTION(BlueprintPure)
	UBrickTeam* GetDummyTeam() const
	{
		return DummyTeam;
	}

	// Get the team for a specific id
	UFUNCTION(BlueprintCallable)
	UBrickTeam* FindTeam(const FGenericTeamId& TeamId) const;
	// Return whether a team of the given id exists
	UFUNCTION(BlueprintPure)
	bool DoesTeamExist(const FGenericTeamId& TeamId) const;
	// Get the number of players in the given team
	UFUNCTION(BlueprintPure)
	int32 GetNumPlayersInTeam(const FGenericTeamId& TeamId) const;
	// Get the maximum allowed player count for the given team
	UFUNCTION(BlueprintPure)
	int32 GetMaxPlayersInTeam(const FGenericTeamId& TeamId) const;
	// Get the number of player slots available for a team
	UFUNCTION(BlueprintPure)
	int32 GetNumFreeSlotsInTeam(const FGenericTeamId& TeamId) const;
	// Whether the given team id is valid for the player
	UFUNCTION(BlueprintPure)
	bool CanPlayerJoinTeam(ABrickPlayerController* PC, const FGenericTeamId& TeamId) const;

private:
	// Creates and initializes the team objects
	void InitializeTeams();
	// ~Teams

public:
	// ~Spawn Points
	// Public accessor to the current array of spawn points
	const TArray<AActor*>& GetSpawnPointArray() const;
	// Add a spawn point to the array
	void AddSpawnActor(AActor* Actor);
	// Remove a spawn point
	void RemoveSpawnActor(AActor* Actor);
	// ~Spawn Points

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface
};
