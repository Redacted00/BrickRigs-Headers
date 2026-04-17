// Copyright Fluppisoft, 2015

#pragma once

#include "Bricks/ActuatorBrick.h"
#include "Bricks/WinchBrick.h"
#include "World/Elevator.h"
#include "BrickSpectatorPawn.h"
#include "Input/BaseInputComponent.h"
#include "PlayerSpawnRequest.h"
#include "BrickChatMessage.h"
#include "GenericTeamAgentInterface.h"
#include "ClientDamageInfo.h"
#include "ReplicatedHitInfo.h"
#include "Game/BrickGameState.h"
#include "Vehicle/BrickVehicle.h"
#include "BasePlayerController.h"
#include "BrickPlayerController.generated.h"

// Actors
class ABaseCharacter;
class AConquestFlag;
class ADragStrip;
class ABrickGameMode;
class ABrickEditor;
class AGravelSilo;
class BrickVehicleDownloadReplicator;
// Components
class UInventoryComponent;
class UAudioComponent;
class UEditorInputComponent;
// Bricks
class UCouplingBrick;
class UDetonatorBrick;
class USwitchBrick;
// Misc
class IBrickPawnInterface;

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickPlayerController : public ABasePlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateChanged, const FName&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCharacterChanged, ABaseCharacter*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnVehicleChanged, ABrickVehicle*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpectatorPawnChanged, ASpectatorPawn*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAccessedInventoryChanged, UInventoryComponent*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamagedCharacter, const FClientDamageInfo&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnReceivedChatMessages, const TArray<FBrickChatMessage>&);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFreeCamModeChanged, EFreeCamMode, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsInFreeCamChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewTargetChanged, UObject*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBrickEditorChanged, ABrickEditor*);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnVehicleDownloadProgressChanged, ABrickVehicleDownloadReplicator*, const TOptional<float>&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnProjectileCameraEnabledChanged, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnRestartFailed, EPlayerSpawnResult, const FVehicleSpawnProperties&);

private:
	// ~Variables
	// Used to delay the unpossession of the pawn after death
	FTimerHandle TimerHandle_UnpossessDeadPawn;
	// NOTE: We need these delegate handles in order to be able to unbind the correct ones
	FDelegateHandle DelegateHandle_OnPlayerCharacterVehicleChanged;
	FDelegateHandle DelegateHandle_OnViewedCharacterVehicleChanged;
	FDelegateHandle DelegateHandle_OnPlayerCharacterStateOfHealthChanged;
	FDelegateHandle DelegateHandle_OnViewedCharacterStateOfHealthChanged;
	FDelegateHandle DelegateHandle_OnViewedCharacterReceivedDamage;
	FPopupHandle PopupHandle_UnsavedChanges;
	FPopupHandle PopupHandle_Inventory;
	FPopupHandle PopupHandle_Map;
	FPopupHandle PopupHandle_Scoreboard;
	FPopupHandle PopupHandle_UGCBrowser;

	// Set to true during OnPossess
	uint32 bIsPossessingPawn : 1;
	// Indicates that the player is being kicked during the short period before the controller is destroyed
	uint32 bHasBeenKicked : 1;
	// Whether the unconscious sound mix is currently added
	uint32 bIsDeathSoundMixAdded : 1;
	// Whether the projectile camera mode is enabled
	uint32 bIsProjectileCameraEnabled : 1;
	// The dynamic sound mixed used to simulate the atmosphere density, has to be stored in order not be garbage collected
	UPROPERTY(Transient)
	USoundMix* AtmosphereSoundMix;
	// The last valid team id assigned
	FGenericTeamId PreviousTeamId;
	// The winch the player is currently attaching
	TBrickEditorObjectPtr<UWinchBrick> AttachingWinchBrick;
	// The actuator being interacted with that needs to be sent to the server
	TWeakObjectPtr<UActuatorBrick> ActuatorToReplicate;
	// Last time the actuator states have been sent to the server
	float LastActuatorSyncTime;
	// Stores times when vehicles have been unpossessed
	TMap<FWeakObjectPtr, float> VehicleUnpossessTimes;

	// The character that belongs to this controller
	UPROPERTY(ReplicatedUsing = OnRep_PlayerCharacter, Transient)
	ABaseCharacter* PlayerCharacter;
	UFUNCTION()
	void OnRep_PlayerCharacter(ABaseCharacter* OldCharacter);
	// The character we are currently spectating
	UPROPERTY(ReplicatedUsing = OnRep_SpectatedCharacter, Transient)
	ABaseCharacter* SpectatedCharacter;
	UFUNCTION()
	void OnRep_SpectatedCharacter();
	// The current view target character
	UPROPERTY(Transient)
	ABaseCharacter* ViewedCharacter;
	// The vehicle the player is in
	UPROPERTY(Transient)
	ABrickVehicle* PlayerVehicle;
	// The vehicle that is currently being viewed
	UPROPERTY(Transient)
	ABrickVehicle* ViewedVehicle;
	// The object that is being used as the current view target
	TWeakObjectPtr<UObject> ViewTargetSubobject;

	// The other inventory component the player is currently looking at in the inventory
	UPROPERTY(Transient, ReplicatedUsing = OnRep_AccessedInventory)
	UInventoryComponent* AccessedInventory;
	UFUNCTION()
	void OnRep_AccessedInventory(UInventoryComponent* OldInventory);
	// Mirrored from the accessed inventory to replicate to the client
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedInventory)
	FInventory ReplicatedInventory;
	UFUNCTION()
	void OnRep_ReplicatedInventory();

	// Number of failed attempts to login as an admin
	uint8 NumFailedAdminLogins;
	// A list of all cached messages
	TArray<FBrickChatMessage> CachedChatMessages;

	// The editor the player is currently in
	UPROPERTY(Transient)
	ABrickEditor* BrickEditor;
	// The actor the editor was entered through, for example a garage
	UPROPERTY(Transient)
	AActor* EditorEntryPoint;
	// Saved delegate to invoke when the unsaved changes popup has been closed
	FOnPopupClosed OnUnsavedChangesPopupClosedDelegate;

	// Server time when the player has been frozen
	UPROPERTY(Transient, Replicated)
	float FreezeTime;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UAudioComponent* HurtAudioComponent;
	// ~Components

