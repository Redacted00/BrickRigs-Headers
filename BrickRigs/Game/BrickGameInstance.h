// Copyright Fluppisoft, 2015

#pragma once

#include "Misc/BrickAssetManager.h"
#include "UGC/UGCTypes.h"
#include "FindSessionsCallbackProxy.h"
#include "FluUGCAsyncTask.h"
#include "UI/Widgets/WindowManagerWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Misc/BrickTimer.h"
#include "Tickable.h"
#include "Engine/GameInstance.h"
#include "BrickGameInstance.generated.h"

class IPlugin;
class UBrickGameInstance;
class UPropertyWidget;
class ULoadingScreenWidget;
class UBrickMaterial;
class UBrickDecal;
class USirenType;
class USoundCue;
class UWeatherCondition;
class ULevelInfo;
class UGameModeInfo;
class UModHook;

UENUM(BlueprintType)
enum class EJoinSessionState : uint8
{
	None,
	JoinSession,
	ClientTravel,
	Cancelled
};

struct FNetworkError
{
	FText Title;
	FText Message;

	FNetworkError(const FText& InTitle = FText(), const FText& InMessage = FText())
		: Title(InTitle), Message(InMessage)
	{
	}
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickGameInstance : public UGameInstance, public FTickableGameObject
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnNetworkError);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnJoinSessionStateChanged, const EJoinSessionState);
	DECLARE_MULTICAST_DELEGATE(FOnFindSessionsStart);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFindSessionsResult, const TArray<FOnlineSessionSearchResult>&, bool);
	DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUGCQueryComplete, const TArray<FUGCQueryResult>&, uint32, uint32);
	DECLARE_MULTICAST_DELEGATE(FOnUGCUploadStarted);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUGCUploadComplete, const EFluUGCUploadResult, const FFluUGCItemIdWrapper&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUGCThumbnailCaptured, const FString&);

	// ~Variables
	FBrickTimer Timer_FadeOut;
	FBrickTimer Timer_PlayNextMenuMusic;
	// Handle used to load the menu music
	FSmartStreamableHandle StreamableHandle_MenuMusic;
	// Online subsystem delegate handles
	FDelegateHandle DelegateHandle_FindSessions;
	FDelegateHandle DelegateHandle_JoinSession;
	FDelegateHandle DelegateHandle_DestroySession;
	FDelegateHandle DelegateHandle_SessionInviteAccepted;
	// UGC subsystem tasks
	FFluUGCAsyncTaskHandle UGCTask_Query;
	FFluUGCAsyncTaskHandle UGCTask_Upload;
	// Delegates used for the streaming pause rendering callbacks
	FBeginStreamingPauseDelegate BeginStreamingPauseRenderingDelegate;
	FEndStreamingPauseDelegate EndStreamingPauseRenderingDelegate;
	// Current UGC upload percentage
	float UploadProgress;
	// Currently active brick timers
	TArray<FBrickTimer*> BrickTimers;

	// Whether this is the first time the main menu has been opened
	uint8 bIsFirstTimeInMainMenu : 1;
	// Helper flag to set bIsFirstTimeInMainMenu
	uint8 bHasMainMenuBeenOpenBefore : 1;
	// Whether the menu music should currently be playing
	uint8 bShouldPlayMenuMusic : 1;
	// Set to true if the level loading screen should be shown
	uint8 bShowLevelLoadingScreen : 1;
	// Whether rendering is currently paused for level streaming
	uint8 bStreamingPauseRendering : 1;

	// The loading screen widget instance
	// IMPORTANT: The loading screen is separate from the window manager because the movie player does not support retainer boxes
	// And because it makes sense that the window manager isn't displayed via the movie player
	UPROPERTY(Transient)
	ULoadingScreenWidget* LoadingScreenWidget;
	// The window manager widget instance
	UPROPERTY(Transient)
	UWindowManagerWidget* WindowManagerWidget;
	// Audio component used for the menu and loading screen music
	TWeakObjectPtr<UAudioComponent> MusicAudioComponent;
	// Current fade amount of the HUD
	float HUDFadeAmount;

	// The latest network error pending to be displayed
	TOptional<FNetworkError> NetworkError;
	// Reference to the ongoing session search
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	// We use this extra array to keep track of all search results
	TArray<FOnlineSessionSearchResult> SessionSearchResults;
	// State of the current join session attempt
	EJoinSessionState JoinSessionState;
	// The session that we are about to join
	FOnlineSessionSearchResult PendingJoinSession;
	// The password given by the player to join the next server
	FString PendingJoinPassword;

public:
#if WITH_EDITOR
	// Net mode PIE was started with
	EPlayNetMode PIENetMode;
#endif
	// ~Variables

