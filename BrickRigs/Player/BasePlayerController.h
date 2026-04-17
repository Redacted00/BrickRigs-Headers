// Copyright Fluppisoft, 2019

#pragma once

#include "UI/UGC/UGCBrowserWidget.h"
#include "PlayerControllerStaticInfo.h"
#include "UI/Input/BrickInputMode.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Online/KickedPlayer.h"
#include "BasePlayerController.generated.h"

class IViewTargetInterface;
/**
 * This should be used as the base class for all player controllers, for shared functionality such as input
 */
UCLASS()
class BRICKRIGS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnKickedPlayerListReceived, const FKickedPlayerList&);

	// ~Variables
	FPopupHandle PopupHandle_JoinSession;
	FPopupHandle PopupHandle_CheatMenu;
	FPopupHandle PopupHandle_KickPlayer;

	// Used to delay updating the kicked player list
	FFluRealTimer Timer_RequestKickedPlayerList;
	// Pending kicked player list request
	FKickedPlayerListRequest KickedPlayerListRequest;
	// Most recent request id
	uint8 KickedPlayerListRequestId;

private:
	// Whether the local player has received all important information to start playing
	uint8 bIsLocalPlayerReady : 1;
	// ~Variables

protected:
	// ~Properties
	// Shared player controller properties class
	UPROPERTY(EditDefaultsOnly, NoClear, Category = Defaults)
	TSubclassOf<UPlayerControllerStaticInfo> StaticInfoClass;
	// ~Properties

public:
	// ~Delegates
	FOnKickedPlayerListReceived OnKickedPlayerListReceivedDelegate;
	FSimpleMulticastDelegate OnKickedPlayerListChangedDelegate;
	// ~Delegates

	// ~Constructor
	ABasePlayerController();

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	virtual void ReceivedPlayer() override;
	virtual void SpawnPlayerCameraManager() override;
	virtual void SetupInputComponent() override;
	virtual void InitPlayerState() override;
	virtual void OnRep_PlayerState() override;
	virtual void ChangeState(FName NewState) override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void UpdateStateInputComponents() override;
	virtual void UpdateRotation(float DeltaTime) override;
	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;
	virtual void SetName(const FString& S) override;
	virtual void ServerChangeName_Implementation(const FString& S) override;
	virtual void PreClientTravel(const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel) override;
	virtual void NotifyLoadedWorld(FName WorldPackageName, bool bFinalDest) override;
	virtual void GetSeamlessTravelActorList(bool bToEntry, TArray<AActor*>& ActorList) override;
	virtual void ReceivedGameModeClass(TSubclassOf<AGameModeBase> GameModeClass) override;
	virtual ASpectatorPawn* SpawnSpectatorPawn() override;
	// ~Super Interface

	// ~Player
	// Get the static info CDO, this can always be assumed to return a valid pointer
	UFUNCTION(BlueprintPure)
	const UPlayerControllerStaticInfo* GetStaticInfo() const
	{
#if WITH_EDITOR
		if (!StaticInfoClass)
		{
			ensure(false);
			return UPlayerControllerStaticInfo::StaticClass()->GetDefaultObject<UPlayerControllerStaticInfo>();
		}
#endif

		return StaticInfoClass.GetDefaultObject();
	}

	// Returns the camera manager casted to our custom class
	ABrickCameraManager* GetBrickCameraManager() const;
	// Return this player's id
	UFUNCTION(BlueprintPure)
	FUniqueNetIdRepl GetPlayerId() const;

	// Update the current view target
	UFUNCTION(BlueprintCallable)
	void UpdateViewTarget();

protected:
	// Returns the actor that should currently be view target
	UFUNCTION(BlueprintNativeEvent)
	AActor* GetDesiredViewTarget(FViewTargetTransitionParams& OutTransitionParams) const;
	// ~Player

public:
	// ~Pawn
	// Returns the pawn that should currently be used as the view target
	virtual APawn* GetViewTargetPawn() const;
	// Get the view target interface that is consuming the control rotation currently
	IViewTargetInterface* GetControlRotationInterface() const;
	// ~Pawn

	// ~Ready
	// Whether the local player has received all critical data to start playing
	bool IsLocalPlayerReady() const
	{
		return bIsLocalPlayerReady;
	}

