// Copyright Fluppisoft, 2016

#pragma once

#include "Player/BrickPlayerController.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "Blueprint/UserWidget.h"
#include "GameOverlayWidget.generated.h"

class ASpectatorPawn;
class ABrickVehicle;
class ABrickEditor;
class UHUDWidget;
class UPingIndicatorWidget;
class UHUDNotificationWidget;
class UChatWidget;
class UInputHelpWidget;
class UMatchTimerWidget;
class UMatchStateWidget;
class UTeamScoreWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UGameOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY(Transient)
	UChatWidget* ChatWidget;
	UPROPERTY(Transient)
	UInputHelpWidget* InputHelpWidget;
	UPROPERTY(Transient)
	UMatchTimerWidget* MatchTimerWidget;
	UPROPERTY(Transient)
	UMatchStateWidget* MatchStateWidget;
	// All team score widgets created so far
	UPROPERTY(Transient)
	TArray<UTeamScoreWidget*> TeamScoreWidgets;
	// All notification widgets that have been created
	UPROPERTY(Transient)
	TArray<UHUDNotificationWidget*> HUDNotificationWidgets;
	// Used to accumulate the money that has been earned and spent
	float AccumulatedMoneyEarned;
	float AccumulatedMoneySpent;
	// The current spectator pawn
	UPROPERTY(Transient)
	ASpectatorPawn* SpectatorPawn;
	// Currently viewed character
	UPROPERTY(Transient)
	ABaseCharacter* ViewedCharacter;
	// Seat the character is in
	TWeakObjectPtr<USeatBrick> VehicleSeat;
	// The players current editor instance
	UPROPERTY(Transient)
	ABrickEditor* BrickEditor;
	// The current view target of the player
	TWeakObjectPtr<> ViewTargetSubobject;
	// Whether delegates for the HUD notifications are currently bound
	uint8 bHUDNotificationsRegistered : 1;
	// Whether the game overlay is attached to the HUD widget
	uint8 bIsAddedToHUDWidget : 1;
	// True while the input help is being opened
	uint8 bIsOpeningInputHelp : 1;
	// True while the chat is being opened
	uint8 bIsOpeningChat : 1;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPingIndicatorWidget* HighPingIndicator;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UInputHelpWidget> InputHelpClass;
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UHUDNotificationWidget> HUDNotificationClass;
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UMatchTimerWidget> MatchTimerWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UMatchStateWidget> MatchStateWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UTeamScoreWidget> TeamScoreWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	TSubclassOf<UChatWidget> ChatWidgetClass;
	// Maximum time of the match timer remaining where it will be displayed
	UPROPERTY(EditDefaultsOnly, Category = GameOverlay)
	int32 MinimalHUDMaxMatchTimerRemaining;
	// ~Properties

public:
	// ~Constructor
	UGameOverlayWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	// ~Super Interface

	// ~Game Overlay
	// Get the game overlay widget instance
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Game Overlay"))
	static UGameOverlayWidget* Get(const UObject* WorldContextObject);
	// Allows the game overlay to override input mode settings
	void OverrideInputMode(FBrickInputMode& OutInputMode);
	// ~Game Overlay

	// ~Chat
	// Whether the chat widget currently has focus
	bool IsChatFocused() const;
	// Shows and sets focus to the chat widgets
	bool OpenChat();
	// Clears focus and hides chat panel
	bool CloseChat() const;
	// Called from the chat widget when it lost focus
	void OnChatLostFocus();
	// Called from the chat when the user committed the chat message
	void OnChatTextCommitted() const;
	// Called from the chat when the fade out animation has finished
	void OnChatClosed();

private:
	// Manages the visibility of the chat widget
	void UpdateChatVisibility(const TArray<FBrickChatMessage>& ChatMessages = {}, bool bHUDVisibilityChanged = false, bool bForceVisible = false);
	// ~Chat

