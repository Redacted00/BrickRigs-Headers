#pragma once

#include "Misc/BrickTimer.h"
#include "MatchWinner.h"
#include "BrickGameState.h"
#include "Vehicle/BrickVehicle.h"
#include "Player/BrickPlayerController.h"
#include "GameFramework/GameMode.h"
#include "BrickGameMode.generated.h"

namespace BrickMatchState
{
	extern const FName EnteringMap; // We are entering this map, actors are not yet ticking
	extern const FName WaitingToStart; // Actors are ticking, but the match has not yet started
	extern const FName WaitingForPlayers; // Warmup phase to run around while waiting for enough players
	extern const FName Warmup; // Countdown before the first round is started
	extern const FName RoundWaitingToStart; // Pre round warmup phase, players are optionally frozen
	extern const FName RoundInProgress; // The round is in progress
	extern const FName RoundEnded; // The round has ended and is about to be exited
	extern const FName Intermission; // Intermission time between rounds
	extern const FName MatchEnd; // Match has ended so we aren't accepting new players, but actors are still ticking
	extern const FName LeavingMap; // We are transitioning out of the map to another location
	extern const FName Aborted; // Match has failed due to network issues or other problems, cannot continue
}

// Actors
class ABrickBuilding;
class AZombie;
class ACapturePoint;
class ABrickPlayerStart;

UCLASS(Abstract)
class BRICKRIGS_API ABrickGameMode : public AGameMode
{
	GENERATED_BODY()

	// ~Variables
	// Used to delay disabling players after the match has ended
	FBrickTimer Timer_UnPossessPlayers;
	// Used to delay restarting the game
	FBrickTimer Timer_RestartGame;
	// Whether the match state is being set
	uint8 bIsSettingMatchState : 1;
	// Set to true during the HandleMatchHasStarted function
	uint8 bIsHandlingMatchStart : 1;
	// ~Variables

public:
	// ~Properties
	// Whether players should be put into the default team by default
	UPROPERTY(EditAnywhere, Category = Teams)
	uint8 bMovePlayersToDefaultTeam : 1;
	// Whether players are allowed to join the zombie team
	UPROPERTY(EditAnywhere, Category = Teams)
	uint8 bCanJoinZombieTeam : 1;
	// Whether players are allowed to join the dummy team
	UPROPERTY(EditAnywhere, Category = Teams)
	uint8 bCanJoinDummyTeam : 1;
	// Team for players who aren't in a team
	UPROPERTY(EditAnywhere, Category = Teams)
	TSubclassOf<UBrickTeam> DefaultTeam;
	// The team zombies are assigned to
	UPROPERTY(EditAnywhere, Category = Teams)
	TSubclassOf<UBrickTeam> ZombieTeam;
	// The team dummies are assigned to
	UPROPERTY(EditAnywhere, Category = Teams)
	TSubclassOf<UBrickTeam> DummyTeam;
	// The teams supported by this mode
	UPROPERTY(EditAnywhere, Category = Teams)
	TSet<TSubclassOf<UBrickTeam>> Teams;

	// The vehicle class to spawn
	UPROPERTY(EditAnywhere, Category = GameMode)
	TSubclassOf<ABrickVehicle> VehicleClass;
	// How long characters of disconnected players should remain in the level
	UPROPERTY(EditAnywhere, Category = GameMode)
	float InactiveCharacterLifeSpan;
	// Time a player remains in playing state after death, 0 means indefinitely
	UPROPERTY(EditAnywhere, Category = GameMode, meta = (ClampMin = 0.f))
	float MinUnpossessDelay;
	// Whether players should be spawned initially and automatically respawned after dying
	UPROPERTY(EditAnywhere, Category = GameMode)
	uint8 bAutoRestartPlayers : 1;
	// If true all players will be invincible by default
	UPROPERTY(EditAnywhere, Category = GameMode)
	uint8 bSpawnCharactersInvincible : 1;
	// Whether players' money should be cleared upon death
	UPROPERTY(EditAnywhere, Category = GameMode)
	uint8 bResetMoneyOnDeath : 1;
	// Whether character movement should be disabled during WaitingToStart state
	UPROPERTY(EditAnywhere, Category = GameMode)
	uint8 bDisableMovementDuringWaitingToStart : 1;
	// The maximum percentage difference for players allowed per team
	UPROPERTY(EditAnywhere, Category = GameMode, meta = (ClampMin = 0.f, ClampMax = 1.f))
	float AutoBalanceTeamsThreshold;
	// Maximum distance at which friendly and enemy players are considered when selecting a spawn point
	UPROPERTY(EditAnywhere, Category = GameMode)
	float MaxSpawnPlayerRelevanceDistance;
	// Max distance local chat message can be received from
	UPROPERTY(EditAnywhere, Category = GameMode)
	float MaxLocalChatDistance;