protected:
	// ~Properties
	// Widget class used for the loading screen
	UPROPERTY(EditDefaultsOnly, Category = GameInstance)
	TSubclassOf<ULoadingScreenWidget> LoadingScreenWidgetClass;
	// Widget used for the main window manager
	UPROPERTY(EditDefaultsOnly, Category = GameInstance)
	TSubclassOf<UWindowManagerWidget> WindowManagerWidgetClass;
	// Resolution to render UGC thumbnails at
	UPROPERTY(EditDefaultsOnly, Category = GameInstance)
	FIntPoint UGCThumbnailResolution;
	// Sound cue used to host the menu music waves
	UPROPERTY(EditDefaultsOnly, Category = GameInstance)
	USoundCue* MenuMusicSoundCue;
	// ~Properties

public:
	// ~Delegates
	FOnNetworkError OnNetworkErrorDelegate;
	FOnJoinSessionStateChanged OnJoinSessionStateChangedDelegate;
	// Called when a session search has been started
	FOnFindSessionsStart OnFindSessionsStartDelegate;
	// Broadcast when the session search has finished or new results were discovered
	FOnFindSessionsResult OnFindSessionsResultDelegate;
	// Broadcast when the current UGC query has completed
	FOnUGCQueryComplete OnUGCQueryCompleteDelegate;
	// Broadcast when an upload has been started
	FOnUGCUploadStarted OnUGCUploadStartedDelegate;
	// Broadcast the upload has finished
	FOnUGCUploadComplete OnUGCUploadCompleteDelegate;
	// Broadcast when a UGC thumbnail has been captured
	FOnUGCThumbnailCaptured OnUGCThumbnailCapturedDelegate;
	// ~Delegates

	// ~Statics
	// Returns the streamable manager instance for the game
	static FStreamableManager& GetStreamableManager()
	{
		return UBrickAssetManager::GetStreamableManager();
	}

	// ~Statics

	// ~Constructor
	UBrickGameInstance();

	// ~Super Interface
	virtual void Init() override;
	virtual void StartGameInstance() override;
#if WITH_EDITOR
	virtual FGameInstancePIEResult StartPlayInEditorGameInstance(ULocalPlayer* LocalPlayer, const FGameInstancePIEParameters& Params) override;
#endif
	virtual void Shutdown() override;
	virtual void PreloadContentForURL(FURL InURL) override;
	virtual int32 AddLocalPlayer(ULocalPlayer* NewPlayer, int32 ControllerId) override;
	virtual TSubclassOf<AGameModeBase> OverrideGameModeClass(TSubclassOf<AGameModeBase> GameModeClass, const FString& MapName, const FString& Options, const FString& Portal) const override;

private:
	virtual bool JoinSession(ULocalPlayer* LocalPlayer, const FOnlineSessionSearchResult& SearchResult) override;
	// ~Super Interface

public:
	// ~Game Instance
	// Get the brick game instance
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Game Instance"))
	static UBrickGameInstance* Get(const UObject* WorldContextObject);

private:
	// Called when the game instance is started in standalone or PIE
	void OnStartGameInstance();
	// ~Game Instance

public:
	// ~FTickableGameObject
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickableWhenPaused() const override;
	virtual void Tick(float DeltaTime) override;
	// ~FTickableGameObject

	// ~Brick Timers
	void RegisterBrickTimer(FBrickTimer* InTimer);
	void UnregisterBrickTimer(FBrickTimer* InTimer);
	// ~Brick Timers

	// ~Window Manager
	// Get the window manager widget
	UWindowManagerWidget* GetWindowManager() const;
	// Allows the camera manager to update the fade amount
	void SetHUDFadeAmount(float NewFadeAmount);

private:
	// To be called when the fade amount has changed, will apply it
	void ApplyHUDFadeAmount();