public:
	// ~Delegates
	FOnStateChanged OnStateChangedDelegate;
	FOnCharacterChanged OnPlayerCharacterChangedDelegate;
	FOnCharacterChanged OnViewedCharacterChangedDelegate;
	FOnCharacterChanged OnSpectatedCharacterChangedDelegate;
	FOnVehicleChanged OnPlayerVehicleChangedDelegate;
	FOnVehicleChanged OnViewedVehicleChangedDelegate;
	FOnSpectatorPawnChanged OnSpectatorPawnChangedDelegate;
	FOnAccessedInventoryChanged OnAccessedInventoryChangedDelegate;
	FOnDamagedCharacter OnDamagedCharacterDelegate;
	FOnReceivedChatMessages OnReceivedChatMessagesDelegate;
	FOnIsInFreeCamChanged OnIsInFreeCamChangedDelegate;
	FOnFreeCamModeChanged OnFreeCamModeChangedDelegate;
	FOnBrickEditorChanged OnBrickEditorChangedDelegate;
	FOnViewTargetChanged OnViewTargetSubobjectChangedDelegate;
	FOnVehicleDownloadProgressChanged OnVehicleDownloadProgressChangedDelegate;
	FOnProjectileCameraEnabledChanged OnProjectileCameraEnabledChangedDelegate;
	FOnRestartFailed OnRestartFailedDelegate;
	// ~Delegates

	// Constructor
	ABrickPlayerController();

	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void SetupInputComponent() override;
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void PostNetReceive() override;
	virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
	virtual void Tick(float DeltaTime) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetControlRotation(const FRotator& NewRotation) override;
	virtual void BuildInputStack(TArray<UInputComponent*>& InputStack) override;
	virtual void UpdateHiddenActors(const FVector& ViewLocation) override;
	virtual void UpdateHiddenComponents(const FVector& ViewLocation, TSet<FPrimitiveComponentId>& OutHiddenComponents) override;
	virtual void ReceivedGameModeClass(TSubclassOf<AGameModeBase> GameModeClass) override;

	virtual void InitPlayerState() override;

	virtual void ClientReset_Implementation() override;
	virtual void Reset() override;
	virtual bool CanRestartPlayer() override;
	virtual void FailedToSpawnPawn() override;
	virtual void SetIgnoreMoveInput(bool bNewMoveInput) override;

	virtual bool IsFrozen() override;
	virtual void UnFreeze() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void PawnLeavingGame() override;
	virtual void PawnPendingDestroy(APawn* InPawn) override;
	virtual void ChangeState(FName NewState) override;
	virtual void BeginPlayingState() override;
	virtual void EndPlayingState() override;
	virtual void BeginSpectatingState() override;
	virtual void EndSpectatingState() override;
	virtual void BeginInactiveState() override;
	virtual void EndInactiveState() override;
	virtual void SetSpectatorPawn(ASpectatorPawn* NewSpectatorPawn) override;
	virtual void DestroySpectatorPawn() override;
	virtual void SetViewTarget(AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;
	virtual void SetSpawnLocation(const FVector& NewLocation) override;
	virtual AActor* GetViewTarget() const override;
	virtual void GetPlayerViewPoint(FVector& out_Location, FRotator& out_Rotation) const override;
	virtual void GetAudioListenerPosition(FVector& OutLocation, FVector& OutFrontDir, FVector& OutRightDir) const override;
	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;
	virtual void ClientWasKicked_Implementation(const FText& KickReason) override;

	virtual AActor* GetDesiredViewTarget_Implementation(FViewTargetTransitionParams& OutTransitionParams) const override;
	virtual APawn* GetViewTargetPawn() const override;
	virtual void OnLocalPlayerReady() override;
	virtual void OnLocalPlayerUnready() override;
	virtual bool ShouldScaleRotationInputToFOV() const override;
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	virtual void OnTappedMenu() override;
	virtual void OnHeldMenu() override;
	virtual void OnMenuOpenChanged(bool bNewOpen) override;
	virtual bool CanViewAndEditKicks() const override;
	// ~Super Interface

private:
	// ~Player
	// Callback for the view target
	void OnViewTargetEndPlay(AActor* Actor, EEndPlayReason::Type EndPlayReason);
	//  Called when the player is actively playing again
	void OnBeginPlayOrRevived();
	// Called when the player stopped playing or was revived
	void OnEndPlayOrRevived();
	// Called when the player is no longer alive or playing
	void OnBeginInactiveOrDied();
	// Called when the player is no conscious or playing
	void OnBeginInactiveOrPassedOut();
	// Called from a timer to unposses the pawn after dying
	void UnpossessDeadPawn();
	// Updates the alive flag on the player state
	void UpdateIsAlive(bool bNewAlive);

public:
	// Takes care of adding or removing the death sound mix
	void UpdateDeathSoundMix();
	// Sweeps against world geometry to restart a pawn or place a marker
	bool SweepPawnOrMarker(FHitResult& OutHit, const FVector& SweepStart, const FVector& SweepEnd, const FQuat& Rotation, const FCollisionShape& CollisionShape, bool bSpawningNewPawn = false) const;
	// ~Player

	// ~Admin
	// Whether the player could ever log in as an admin on the server, server and clients
	UFUNCTION(BlueprintPure)
	bool CanEverBeAdmin() const;
	// Whether the player should automatically be registered as an admin (for example the host), server only
	UFUNCTION(BlueprintPure)
	bool IsAdminByDefault() const;
	// Whether this player has admin rights, server and owning client
	bool IsAdmin() const;
	// Sets whether this player is an admin, server only
	void SetIsAdmin(bool bNewAdmin);
	// Players have to send a request to the server with the correct password in order to receive admin access
	UFUNCTION(BlueprintCallable)
	void RequestAdminAccess(const FString& PW);
	// Increment the failed login attempts and return the new value
	int32 AddFailedLoginAttempt();
	// Notifies the client about a failed login attempt
	UFUNCTION(Client, Reliable)
	void ClientSentWrongAdminPassword(uint8 AttemptsRemaining);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestAdminAccess(const FString& PW);

public:
	// Called when the player changed the match settings
	void OnPlayerChangedMatchSettings(const FMatchSettings& NewSettings);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnPlayerChangedMatchSettings(const FMatchSettings& NewSettings, bool bApply, bool bFade);

public:
	// Applies the next match settings
	void ApplyMatchSettings(bool bFade);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplyMatchSettings(bool bFade);

public:
	// Run the server command to restart the level
	UFUNCTION(BlueprintCallable, Exec)
	void RestartMatch();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRestartMatch();

public:
	// Resets all actors in the level
	UFUNCTION(BlueprintCallable, Exec)
	void ResetLevel();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerResetLevel();
	// ~Admin

public:
	// ~Team
	// Get the last valid team id assigned
	const FGenericTeamId& GetPreviousTeamId() const
	{
		return PreviousTeamId;
	}

	// Marks this player as a team leader, server only
	void SetIsTeamLeader(bool bNewLeader);
	// Whether this player is currently a team leader
	bool IsTeamLeader() const;
	// Whether this player is allowed to join the given team
	UFUNCTION(BlueprintPure)
	bool CanJoinTeam(const FGenericTeamId& InTeamId) const;
	// Try to join a different team
	UFUNCTION(BlueprintCallable)
	void JoinTeam(const FGenericTeamId& InTeamId);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerJoinTeam(const FGenericTeamId& InTeamId);
	// ~Team

public:
	// ~Chat
	// Get all cached chat messages
	const auto& GetChatMessages() const
	{
		return CachedChatMessages;
	}

	// Used to send chat messages from server to client
	UFUNCTION(Client, Reliable)
	void ClientReceiveChatMessage(const FBrickChatMessage& ChatMessage);
	// Version that takes multiple messages
	UFUNCTION(Client, Reliable)
	void ClientReceiveChatMessages(const TArray<FBrickChatMessage>& ChatMessages);

	// Sends a chat message to other clients via the server
	UFUNCTION(BlueprintCallable)
	void SendChatMessage(EChatContext Context, const FText& Message);
	// Console friendly chat function
	UFUNCTION(BlueprintCallable, Exec)
	void Say(const FText& Message);
	// Console friendly admin chat function
	UFUNCTION(BlueprintCallable, Exec)
	void AdminSay(const FText& Message);

private:
	// Determine if the player is allowed to send in the given channel
	bool CanPlayerSendMessage(EChatContext Context) const;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendChatMessage(EChatContext Context, const FText& Message);
	// ~Chat

public:
	// ~Hit Detection
	// Called from the character class when it has received damage
	void OnDamagedCharacter(ABaseCharacter* OtherCharacter, const FClientDamageInfo& DamageInfo);
	// Informs the client about damage they have dealt
	UFUNCTION(Client, Reliable)
	void ClientDamagedCharacter(const FClientDamageInfo& DamageInfo);
	// ~Hit Detection

	// ~Player State
	// Returns the player name
	FString GetPlayerName() const;
	// Returns the player name display text
	FText GetPlayerNameText() const;
	// Add a death to the stats
	void AddDeath();
	// Add a kill to the stats
	void AddKill();
	// Returns the amount of money the player has, -1 means infinite
	float GetMoney() const;
	// Whether the player has access to infinite money
	bool HasInfiniteMoney() const;
	// Returns the number of items the player could afford
	int32 CanPurchaseItems(float PricePerItem, int32 Amount) const;
	// Sets the total money balance
	void SetMoney(float NewAmount);
	// ~Player State

	// ~Game Mode
	// Callback for the game state
	void OnMatchStateChanged(const FName& NewState);
	// Helper function to determine if the match is over yet
	bool HasMatchEnded() const;

	// Whether the player is exempt from match restrictions
	bool IsExemptFromMatchRestrictions() const;
	// Whether the player is allowed to enter seatless vehicles
	bool CanEnterSeatlessVehicle() const;
	// ~Game Mode

	// ~UI
	// Show the game HUD
	void ShowPlayingHUD();
	// Show the death screen
	void ShowDeathHUD();
	// Continue to the spawn screen
	void ShowSpawnHUD();
	// Show the spectator screen
	void ShowSpectatorHUD();
	// Shows the brick editor HUD
	void ShowEditorHUD();
	// Show the end screen
	void ShowMatchEndHUD();
	// Sets the HUD according to the current state
	void UpdateHUD();
	// Changes the current HUD widget
	UFUNCTION(BlueprintCallable)
	void ShowHUDWidget(const TSoftClassPtr<UUserWidget>& HUDWidgetClass);
	// Opens or closes the in game menu
	void ToggleMenuOpen();
	UFUNCTION(BlueprintCallable)
	void OpenChat();
	bool CanOpenMap() const;
	UFUNCTION(BlueprintCallable)
	void OpenMap(bool bToggleOpen = false);
	UFUNCTION(BlueprintCallable)
	void CloseMap();
	bool CanOpenScoreboard() const;
	UFUNCTION(BlueprintCallable)
	void OpenScoreboard(bool bToggleOpen = false);
	UFUNCTION(BlueprintCallable)
	void CloseScoreboard();
	// Whether the vehicle browser may be opened
	UFUNCTION(BlueprintPure)
	bool CanOpenVehicleBrowser(AActor* EntryPoint) const;
	// Opens the vehicle spawn menu
	UFUNCTION(BlueprintCallable)
	void OpenVehicleBrowser(AActor* EntryPoint);
	// Closes the vehicle spawn menu
	UFUNCTION(BlueprintCallable)
	void CloseVehicleBrowser();
	// ~UI

	// ~Pawn
	// Set the associated character, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPlayerCharacter(ABaseCharacter* InCharacter);

private:
	// Called when the character was changed on the server or by replication
	void OnPlayerCharacterChanged(ABaseCharacter* OldCharacter);

public:
	// Get the character owned by this player
	UFUNCTION(BlueprintPure)
	ABaseCharacter* GetPlayerCharacter() const
	{
		return PlayerCharacter;
	}

	// Templated version
	template <class T>
	T* GetPlayerCharacter() const
	{
		return Cast<T>(PlayerCharacter);
	}

	// Get the character currently being controlled or spectated
	UFUNCTION(BlueprintPure)
	ABaseCharacter* GetViewedCharacter() const
	{
		return ViewedCharacter;
	}

	// Get the vehicled the viewed character is in
	UFUNCTION(BlueprintPure)
	ABrickVehicle* GetViewedVehicle() const
	{
		return ViewedVehicle;
	}

private:
	// Updates the viewed character if needed
	void UpdateViewedCharacter();
	// Called whenever the currently viewed character has changed
	void OnViewedCharacterChanged();
	// Updates the cached view target vehicle
	void UpdateViewedVehicle();
	// Called when the viewed vehicle has changed
	void OnViewedVehicleChanged();
	// Updates the associated vehicle
	void SetPlayerVehicle(ABrickVehicle* NewVehicle);

public:
	// Return the vehicle the character is in
	UFUNCTION(BlueprintPure)
	ABrickVehicle* GetPlayerVehicle() const
	{
		return PlayerVehicle;
	}

	// Whether the player is currently in the vehicle
	bool IsInVehicle() const
	{
		return PlayerVehicle != nullptr;
	}

	// Returns either the character or vehicle
	APawn* GetBrickPawn() const;
	// Get the pawn casted to the interface
	IBrickPawnInterface* GetBrickPawnInterface() const;
	// Get the pawn that should be restarted with the player
	APawn* GetPawnToRestart() const;
	// Return the time the player will remain viewing the character while dead
	float GetPawnUnpossessDelay() const;
	// Whether the player has a character and is alive
	bool IsCharacterAlive() const;
	// Whether the player has a conscious character
	bool IsCharacterConscious() const;
	// Kills the own character
	UFUNCTION(BlueprintCallable, Exec)
	void KillCharacter();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKillCharacter();
	// Callbacks for the player character
	void OnPlayerCharacterStateOfHealthChanged(ECharacterStateOfHealth NewState);
	// Callbacks for the viewed character
	void OnViewedCharacterReceivedDamage(const FCharacterDamageInfo& DamageInfo);
	void OnViewedCharacterStateOfHealthChanged(ECharacterStateOfHealth NewState);
	void OnViewedCharacterVehicleChanged(ABrickVehicle* NewVehicle);
	// ~Pawn

public:
	// ~View
	// Can be used to copy the view rotation from the current view target to the control rotation
	void ApplyCurrentViewRotation();
	// Get the object or subobject being used as the view target
	UObject* GetViewTargetSubobject() const
	{
		return ViewTargetSubobject.Get();
	}

	// Get the active view target interface, from the view target actor or subobject
	IViewTargetInterface* GetActiveViewTargetInterface() const
	{
		return Cast<IViewTargetInterface>(ViewTargetSubobject);
	}

	// ~View

	// ~Sandbox
	// Whether this player is allowed to use free cam
	UFUNCTION(BlueprintPure)
	bool CanUseFreeCam() const;
	// Whether the player is currently in free cam
	UFUNCTION(BlueprintPure)
	bool IsInFreeCam() const;
	// Get the current free cam mode
	UFUNCTION(BlueprintPure)
	EFreeCamMode GetFreeCamMode() const;
	// Changes the current free cam mode, enters it if needed
	UFUNCTION(BlueprintCallable)
	void SetFreeCamMode(EFreeCamMode NewMode);
	// Cycles between the different free cam modes
	void CycleFreeCamMode();
	// Whether the free cam is currently fixed
	UFUNCTION(BlueprintPure)
	bool IsFreeCamFixed() const;
	// Enters free or fixed cam mode, returns true if successful (or already in free cam mode)
	UFUNCTION(BlueprintCallable)
	bool EnterFreeCam();
	// Enters the spawn vehicle free cam mode
	bool EnterSpawnCam(const FPlayerSpawnRequest& SpawnRequest);

private:
	// Internal version used to enter free or spawn cam
	bool EnterFreeCamInternal(EFreeCamMode InFreeCamMode, bool bInIsFixed, const TOptional<FPlayerSpawnRequest>& SpawnRequest = TOptional<FPlayerSpawnRequest>());

public:
	// Exits the free cam mode
	UFUNCTION(BlueprintCallable)
	void ExitFreeCam();
	// Toggle between free cam and the normal camera
	void ToggleFreeCam();
	// Can be used to enter or exit fixed camera mode
	UFUNCTION(BlueprintCallable)
	void SetFixedCam(bool bNewFixedCam);
	// Toggles between fixed and free camera
	void ToggleFixedCam();

	// Whether the player is allowed to enter and exit god mode
	UFUNCTION(BlueprintPure)
	bool CanToggleInvincible() const;
	// Toggles god mode on or off on the character
	UFUNCTION(BlueprintCallable)
	void ToggleInvincible(bool bVehicle);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerToggleInvincible(bool bVehicle);

public:
	// Whether the player is allowed to pin vehicles
	UFUNCTION(BlueprintPure)
	bool CanPinVehicle() const;
	// Sets a specific pin mode for the current vehicle
	UFUNCTION(BlueprintCallable)
	void SetVehiclePinMode(EVehiclePinMode PinMode);
	// Toggles between the given pin mode and no pinning
	void ToggleVehiclePinned(EVehiclePinMode PinMode);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetVehiclePinMode(EVehiclePinMode PinMode);

public:
	// Whether the player is allowed to teleport between vehicles
	bool CanCycleVehicles() const;
	// Cycles through all vehicles on the map
	void CycleVehicles(bool bReverse);

private:
	// NOTE: We do this on a special RPC rather than sending the vehicle to possess because the client doesn't always know about all of them
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCycleVehicles(bool bReverse);

public:
	// Whether the player can enable slow motion
	UFUNCTION(BlueprintPure)
	bool CanUseSlowMotion() const;
	// Toggles slow motion on and off
	UFUNCTION(BlueprintCallable)
	void ToggleSlowMotion();
	// Adjusts the slow motion speed up or down with 1 or -1, 2 and -2 are used to go to the max and min speed
	UFUNCTION(BlueprintCallable)
	void AdjustSlomoSpeed(int32 Dir);
	// Pauses/unpauses the physics
	void TogglePause();

	// Whether the player is allowed to use the projectile camera
	bool CanUseProjectileCamera() const;
	// Toggles the projectile camera enabled state
	void ToggleProjectileCamera();

private:
	// Enables or disables projectile camera
	void SetProjectileCameraEnabled(bool bNewEnabled);

public:
	// Whether the player is allowed to possess any character
	bool CanPossessCharacter() const;
	// Whether this player is currently allowed to possess the given character
	UFUNCTION(BlueprintPure)
	bool CanPossessCharacter(ABaseCharacter* Char) const;
	// Lets the player possess the given character if possible
	UFUNCTION(BlueprintCallable)
	void PossessCharacter(ABaseCharacter* Char);
	// Tries to find another character the player could possess
	ABaseCharacter* FindNextCharacterToPossess(bool bForward) const;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPossessCharacter(ABaseCharacter* Char);

public:
	// Whether the player is allowed to destroy any character
	bool CanDestroyCharacter() const;
	// Whether the player is allowed to destroy the given character
	UFUNCTION(BlueprintPure)
	bool CanDestroyCharacter(ABaseCharacter* Char) const;
	// Destroys the given character if possible
	UFUNCTION(BlueprintCallable)
	void DestroyCharacter(ABaseCharacter* Char);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyCharacter(ABaseCharacter* Char);

public:
	// Whether the player is allowed to spawn a dummy, set bAtAll to true to ignore the dummy limit on the server
	UFUNCTION(BlueprintPure)
	bool CanSpawnDummy(bool bAtAll = false) const;
	// Tries to spawn a dummy character at the given position
	UFUNCTION(BlueprintCallable)
	void SpawnDummy(const FVector& Location, float Yaw);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnDummy(const FVector& Location, uint16 CompressedYaw);

public:
	// Whether the player is allowed to cycle between characters
	bool CanCycleCharacters() const;
	// Cycles through all unpossessed characters in the level
	void CycleCharacters(bool bForward);

private:
	// NOTE: We do this on a special RPC rather than sending the character to possess because the client doesn't always know about all of them
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCycleCharacters(bool bForward);

public:
	// Whether the player is allowed to recover all dummies
	UFUNCTION(BlueprintPure)
	bool CanRecoverAllDummies() const;
	// Recovers all dummies in the level
	UFUNCTION(BlueprintCallable)
	void RecoverAllDummies();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRecoverAllDummies();

public:
	// Whether the player is allowed to destroy all dummies
	UFUNCTION(BlueprintPure)
	bool CanDestroyAllDummies() const;
	// Destroys all dummies in the level
	UFUNCTION(BlueprintCallable)
	void DestroyAllDummies();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDestroyAllDummies();
	// ~Sandbox

public:
	// ~Vehicle
	// Whether the player is allowed to scrap any vehicle
	bool CanScrapVehicle() const;
	// Return whether the player is allowed to scrap the given vehicle
	UFUNCTION(BlueprintPure)
	bool CanScrapVehicle(ABrickVehicle* InVehicle, bool bInPerson) const;
	// Scrap/remove a vehicle from the map
	UFUNCTION(BlueprintCallable)
	void ScrapVehicle(ABrickVehicle* InVehicle, bool bInPerson);
	// Whether the player is allowed to scrap all vehicles
	UFUNCTION(BlueprintPure)
	bool CanScrapAllVehicles() const;
	// Removes vehicles from the map
	UFUNCTION(BlueprintCallable)
	void ScrapAllVehicles(bool bIncludeUsed);
	// Whether the player is allowed to explode their current vehicle
	UFUNCTION(BlueprintPure)
	bool CanExplodeVehicle() const;
	// Lets the player explode their vehicle
	UFUNCTION(BlueprintCallable)
	void ExplodeVehicle();

	// Sends a vehicle movement update to the server
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdateVehicleMovement(ABrickVehicle* Vehicle, const TArray<FRepBrickMovementState>& MovementSates, const float OwnerTimestamp, const FRestartTransformVersion& ComparisonIndex);
	// Version that also sends along broken broken connections
	// NOTE: This HAS to be reliable, otherwise the broken connections would get lost
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateVehicleMovementAndDamage(ABrickVehicle* Vehicle, const TArray<FRepBrickMovementState>& MovementSates, const float OwnerTimestamp, const FBrickConnectionDamageBitfield& ConnectionDamage, const FRestartTransformVersion& ComparisonIndex);

private:
	// Lets the client know their damage was rejected
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientRevertVehicleDamage(ABrickVehicle* Vehicle, const FRestartTransformVersion& ComparisonIndex);
	// Returns true if the damage update can be applied
	bool ValidateVehicleDamageUpdate(const ABrickVehicle* Vehicle) const;

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerScrapVehicle(ABrickVehicle* InVehicle, bool bInPerson);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerScrapAllVehicles(bool bIncludeUsed);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExplodeVehicle();

public:
	// Called from vehicle download replicators when their download progress has changed
	void OnVehicleDownloadProgressChanged(ABrickVehicleDownloadReplicator* InDownloadReplicator, const TOptional<float>& Progress);
	// Called when any vehicle in the level has been constructed or deconstructed
	void OnAnyVehicleConstructedOrDeconstructed(ABrickVehicle* InVehicle, bool bNewConstructed);
	// Get the number of vehicles spawned by the player
	int32 GetNumSpawnedVehicles(bool bExcludeCurrent = false) const;
	// Executes a function on each vehicle the player has spawned
	void ForEachSpawnedVehicle(const TFunction<void(ABrickVehicle*)>& Func) const;
	// ~Vehicle

	// ~Inventory
	// Whether the inventory can currently be accessed
	UFUNCTION(BlueprintPure)
	bool CanViewInventory() const;
	// Whether the inventory popup is currently visible
	UFUNCTION(BlueprintPure)
	bool IsInventoryOpen() const;
	// Opens the inventory interface
	UFUNCTION(BlueprintCallable)
	void OpenInventory();
	// Opens the inventory interface to access another inventory
	UFUNCTION(BlueprintCallable)
	void AccessInventory(UInventoryComponent* InAccessedInventory);
	// Closes the inventory interface
	UFUNCTION(BlueprintCallable)
	void CloseInventory();
	// Toggles between the inventory open and closed
	void ToggleInventory();
	// Get the inventory component of the owned character
	UInventoryComponent* GetOwnInventory() const;
	// Whether the player is allowed to view the inventory
	UFUNCTION(BlueprintPure)
	bool CanAccessInventory(UInventoryComponent* InInventory) const;
	// Return the currently accessed inventory
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetAccessedInventory() const;
	// Return whether we need to replicate the accessed inventory via our replicated inventory component
	bool NeedsReplicatedInventory() const;

private:
	// Callback for the inventory popup
	void OnInventoryPopupClosed(EPopupResult Result);
	// For internal use only, sets the accessed inventory and calls the delegate
	void SetAccessedInventory(UInventoryComponent* InInventory);
	// Let the server set the accessed inventory so it can be replicated
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAccessInventory(UInventoryComponent* InInventory);
	// To be called when the accessed inventory has changed
	void OnAccessedInventoryChanged(UInventoryComponent* OldInventory);
	// Callback for the accessed inventory
	void OnAccessedInventoryModified();
	// ~Inventory

public:
	// ~Interaction
	// Whether the player can currently interact with interaction components
	UFUNCTION(BlueprintPure)
	bool CanInteract() const;
	// Used by the input component to determine whether it should show interaction
	EInteractionVisibility GetInteractionVisibility() const;
	// Get the currently maximum allowed interaction distance
	float GetMaxInteractionDistance() const;
	// Get the current draw distance scale factor for HUD icons
	float GetHUDIconDrawDistanceScale() const;
	// Find the interactive component the player is currently looking at
	UFUNCTION(BlueprintCallable)
	UHUDIconComponent* FindFocusedHUDIconComponent() const;
	// Starts an async trace to find the pickupable brick the player is looking at
	FTraceHandle FindFocusedBrick(const FTraceDelegate& InDelegate) const;

protected:
	// Helper function to check if the client is actually close enough to the target in order to interact
	bool ValidateClientInteraction(AActor* InteractionTarget) const;
	// Version that takes an exact interactive location
	bool ValidateClientInteraction(const FVector& Location) const;

public:
	// Used by the coupling bricks to engage/disengage over the server
	void EngageCouplingBrick(UCouplingBrick* CouplingBrick, bool bEngage);
	// Lets the character detonate a detonator via interaction
	void TriggerDetonatorBrick(UDetonatorBrick* DetonatorBrick);
	// Operates a switch via interaction
	void SetSwitchBrickValue(USwitchBrick* SwitchBrick, int32 NewValue);
	// Used to pick up and place a winch brick
	void StartAttachWinchBrick(UWinchBrick* WinchBrick);
	// Drops the current winch
	bool StopAttachWinchBrick();
	// Used to attach the current winch
	bool AttachWinchBrick();
	// Get the target we should attach our winch to
	FWinchAttachTarget GetWinchAttachTarget() const;
	// Get the winch being attached
	UWinchBrick* GetAttachingWinchBrick() const
	{
		return AttachingWinchBrick.Get();
	}

	// Used to attach  or detach a winch to and from anything
	void AttachWinchBrick(UWinchBrick* WinchBrick, const FWinchAttachTarget& AttachTarget);
	// Used to open/close a gravel silo via interaction
	void OpenGravelSilo(AGravelSilo* Silo, bool bOpen);
	// Used to interact with elevators
	void SetElevatorDirection(AElevator* Elevator, EElevatorDirection Dir);
	// Start the timer on a drag strip
	void StartDragRace(ADragStrip* DragStrip);
	// Marks an interacted actuator as needing to be replicated to the server
	void ReplicateActuatorToServer(UActuatorBrick* Actuator);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEngageCouplingBrick(UCouplingBrick* CouplingBrick, bool bEngage);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerTriggerDetonatorBrick(UDetonatorBrick* DetonatorBrick);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetSwitchBrickValue(USwitchBrick* SwitchBrick, int8 NewValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAttachWinchBrick(UWinchBrick* WinchBrick, const FWinchAttachTarget& AttachTarget);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOpenGravelSilo(AGravelSilo* Silo, bool bOpen);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetElevatorDirection(AElevator* Elevator, EElevatorDirection Dir);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartDragRace(ADragStrip* DragStrip);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateActuator(const FRepActuatorState& NewState, const FRestartTransformVersion& RestartVersion);
	// ~Interaction

public:
	// ~Kicking
	// Called from the game session to kick this player
	void OnKicked(const FString& KickReason, const FTimespan& KickDuration);
	// Returns true if OnKicked has been called before
	bool HasBeenKicked() const;

private:
	// Our custom version of ClientWasKicked so we can pass along a duration for the kick
	UFUNCTION(Client, Reliable)
	void ClientWasKickedForDuration(const FString& KickReason, const FTimespan& KickDuration);
	// Called when the player has been kicked either with or without a duration
	void OnClientWasKicked(const FString& KickReason, const FTimespan* KickDuration);
	// ~Kicking

public:
	// ~Spawning
	// Get the time remaining until the player is allowed to spawn again
	UFUNCTION(BlueprintPure)
	float GetRespawnTimerRemaining() const;
	// Whether the player can currently teleport, works parallel to CanRestartPlayer
	UFUNCTION(BlueprintPure)
	bool CanTeleportPlayer() const;
	// Whether the player is allowed to spawn/teleport anywhere on the map
	UFUNCTION(BlueprintPure)
	bool CanSpawnAnywhere() const;
	// Whether the player is allowed to restart/respawn at the current location
	UFUNCTION(BlueprintPure)
	bool CanRestartOnSpot() const;
	// Respawn the player at the current position
	UFUNCTION(BlueprintCallable)
	void RestartOnSpot();
	// Respawn at the closest spawn point
	UFUNCTION(BlueprintCallable)
	void RestartAtNextSpawnPoint();
	// Returns whether the player is currently allowed to spawn the vehicle
	UFUNCTION(BlueprintPure)
	EPlayerSpawnResult CanSpawnVehicle(const FUGCFileInfo& FileInfo, bool bRemoveCurrent) const;
	// Whether the player is currently allowed to spawn any vehicle
	bool CanSpawnVehicle() const;
	// Whether the player is allowed to spawn vehicles while they are dead
	bool CanSpawnVehicleWhileDead() const;
	// Whether the player is allowed to restart/teleport their current vehicle
	UFUNCTION(BlueprintPure)
	bool CanRestartVehicle() const;
	// Returns whether the player is allowed to restart with the given request, also tries to modify it to make it legal
	UFUNCTION(BlueprintPure)
	EPlayerSpawnResult CanRestartAt(FPlayerSpawnRequest& InOutSpawnRequest, bool bForceRestart = false);
	// Try to restart at a specific spawn point, world location optionally with a new vehicle
	UFUNCTION(BlueprintCallable)
	EPlayerSpawnResult RestartAt(FPlayerSpawnRequest SpawnRequest);
	// Called by the game mode upon successful restart
	void OnRestarted();
	// Notifies the client about a restart failure
	UFUNCTION(Client, Reliable)
	void ClientOnFailedToRestart(EPlayerSpawnResult SpawnResult, const FVehicleSpawnProperties& Props);
	// Returns the display text for the given spawn failure reason
	FText GetVehicleSpawnResultDisplayText(EPlayerSpawnResult SpawnResult, const FVehicleSpawnProperties& Props) const;

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRestartAt(const FPlayerSpawnRequest& SpawnRequest);
	// Find the spawn point closest to the player
	AActor* FindClosestSpawnPoint(bool bForVehicle) const;
	// Starts the unfreeze timer
	void SetupUnFreezeTimer();
	// ~Spawning

public:
	// ~Context Menu
	// Used by the context menu to gather all active input components
	void BuildInputStackForInputHelp(TArray<UBaseInputComponent*>& OutInputComponents);
	// ~Context Menu

	// ~Hit Detection
	// Called to notify the player that they have spawned a new projectile
	void OnProjectileSpawned(ABrickProjectile* Projectile);
	// Whether hit damage should be applied locally
	bool ShouldApplyLocalHitDamage(bool bIsExplosiveProjectile, const FHitResult& Hit) const;
	// Called when a projectile has hit a replicated actor
	void OnProjectileHit(bool bIsExplosiveProjectile, const FHitResult& Hit, const FProjectileHitInfo& HitInfo);
	// Called when the character has hit something
	void OnMeleeHit(const FHitResult& Hit, const FRepHitInfo& HitInfo);

private:
	// Whether the hit may cause gameplay relevant damage and thus has to be replicated
	bool IsHitGameplayRelevant(const FHitResult& Hit) const;
	// Whether a hit needs to be sent to the server
	bool ShouldSendHitToServer(const FHitResult& Hit) const;
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnProjectileHit(const FProjectileHitInfo& HitInfo);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnMeleeHit(const FRepHitInfo& HitInfo);
	// ~Hit Detection

public:
	// ~Spectating
	// Get the currently spectated character
	UFUNCTION(BlueprintPure)
	ABaseCharacter* GetSpectatedCharacter() const
	{
		return SpectatedCharacter;
	}

	// Whether this player is allowed to spectate the given character
	UFUNCTION(BlueprintPure)
	bool CanSpectateCharacter(ABaseCharacter* InCharacter) const;
	// Start spectating a specific player, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetSpectatedCharacter(ABaseCharacter* InCharacter);
	// Try to spectate the previous or next character, returns true if a new character is being spectated
	UFUNCTION(BlueprintCallable)
	bool SpectateNextCharacter(bool bForward);

private:
	// We need a server RPC since not all characters are replicated
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpectateNextCharacter(bool bForward);
	// Called any time the spectated character has changed
	void OnSpectatedCharacterChanged();
	// ~Spectating

public:
	// ~Cheating
	// Whether the player is currently allowed to cheat
	UFUNCTION(BlueprintPure)
	bool CanCheat();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCheatScore(float Amount);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCheatMoney(float Amount);
	// ~Cheating

	// ~Editor
	// Whether the player is in any editor, client only
	UFUNCTION(BlueprintPure)
	bool IsInEditor(EUGCType InUGCType = EUGCType::None) const;
	// Gets the editor instance
	UFUNCTION(BlueprintPure)
	ABrickEditor* GetBrickEditor() const
	{
		return BrickEditor;
	}

	// Returns the actor the editor was entered through
	UFUNCTION(BlueprintPure)
	AActor* GetEditorEntryPoint() const
	{
		return EditorEntryPoint;
	}

	// Utility to get the input component from the editor
	UEditorInputComponent* GetBrickEditorInputComponent() const;
	// Enters the vehicle editor with the current vehicle
	UFUNCTION(BlueprintCallable)
	void EnterVehicleEditor(AActor* EntryPoint);
	// Whether the player is allowed to use the given editor
	UFUNCTION(BlueprintCallable)
	bool CanEnterEditor(EUGCType InUGCType, AActor* EntryPoint) const;
	// Enters the editor for the given UGC type, optionally opening a file
	UFUNCTION(BlueprintCallable)
	bool EnterEditor(EUGCType InUGCType, const FUGCFileInfo& InFileInfo, bool bImport, AActor* EntryPoint);
	// Exits the current editor, optionally with a vehicle
	UFUNCTION(BlueprintCallable)
	void ExitEditor(const FUGCFileInfo& InFileInfo);
	// If in the editor with unsaved changes, opens a popup to confirm that they will be lost. Returns true if there are unsaved changes
	bool WarnAboutUnsavedEditorChanges(const FOnPopupClosed& Delegate);

protected:
	// Callback for the unsaved changes popup
	void OnUnsavedChangesPopupClosed(EPopupResult Result);
	// Used to spawn an editor instance when entering one
	void SpawnBrickEditor(UClass* EditorClass);
	// Sets the current editor
	void SetBrickEditor(ABrickEditor* NewEditor);
	// ~Editor

public:
	// ~IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface

private:
	// Helper function used to find the next character in line that meets the given predicate
	ABaseCharacter* FindNextCharacterByPredicate(ABaseCharacter* Start, bool bForward, const TFunction<bool(ABaseCharacter*)>& Predicate) const;
};