	// Score at which a team wins
	UPROPERTY(EditAnywhere, Category = TeamScore)
	int32 MaxTeamScore;
	// Score added for a team per kill
	UPROPERTY(EditAnywhere, Category = TeamScore)
	int32 KillTeamScore;

	// Percentage of money given to a player for any amount of score earned
	UPROPERTY(EditAnywhere, Category = Score)
	float AddScoreMoneyRatio;
	// Score given for a kill
	UPROPERTY(EditAnywhere, Category = Score)
	float KillScore;
	// Score given for killing a zombie
	UPROPERTY(EditAnywhere, Category = Score)
	float ZombieKillScore;
	// Score given for a captured point
	UPROPERTY(EditAnywhere, Category = Score)
	float PointCapturedScore;
	// Score given for neutralizing a point
	UPROPERTY(EditAnywhere, Category = Score)
	float PointNeutralizedScore;
	// Score given to all players when a round was won
	UPROPERTY(EditAnywhere, Category = Score)
	float RoundWinScore;
	// Score given to all players when a round before an intermission was won
	UPROPERTY(EditAnywhere, Category = Score)
	float IntermissionRoundWinScore;
	// Whether match score widgets like the timer, team scores etc should be displayed
	UPROPERTY(EditAnywhere, Category = Score)
	bool bShowMatchScore;

	UPROPERTY(EditAnywhere, Category = MatchTimers)
	int32 WarmupTime;
	UPROPERTY(EditAnywhere, Category = MatchTimers)
	int32 RoundStartTime;
	UPROPERTY(EditAnywhere, Category = MatchTimers)
	int32 RoundEndTime;
	UPROPERTY(EditAnywhere, Category = MatchTimers)
	int32 IntermissionTime;
	UPROPERTY(EditAnywhere, Category = MatchTimers)
	int32 MatchEndTime;

	// Whether the warmup stage should be skipped for this mode
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bSkipWarmup : 1;
	// The maximum number of rounds to play
	UPROPERTY(EditAnywhere, Category = Rounds)
	int32 MaxRoundsPerMatch;
	// If > 0 will determine after how many rounds an intermission is started
	UPROPERTY(EditAnywhere, Category = Rounds)
	int32 NumRoundsToIntermission;
	// Whether the round should be ended when one team has died
	UPROPERTY(EditAnywhere, Category = Rounds)
	uint32 bEndRoundWhenTeamEliminated : 1;
	// Whether the match should be ended when one team has died
	UPROPERTY(EditAnywhere, Category = Rounds)
	uint32 bEndMatchWhenTeamEliminated : 1;
	// Whether players are allowed to restart while the round is still in progress
	UPROPERTY(EditAnywhere, Category = Rounds)
	uint32 bAllowRestartDuringRound : 1;
	// Whether players may restart while in the playing state
	UPROPERTY(EditAnywhere, Category = Rounds)
	uint32 bAllowRestartWhilePlaying : 1;
	// Whether the levels and players should be reset for every round
	UPROPERTY(EditAnywhere, Category = Rounds)
	uint32 bResetLevelAndPlayersEveryRound : 1;
	// Whether the loadout should be restored upon respawning/teleporting
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bRespawnLoadoutOnRestart : 1;