public:
	// Whether the popup with the given handle is open
	UFUNCTION(BlueprintPure)
	bool IsPopupOpen(const FPopupHandle& Handle) const
	{
		return WindowManagerWidget && WindowManagerWidget->IsPopupOpen(Handle);
	}

	// Creates popup params used to open a popup, return value can always assumed to be valid
	template <class T = UPopupParams>
	T* CreatePopupParams(const TSoftClassPtr<UPopupWidget>& InWidgetClass)
	{
		auto* OutParams = NewObject<T>(this, NAME_None, RF_Transient);
		OutParams->PopupClass = InWidgetClass;
		return OutParams;
	}

	// Blueprint version
	UFUNCTION(BlueprintCallable)
	UPopupParams* CreatePopupParams(TSubclassOf<UPopupParams> ParamsClass)
	{
		// Fallback to make sure a valid pointer is always returned
		if (!ParamsClass)
		{
			return NewObject<UPopupParams>(this, NAME_None, RF_Transient);
		}
		return NewObject<UPopupParams>(this, ParamsClass, NAME_None, RF_Transient);
	}

	// Version that takes an already construct popup params object
	UFUNCTION(BlueprintCallable)
	bool OpenPopup(FPopupHandle& Handle, UPopupParams* PopupParams, bool bToggleOpen = false)
	{
		if (WindowManagerWidget)
		{
			return WindowManagerWidget->OpenPopup(Handle, PopupParams, bToggleOpen);
		}

		return false;
	}

	// Version that opens a simple message popup
	bool OpenMessagePopup(FPopupHandle& Handle, const FText& Message, bool bCanCancel = false, const FOnPopupClosed& ClosedDelegate = FOnPopupClosed())
	{
		auto* PopupParams = CreatePopupParams<UMessagePopupParams>(nullptr);
		PopupParams->TitleText = Message;
		PopupParams->bCanCancel = bCanCancel;
		PopupParams->OnClosedDelegate = ClosedDelegate;
		return OpenPopup(Handle, PopupParams);
	}

	// Blueprint version
	UFUNCTION(BlueprintCallable)
	bool OpenMessagePopup(FPopupHandle& Handle, FText Message, bool bCanCancel, FOnPopupClosedScript ClosedDelegate)
	{
		return OpenMessagePopup(Handle, Message, bCanCancel, FOnPopupClosed::CreateUFunction(ClosedDelegate.GetUObject(), ClosedDelegate.GetFunctionName()));
	}

	// Closes a popup with the given handle
	UFUNCTION(BlueprintCallable)
	bool ClosePopup(FPopupHandle& Handle, EPopupResult Result = EPopupResult::ForceClose)
	{
		return WindowManagerWidget && WindowManagerWidget->ClosePopup(Handle, Result);
	}

	// ~Window Manager

	// ~Menu
	// Start a new local game according to the match user settings
	void StartLocalGame();
	// Gracefully quit the game after fading to black
	UFUNCTION(BlueprintCallable)
	bool QuitGame();
	// Fade out and then go to the main menu, optionally forcing an exit and skipping the unsaved changes popup
	UFUNCTION(BlueprintCallable)
	void OpenMainMenu(bool bForceExit = false);
	// Return whether the main menu is currently open
	bool IsInMainMenu() const;
	// Whether this is the first time the main menu has been opened
	bool IsFirstTimeInMainMenu() const
	{
		return bIsFirstTimeInMainMenu;
	}

	// Whether the menu is open or has been open before
	bool HasMainMenuBeenOpenBefore() const
	{
		return bHasMainMenuBeenOpenBefore;
	}

	// Whether the menu is currently open (in game or main menu)
	bool IsMenuOpen() const;
	// Called when the menu has been opened or closed
	void OnMenuOpenChanged(bool bNewOpen);

private:
	// Fades the players out before quitting the game or going to another level, takes care of unsaved changes an notifying clients
	void PrepareToLeaveLevel(void (UBrickGameInstance::*Callback)(), bool bForceExit = false);
	// Called when the fade out has been confirmed by the user
	void OnLeaveLevelUnsavedChangesPopupClosed(EPopupResult Result);
	// Timer callback for starting a local match
	void OnStartGameComplete();
	// Timer callback for when the client has faded out
	void OnQuitGameComplete();
	// Callback for the main menu transition callback
	void OnOpenMainMenuComplete();
	// Starts the next menu track
	void PlayMenuMusic();
	// Callback for the music audio component
	void OnMusicAudioComponentFinished(UAudioComponent* InAudioComponent);
	// Return whether the given map is the main menu
	bool IsMainMenuLevelName(const FString& InURL) const;
	// ~Menu

public:
	// ~User Interface
	// Opens the context menu at the current mouse position
	void OpenContextMenu(const FContextMenuParams& Params);
	// ~User Interface

	// ~Loading Screen
	// Shows or hides the loading screen as needed
	void UpdateLoadingScreen();
	// ~Loading Screen

	// ~Network Errors
	// Set a new error message, will be displayed in the menu
	void SetNetworkError(const FText& ErrorType, const FText& ErrorMessage, bool bOverride = false);
	// Return whether there is a network error that has not been processed yet
	bool HasNetworkError() const;
	// Gets the current network error, if any
	bool GetNetworkError(FNetworkError& OutError) const;
	// Marks the network error as read
	void ClearNetworkError();
	// Used to unify the kick message errors
	void SetKickNetworkError(const FString& KickReason, const FTimespan& TimeRemaining);

private:
	// Network error callbacks
	void OnTravelFailure(UWorld* World, ETravelFailure::Type Type, const FString& Message);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Type, const FString& Message);
	// ~Network Errors