public:
	// ~Input Help
	// Whether the input help panel has focus
	bool IsInputHelpFocused() const;
	// Whether the input help panel is expanded
	bool IsInputHelpOpen() const;
	// Opens or closes the input help
	bool SetInputHelpOpen(bool bNewOpen, bool bUpdateFocus);
	// Toggles between input help open and closed
	UFUNCTION(BlueprintCallable)
	bool ToggleInputHelpOpen(bool bUpdateFocus);
	// Callback for the input help widget
	void OnInputHelpLostFocus();
	// ~Input Help

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddInputHelpWidget(UInputHelpWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddMatchTimerWidget(UMatchTimerWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddMatchStateWidget(UMatchStateWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddTeamScoreWidget(UTeamScoreWidget* Widget, ETeamAttitude::Type TeamAttitude);
	UFUNCTION(BlueprintImplementableEvent)
	void AddHUDNotification(UHUDNotificationWidget* Widget, int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void AddChatWidget(UChatWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHUDVisibilityNotification(UHUDNotificationWidget* Widget, EHUDVisibility NewVisibility);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateFreeCamNotification(UHUDNotificationWidget* Widget, bool bIsInFreeCam, EFreeCamMode InFreeCamMode, bool bIsFixedCam);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCameraSpeedNotification(UHUDNotificationWidget* Widget, float NewSpeedRatio, float NewMaxSpeed);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCameraZoomNotification(UHUDNotificationWidget* Widget, float NewZoomRatio);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCameraModeNotification(UHUDNotificationWidget* Widget, ECameraMode NewMode);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlomoNotification(UHUDNotificationWidget* Widget, float SlomoSpeed, bool bInvertSpeed, bool bSlomoEnabled, bool bIsPaused);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMoneyNotification(UHUDNotificationWidget* Widget, float NewAmount, float AmountAdded);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateThumbnailNotification(UHUDNotificationWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleSeatNotification(UHUDNotificationWidget* Widget, const FText& SeatDisplayName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleCameraNotification(UHUDNotificationWidget* Widget, const FText& CameraDisplayName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleDownloadNotification(UHUDNotificationWidget* Widget, const FText& VehicleDisplayName, float Progress);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateVehicleConstructionNotification(UHUDNotificationWidget* Widget, const FText& VehicleDisplayName, float Progress);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSaveNotification(UHUDNotificationWidget* Widget, bool bSuccess, bool bIsAutoSave);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateProjectileCameraNotification(UHUDNotificationWidget* Widget, bool bNewEnabled);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRespawnDelayNotification(UHUDNotificationWidget* Widget, float Remaining);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRestartFailedNotification(UHUDNotificationWidget* Widget, const FText& Message);
	// ~Blueprint Interface

public:
	// Whether HUD notifications should currently be displayed
	bool ShouldShowHUDNotifications(const FName& Context = NAME_None) const;
	// Creates a new notification widget or reuses one from the same context
	UFUNCTION(BlueprintCallable)
	UHUDNotificationWidget* CreateHUDNotification(const FName& Context, bool bShouldFadeOut = true);
	// Tries to find an existing notification for the given context
	UHUDNotificationWidget* FindHUDNotification(const FName& Context);
	// Ends the given notification
	void RemoveHUDNotification(UHUDNotificationWidget* InWidget);
	// Called from a notification when it has faded out
	void OnHUDNotificationFadedOut(UHUDNotificationWidget* Widget);
	// Called by the match state widget
	void OnMatchStateFadedOut();

private:
	// Updates the registered state of HUD notification delegates
	void UpdateHUDNotificationsRegistered();
	// Registers or unregisters the HUD icon delegates
	void SetHUDNotificationsRegistered(bool bNewRegistered);
	// Removes a notification at a specific index
	void RemoveHUDNotificationInternal(int32 Index);
	// Whether the slomo notification should currently be displayed
	bool ShouldPinSlomoNotification() const;
	// Get the context name that should be used for the download notification for the given vehicle
	FName GetVehicleDownloadNotificationContext(ABrickVehicleDownloadReplicator* InDownloadReplicator) const;
	// Used to handle all free cam notifications
	void ShowFreeCamNotification();

	// Updates the current input mode
	void UpdateInputMode() const;
	// Shows or hides the input context widget as needed
	void UpdateInputHelpVisibility(bool bForceVisible = false);
	// Removes the input help widget
	void RemoveInputHelp();
	// Whether the match timer should currently be visible
	bool ShouldShowMatchTimer() const;
	// Shows or hides the match timer if needed
	void UpdateMatchTimerVisibility();
	// Removes the match timer widget
	void RemoveMatchTimer();
	// Whether the match state should currently be drawn
	bool ShouldShowMatchState() const;
	// Removes the match state widget
	void RemoveMatchState();
	// Removes the chat widget from screen immediately
	void RemoveChatWidget();
	// Whether team scores should be displayed
	bool ShouldShowTeams() const;
	// Updates the teams displayed
	void UpdateTeams();

	// Callbacks for the player controller
	void OnSpectatorPawnChanged(ASpectatorPawn* NewSpectatorPawn);
	void SetSpectatorPawn(ASpectatorPawn* NewSpectatorPawn);
	void OnIsInFreeCamChanged(bool bNewIsInFreeCam);
	void OnFreeCamModeChanged(EFreeCamMode NewFreeCamMode, bool bNewFixed);
	void OnViewedCharacterChanged(ABaseCharacter* NewCharacter);
	void SetViewedCharacter(ABaseCharacter* NewCharacter);
	void OnVehicleSeatChanged(USeatBrick* NewSeat);
	void SetVehicleSeat(USeatBrick* NewSeat);
	void OnVehicleDownloadProgressChanged(ABrickVehicleDownloadReplicator* InDownloadReplicator, const TOptional<float>& Progress);
	void OnVehicleConstructionProgressChanged(ABrickVehicle* InVehicle, const TOptional<float>& Progress);
	void OnReceivedChatMessages(const TArray<FBrickChatMessage>& ChatMessages);
	void OnBrickEditorChanged(ABrickEditor* NewEditor);
	void SetBrickEditor(ABrickEditor* NewEditor);
	void OnViewTargetSubobjectChanged(UObject* NewViewTarget);
	void SetViewTargetSubobject(UObject* NewViewTarget);
	void OnProjectileCameraEnabledChanged(bool bNewEnabled);
	// Callback for the player state
	void OnTeamChanged(const FGenericTeamId& NewTeam);
	void OnMoneyChanged(float NewMoney, float AmountAdded);
	// Callback for the seat
	void OnSeatViewTargetChanged();
	// Callback for the editor
	void OnEditorSaved(bool bSuccess, bool bIsAutoSave);
	// Callbacks for the view target
	void OnCameraSpeedRatioChanged(float NewSpeedRatio);
	void OnCameraZoomRatioChanged(float NewZoomRatio);
	// Callback for the world settings
	void OnSlomoChanged();
	// Callback for the game instance
	void OnUGCThumbnailCaptured(const FString& ThumbnailPath);
	// Callbacks for the user settings
	UFUNCTION()
	void OnHUDVisibilityChanged(EHUDVisibility NewVisibility);
	UFUNCTION()
	void OnCameraModeChanged(ECameraMode NewMode);
	// Callbacks for the game state
	void OnMatchStateChanged(const FName& NewState);
	void OnMatchTimerChanged(int32 NewTime);
};