	// Whether the player is allowed to interact even while unconscious
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowInteractionWhileUnconscious : 1;
	// Whether players are allowed to respawn while alive
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowTeleporting : 1;
	// Whether players are allowed to spawn on/teleport to garages
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSpawnOnGarages : 1;
	// Whether players are allowed to spawn on neutral vehicles
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSpawnOnNeutralVehicles : 1;
	// Whether players are allowed to spawn on hostile vehicles
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSpawnOnHostileVehicles : 1;
	// Whether dead players can also spawn vehicles
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSpawnVehicleWhileUnconscious : 1;
	// Whether players are allowed to use slow motion
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSlowMotion : 1;
	// Whether players are allowed to enter the vehicle editor
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowVehicleEditor : 1;
	// Whether admins are allowed to remove any vehicle on the map
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowAdminsToScrapAllVehicles : 1;
	// Whether the player is allowed to scrap vehicles from the map
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowRemoteScrapVehicles : 1;
	// Whether players are allowed to manually explode their vehicles
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowExplodeVehicle : 1;
	// Whether players are allowed to enter vehicles with hostiles in them
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowEnterHostileVehicle : 1;
	// Whether players have access to infinite money
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowInfiniteMoney : 1;
	// Whether players can damage team mates
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowFriendlyFire : 1;
	// Whether players may change teams while alive
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowChangeTeamWhileAlive : 1;
	// Whether players are allowed to use the projectile camera
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowProjectileCamera : 1;
	// Whether players are allowed to take control of abandoned characters
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowPossessAbandonedCharacters : 1;
	// Whether dummies can be spawned and removed
	UPROPERTY(EditAnywhere, Category = Sandbox)
	uint32 bAllowSpawnDummies : 1;
	// ~Properties

	// ~Constructor
	ABrickGameMode();

	// ~Super Interface
	virtual void PreInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void GenericPlayerInitialization(AController* C) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	virtual void AddInactivePlayer(APlayerState* PlayerState, APlayerController* PC) override;
	virtual bool FindInactivePlayer(APlayerController* PC) override;
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual void InitializeHUDForPlayer_Implementation(APlayerController* NewPlayer) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	virtual void RestartPlayer(AController* NewPlayer) override;
	virtual void RestartPlayerAtPlayerStart(AController* NewPlayer, AActor* StartSpot) override;
	virtual void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;
	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;
	virtual APlayerController* SpawnPlayerControllerCommon(ENetRole InRemoteRole, const FVector& SpawnLocation, const FRotator& SpawnRotation, TSubclassOf<APlayerController> InPlayerControllerClass) override;
	virtual void PostSeamlessTravel() override;
	virtual bool AllowCheats(APlayerController* P) override;
	virtual void SetMatchState(FName NewState) override;
	virtual void OnMatchStateSet() override;
	virtual bool IsMatchInProgress() const override;
	virtual void HandleMatchHasStarted() override;
	virtual void HandleMatchHasEnded() override;
	virtual bool ReadyToStartMatch_Implementation() override;

private:
	virtual bool ReadyToEndMatch_Implementation() override final;
	virtual void EndMatch() override final;

public:
	virtual void RestartGame() override;
	// ~Super Interface

	// ~Testing
#if !UE_BUILD_SHIPPING
	void OnNumFakePlayersCVarChanged(IConsoleVariable* InCVar);
#endif
	// ~Testing

	// ~Game Mode
	// Get the brick game mode instance
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Game Mode"))
	static ABrickGameMode* Get(const UObject* WorldContextObject);
	// Get the game mode class CDO from the game state
	UFUNCTION(BlueprintPure, meta = (WorldContext = WorldContextObject))
	static const ABrickGameMode* GetDefaultGameMode(const UObject* WorldContextObject);
	// Called from the game state when all game mode specific sub levels have been loaded
	void OnAllGameModeSubLevelsLoaded();
	// Called when the host is about to leave the match
	void OnHostLeavingMatch();