public:
	// ~Sessions
	// Start a session search, returns true if a search was started
	bool FindSessions(ESearchSessionType SearchType, const FString& SearchText, bool bSearchPrivate, bool bSearchDifferentMods);
	// Whether there is currently a session search going on
	bool IsSessionSearchInProgress() const;
	// Whether the last session search has finished successfully
	bool WasLastSessionSearchSuccessful() const;
	// Get the search results of the find sessions task
	const TArray<FOnlineSessionSearchResult>& GetSessionSearchResults() const
	{
		return SessionSearchResults;
	}

	// Called from the join session popup to actually join the pending session
	void JoinPendingSession(const FOnlineSessionSearchResult& SearchResult, const FString& Password);
	// Cancel the current session join
	void CancelPendingSessionJoin();
	// Get the current join state
	EJoinSessionState GetJoinSessionState() const;

private:
	// Called when the user has confirmed joining the session
	void OnJoinPendingSessionUnsavedChangesPopupClosed(EPopupResult Result);
	// Changes the current join state
	void SetJoinSessionState(EJoinSessionState NewState);
	// Leaves the current session
	void DestroyCurrentSession();
	// Callback for the find sessions task
	void OnFindSessionsComplete(bool bWasSuccessful);
	// Callback function for the JoinSession call
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	// Callback for the DestroySession call
	void OnDestroySessionComplete(FName SessionName, bool bSuccess);
	// Callback for the session interface when an invite has been accepted
	void OnSessionUserInviteAccepted(const bool bWasSuccessful, const int32 ControllerIndex, TSharedPtr<const FUniqueNetId> UserId, const FOnlineSessionSearchResult& InviteResult);
	// ~Sessions

	// ~Misc
	// Initialize the main menu flags
	bool InitMainMenuFlags(const FString& WorldName);
	// Core Delegates
	void PreLoadMap(const FString& MapName);
	void PostLoadMap(UWorld* LoadedWorld);
	// Callbacks for the engine
	void BeginStreamingPauseRendering(FViewport* GameViewport);
	void EndStreamingPauseRendering();
	// ~Misc

public:
	// ~UGC
	// Whether an async UGC query is currently executing
	bool IsUGCQueryInProgress() const;
	// Starts a new UGC query, return true if it was started successfully
	bool QueryUGC(const FUGCQuerySettings& QuerySettings);
	// Cancels the current UGC query
	void CancelQueryUGC();
	// Starts a new UGC upload task
	bool StartUGCUpload(const FUGCFileInfo& FileInfo, const EFluUGCUploadFlags Flags);
	// Whether an upload is currently in progress
	bool IsUploadingUGC() const;
	// Get the current upload percentage
	float GetUGCUploadProgress() const;
	// Duplicates a local UGC file, returns true on success
	bool DuplicateUGCItem(const FUGCFileInfo& FileInfo);
	// Deletes a local UGC file or a published online UGC file
	bool DeleteUGCItem(const FUGCFileInfo& FileInfo);
	// Whether the given sort method can be used with the given query type
	bool IsValidUGCSortMethod(EUGCQueryType QueryType, EUGCSortMethod SortMethod) const;

private:
	// Converts a query type to an online query type
	EFluUGCQueryType ConvertUGCQueryTypeToOnlineQueryType(EUGCQueryType QueryType) const;
	// Converts a sort method to an online sort method
	EFluUGCSortMethod ConvertUGCSortMethodToOnlineSortMethod(EUGCSortMethod SortMethod) const;
	// Converts a sort method to a query result sort function for local files
	static FUGCQueryResult::FSortFunction ConvertUGCSortMethodToSortFunction(EUGCSortMethod SortMethod);
	// To be called after an online or local UGC query
	void OnUGCQueryCompleteInternal(const TArray<FUGCQueryResult>& Results, const uint32 NumTotalResults, const uint32 NumPages);

public:
	// Captures a new thumbnail for a given item
	void CaptureUGCThumbnail(UBrickEditorInterfaceComponent* Component, const FUGCFileInfo& FileInfo, bool bFocusView, bool bHideBackground, bool bShowNotification);
	// ~UGC

	// ~Online
	// Gets the unique app id of the game version
	FString GetAppId(const FName& OnlineSubsystemName = NAME_None) const;
	// ~Online

	// ~Mods
	static auto constexpr NoModsHash = 0;
	// Returns all enabled mod plugins
	static void GetEnabledModPlugins(TArray<TSharedRef<IPlugin>>& OutPlugins);
	// Returns a list of enabled mod names
	static void GetEnabledModNames(TArray<FString>& OutNames);
	// Returns a has based on all enabled mod names
	static int32 GetEnabledModsHash();
	// ~Mods
};
