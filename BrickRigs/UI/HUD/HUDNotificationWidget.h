// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "UI/Misc/BrickUITimer.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDNotificationWidget.generated.h"

class UGameOverlayWidget;
class UBrickTextBlock;
class UBrickImage;

/**
 * Represents a simple notification that is displayed on the HUD, like the current slomo speed
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	const static FName ThumbnailNotificationName;

private:
	// ~Variables
	FBrickUITimer Timer_FadeOut;
	// Reference to the owning game overlay
	TWeakObjectPtr<UGameOverlayWidget> GameOverlay;
	// Whether the notification is currently fading in or has already faded in
	uint8 bIsFadingIn : 1;
	// Whether the notification is playing the fade out animation
	uint8 bIsFadingOut : 1;
	// Conext name associated with this notification
	FName NotificationContext;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* TextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* ThumbnailImage;
	// ~Widgets

	// ~Properties
	// Amount of time the notification is displayed before fading away
	UPROPERTY(EditDefaultsOnly, Category = Notification)
	float NotificationDisplayTime;
	// ~Properties

public:
	// ~Constructor
	UHUDNotificationWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	// ~Super Interface

	// Sets the owning widget
	void SetGameOverlay(UGameOverlayWidget* InWidget);
	// Setup the context
	void SetNotificationContext(const FName& InContext);
	// Public accessor to the associated context
	const FName& GetNotificationContext() const
	{
		return NotificationContext;
	}

	// Whether the notification displayes a new thumbnail
	bool IsThumbnailNotification() const
	{
		return NotificationContext == ThumbnailNotificationName;
	}

	// Used for thumbnail notifications to display the captured thumbnail
	void SetCapturedThumbnail(const FString& ThumbnailPath);
	// Fades the notification in initially and sets up the fade out timer
	void StartNotification(bool bShouldFadeOut);
	// Fades out the notification
	void EndNotification();

protected:
	// To be called when the notification has faded out entirely
	UFUNCTION(BlueprintCallable)
	void OnFadedOut();

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeNotification(FBrickUIIconSlot InIconSlot, const FText& InText, EBrickUIColorStyle InColorStyle);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsThumbnailNotification(bool bNewIsThumbnail);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeOutAnim();
	// ~Blueprint Interface
};
