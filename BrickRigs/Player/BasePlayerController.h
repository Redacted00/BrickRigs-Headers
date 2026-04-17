// Copyright Fluppisoft, 2019

#pragma once

#include "UI/UGC/UGCBrowserWidget.h"
#include "PlayerControllerStaticInfo.h"
#include "UI/Input/BrickInputMode.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * This should be used as the base class for all player controllers, for shared functionality such as input
 */
UCLASS(Abstract)
class BRICKRIGS_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	// ~Variables
	FPopupHandle PopupHandle_JoinSession;
	FPopupHandle PopupHandle_CheatMenu;
	// ~Variables

	// ~Properties
	// Shared player controller properties class
	UPROPERTY(EditDefaultsOnly, NoClear, Category = Defaults)
	TSubclassOf<UPlayerControllerStaticInfo> StaticInfoClass;
	// ~Properties

public:
	// ~Constructor
	ABasePlayerController();

	// ~Super Interface
	virtual void Destroyed() override;
	virtual void ReceivedPlayer() override;
	virtual void SpawnPlayerCameraManager() override;
	virtual void UpdateStateInputComponents() override;
	virtual void UpdateRotation(float DeltaTime) override;
	virtual void ClientSetHUD_Implementation(TSubclassOf<AHUD> NewHUDClass) override;
	virtual void SetName(const FString& S) override;
	virtual void ServerChangeName_Implementation(const FString& S) override;
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
	FUniqueNetIdRepl GetPlayerId() const;
	// ~Player

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
};