protected:
	// To be called when a factor for the ready status has changed
	void UpdateLocalPlayerReady();
	// To be called upon changing the level or being destroyed
	void ResetLocalPlayerReady();
	// Called when the local player became ready
	virtual void OnLocalPlayerReady();
	// Called when the local player ready state was reset
	virtual void OnLocalPlayerUnready();
	// ~Ready

public:
	// ~Online
	// Used to join a session via an invite or the server browser, will open a confirmation popup first
	bool JoinSession(const FOnlineSessionSearchResult& SearchResult);
	// ~Online

	// ~Camera
	// Return the default field of view angle
	float GetDefaultFOV() const;
	// Whether the view rotation input should be scaled 
	virtual bool ShouldScaleRotationInputToFOV() const;
	// ~Camera

	// ~Fading
	// Sets the default manual fade out state
	void InitializeCameraFade(bool bFadeHUD);
	// Starts a camera fade animation
	void StartCameraFade(const FCameraFadeParams& Params, bool bFadeOut);
	// Starts a fade with the level transition fade parameters
	void StartLevelTransitionFade(bool bFadeOut);
	// Get the fade out time for level transitions
	float GetLevelTransitionFadeOutTime() const;
	// Get the fade time for ending a match
	float GetMatchEndFadeOutTime() const;
	// ~Fading

	// ~UI
	// These input events are called from the viewport client
	virtual void OnTappedMenu();
	virtual void OnHeldMenu();
	// Whether the in game or main menu is currently open
	bool IsMenuOpen() const;
	// Whether the menu can currently be opened or closed
	virtual bool CanOpenOrCloseMenu(bool bOpen) const;
	// Called from the window manager when the menu has been opened or closed
	virtual void OnMenuOpenChanged(bool bNewOpen);
	// Opens or closes the cheat menu
	UFUNCTION(BlueprintCallable)
	void ToggleCheatMenuOpen();

	// Update the input mode and focused widget, i.e. toggle between UI and game
	void UpdateInputMode();
	// Get basic parameters for the desired input mode
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const;
	// ~UI

	// ~Kicking
	// Whether the player is allowed to view, issue, revoke and edit kicks
	UFUNCTION(BlueprintPure)
	virtual bool CanViewAndEditKicks() const;
	// Opens a popup to kick the given player
	UFUNCTION(BlueprintCallable)
	void OpenKickPlayerPopup(const FUniqueNetIdRepl& OtherPlayerId, const FString& OtherPlayerName);
	// Return whether this player is allowed to kick another one
	UFUNCTION(BlueprintPure)
	bool CanKickPlayer(const FUniqueNetIdRepl& OtherPlayerId) const;
	// Kicks a player from the server
	UFUNCTION(BlueprintCallable)
	void KickPlayer(const FUniqueNetIdRepl& OtherPlayerId, const FString& OtherPlayerName, const FString& KickReason, const FTimespan& KickDuration);
	// Revoke the kick for another player
	UFUNCTION(BlueprintCallable)
	void RevokeKick(const FUniqueNetIdRepl& OtherPlayerId);

private:
	// Server RPC functions
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerKickPlayer(const FUniqueNetIdRepl& OtherPlayerId, const FString& OtherPlayerName, const FString& KickReason, const FTimespan& KickDuration);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRevokeKick(const FUniqueNetIdRepl& OtherPlayerId);
	// ~Kicking

public:
	// ~Kick List
	// Requests the given kicked player list page from the server, return true if the request was successfully started
	bool RequestKickedPlayerList(const FKickedPlayerListRequest& Request);
	// Allows the server to notify clients about the kicked player list changing
	UFUNCTION(Client, Reliable)
	void ClientOnKickedPlayerListChanged();

private:
	// Internal version that is called from the server
	bool RequestKickedPlayerListInternal(const FKickedPlayerListRequest& Request, uint8 RequestId);
	// Used to send the player list request to the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestKickedPlayerList(const FKickedPlayerListRequest& Request, uint8 RequestId);
	// Used to send the list back to the client
	UFUNCTION(Client, Reliable)
	void ClientReceiveKickedPlayerList(const FKickedPlayerList& PlayerList, uint8 RequestId);
	// Callback for the request kicked players timer
	void OnRequestKickedPlayerListTimer();
	// ~Kick List
};