	// End match version that supports a winner
	UFUNCTION(BlueprintCallable)
	void EndMatchWithWinner(const FMatchWinner& InWinner);

private:
	// Timer callback
	void OnUnPossessPlayersTimer();
	// Timer callback for actually restarting the game
	void OnRestartGameTimer();
	// ~Game Mode

public:
	// ~Match
	// Can be used by admins or for debugging
	UFUNCTION(BlueprintCallable, Exec)
	void ForceEndMatch();
	// Sets the winner of the match/round
	void SetMatchWinner(const FMatchWinner& InWinner);
	// Handlers for custom match states
	virtual void HandleWaitingForPlayers();
	virtual void HandleWarmup();
	virtual void HandleRoundWaitingToStart();
	virtual void HandleRoundHasStarted();
	virtual void HandleRoundHasEnded();
	virtual void HandleIntermission();
	// Called from the exit match timer on the game state
	void ExitMatch();
	// Called from players when they changed match settings locally
	void PlayerSetNextMatchSettings(ABrickPlayerController* PC, const FMatchSettings& NewSettings);
	// Applies next match settings to the current match
	void PlayerApplyNextMatchSettings(ABrickPlayerController* PC, bool bFadeIn);
	// ~Match

	// ~Rounds
	// Returns the round counter from the game state
	int32 GetCurrentRound() const;
	// Whether this is the last round before an intermission
	bool IsIntermissionRound() const
	{
		return NumRoundsToIntermission > 0 && GetCurrentRound() % NumRoundsToIntermission == 0;
	}

	// Whether the first round can be started yet
	UFUNCTION(BlueprintNativeEvent)
	bool IsReadyToStartWarmup() const;
	// Return true if the round should end and output the winner team
	UFUNCTION(BlueprintNativeEvent)
	bool IsReadyToEndRound(FMatchWinner& OutWinner, bool& bOutEndMatch) const;
	// Ends the round with the given winner, optionally ending the match as well
	UFUNCTION(BlueprintCallable)
	void EndRound(const FMatchWinner& InWinner, bool bEndMatch = false);

	// These functions are all used to initiate match state transitions
	void StartWarmup();
	void StartFirstRound();
	void StartRound();
	void ExitRound();
	void EndIntermission();
	// ~Rounds

	// ~Restarting
	// Tries to spawn all players
	UFUNCTION(BlueprintCallable)
	void RestartAllPlayers(bool bIncludeAlive = true);
	// Lets a player respawn at a specific spawn point or location
	EPlayerSpawnResult RestartPlayerAt(ABrickPlayerController* PC, FPlayerSpawnRequest SpawnRequest);
	// Custom version that allows resetting the existing character
	void RestartPlayerAtTransform(AController* NewPlayer, const FTransform& SpawnTransform, bool bMoveCharacter, bool bResetCharacter);
	// Used to manage the state of a player after dying
	virtual void HandleRestartingPlayer(ABrickPlayerController* PC);
	// Selects a random valid player start from the list
	ABrickPlayerStart* ChoosePlayerStart(const FGenericTeamId& PawnTeamId, UClass* PawnClass, const TArray<ABrickPlayerStart*>& PlayerStarts);
	// Version used for player spawns
	ABrickPlayerStart* ChoosePlayerStart(ABrickPlayerController* PC, const TArray<ABrickPlayerStart*>& PlayerStarts);
	// Called when a player has left the playing state
	void OnPlayerEndPlayingState(ABrickPlayerController* PC);
	// Get the number of players currently alive and playing
	UFUNCTION(BlueprintPure)
	int32 GetNumPlayersAlive() const;
	// Called when a vehicle was spawned
	void OnPlayerSpawnedVehicle(ABrickPlayerController* PC, ABrickVehicle* Vehicle) const;
	// ~Restarting

	// ~Admins
	// Return whether the given player is allowed to be an admin
	bool CanPlayerBeAdmin(ABrickPlayerController* PC) const;
	// Allows players to login as an admin
	void PlayerRequestAdminAccess(ABrickPlayerController* PC, const FString& PW);
	// ~Admins

	// ~Pawns
	// Returns the default character pawn CDO
	static const ABaseCharacter* GetDefaultCharacter();
	// Allow the game mode to modify the damage dealt to a pawn
	UFUNCTION(BlueprintNativeEvent)
	float CalcDamage(float DamageAmount, ABaseCharacter* Character, AController* EventInstigator, AActor* DamageCauser) const;
	// Called when any character has died
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterDied(ABaseCharacter* Character, AController* EventInstigator, AActor* DamageCauser);
	// Called when any character was revived
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterRevived(ABaseCharacter* Character, AController* EventInstigator);
	// Called when a character was spawned
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterBeginPlay(ABaseCharacter* Character);
	// Called when a character was destroyed
	UFUNCTION(BlueprintNativeEvent)
	void OnCharacterEndPlay(ABaseCharacter* Character);

protected:
	// Called from characters when they were just spawned or revived
	virtual void OnCharacterBeginPlayOrRevived(ABaseCharacter* Character);
	// Called when a character has either died or was destroyed
	virtual void OnCharacterEndPlayOrDied(ABaseCharacter* Character);
	// Called when a player has died
	virtual void OnPlayerDied(ABrickPlayerController* PC);
	// Called when a player has killed a hostile
	virtual void OnPlayerKilledEnemy(ABrickPlayerController* PC, ABaseCharacter* Enemy);
	// ~Pawns

public:
	// ~Teams
	// Lets a player try to join a team
	void PlayerJoinTeam(ABrickPlayerController* PC, const FGenericTeamId& TeamId);

protected:
	// Moves a player to another team, takes care of selecting a new leader etc. if required
	UFUNCTION(BlueprintCallable)
	void MovePlayerToTeam(ABrickPlayerController* PC, const FGenericTeamId& TeamId);
	// Moves the player to the team where balance would be the most fair, optionally avoid a specific team to rotate teams
	void MovePlayerToTeamBalanced(ABrickPlayerController* PC, const FGenericTeamId& AvoidTeamId = FGenericTeamId::NoTeam);

private:
	// Called when a player has died or is about to leave a team
	void OnTeamMemberLeftOrDied(ABrickPlayerController* PC, bool bUpdateLeaderState);
	// Called when a player has just joined a team, spawned or was revived
	void OnTeamMemberJoinedOrSpawned(ABrickPlayerController* PC);
	// ~Teams

public:
	// ~Score
	// Add point/rounds/tickets etc. to a specific team
	UFUNCTION(BlueprintCallable)
	void AddTeamScore(const FGenericTeamId& TeamId, int32 Score);
	// Add score to a specific player
	UFUNCTION(BlueprintCallable)
	void AddPlayerScore(ABrickPlayerController* PC, float Score, bool bAdd = true);
	// Give score to all active players
	UFUNCTION(BlueprintCallable)
	void AddScoreToAllPlayers(float Score);
	// Give score to all players of a specific team
	UFUNCTION(BlueprintCallable)
	void AddScoreToAllPlayersOfTeam(const FGenericTeamId& TeamId, float Score);
	// Gives or removes money from the player
	UFUNCTION(BlueprintCallable)
	void AddPlayerMoney(ABrickPlayerController* PC, float Amount, bool bAdd = true);
	// ~Score

	// ~Capture Points
	UFUNCTION(BlueprintNativeEvent)
	void OnCapturePointCaptured(ACapturePoint* CapturePoint, const FGenericTeamId& Team, const TSet<ABaseCharacter*>& Characters);
	UFUNCTION(BlueprintNativeEvent)
	void OnCapturePointNeutralized(ACapturePoint* CapturePoint, const FGenericTeamId& Team, const TSet<ABaseCharacter*>& Characters);
	// ~Capture Points

	// ~Vehicles
	// Try to scrap a vehicle and give the player money for it
	void ScrapVehicle(ABrickPlayerController* PC, ABrickVehicle* Vehicle);
	// Remove all (empty) vehicles from the level
	void ScrapAllVehicles(ABrickPlayerController* PC, bool bIncludeUsed);
	// ~Vehicles
};
