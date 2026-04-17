// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Misc/BrickUITimer.h"
#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Player/BrickChatMessage.h"
#include "UI/Menu/Popups/PopupParams.h"
#include "ChatWidget.generated.h"

class UGameOverlayWidget;
class UChatMessageWidget;
class UBrickScrollBox;
class UControlHintWidget;
class UBrickTextBoxWidget;

/**
 * Container for the chat messages and chat
 */
UCLASS(Abstract)
class BRICKRIGS_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// Used to hide the chat after a few seconds of inactivity
	FBrickUITimer Timer_CloseChat;
	// The parent game overlay
	TWeakObjectPtr<UGameOverlayWidget> GameOverlay;
	// List of all message widgets
	UPROPERTY(Transient)
	TArray<UChatMessageWidget*> ChatMessageWidgets;
	// Whether the chat currently has focus
	uint8 bIsFocused : 1;
	// Whether the scroll box is currently visible
	uint8 bIsScrollBoxVisible : 1;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickScrollBox* ChatScrollBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UControlHintWidget* ChatContextControlHint;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBoxWidget* TextBox;
	// ~Widgets

	// ~Properties
	// How long to display the chat in the 'when active' mode
	UPROPERTY(EditDefaultsOnly, Category = Chat)
	float FadeOutDelay;
	UPROPERTY(EditDefaultsOnly, Category = Chat)
	TSubclassOf<UChatMessageWidget> ChatMessageWidgetClass;
	// ~Properties

public:
	// ~Constructor
	UChatWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	// Sets the parent game overlay
	void SetGameOverlay(UGameOverlayWidget* InGameOverlay);
	// Shows and sets focus to the chat
	void OpenChat(bool bCloseAutomatically);
	// Starts the fade out timer or closes the chat immediately
	void CloseChat(bool bCloseImmediately);

	bool IsFocused() const
	{
		return bIsFocused;
	}

private:
	// Updates the focused flag
	void SetIsFocused(bool bNewFocused, bool bIsDestructing = false);
	// Start the timer for fading out
	void SetupCloseChatTimer(bool bResetTimer);
	// Stops the fade out timer
	void ClearCloseChatTimer();
	// Called by the close delay timer
	void OnCloseChatTimer();

	// Scrolls the latest chat message into view
	void ScrollToLastChatMessage();
	// Adds a new widget for the chat messages
	void AddChatMessages(const TArray<FBrickChatMessage>& ChatMessages);

	// Cycle to the next valid context
	void CycleChatContext();
	// Makes sure the current context may be selected and changes it if needed
	bool VerifyChatContext();

	// Updates the action displayed on the control hint
	void UpdateControlHint() const;
	// Shows or hides the scroll box as needed<
	void UpdateScrollBoxVisibility();

public:
	// Called from the game overlay when new messages have been received
	// NOTE: This is not bound to the player delegate directly so we can avoid receiving it right after construction
	void OnReceivedChatMessages(const TArray<FBrickChatMessage>& ChatMessages);

private:
	// Callbacks for the player
	void OnIsAdminChanged(bool bNewIsAdmin);
	void SetIsAdmin(bool bNewIsAdmin);
	// Callbacks for the user settings
	void OnChatContextChanged(EChatContext NewContext);
	void SetChatContext(EChatContext NewContext);
	// Callback for the text box
	UFUNCTION()
	void OnTextChanged(const FText& Text, EValueChangedEventType EventType);

public:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus();

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsTyping(bool bNewTyping);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsFocused(bool bNewFocused);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateScrollBoxVisibility(bool bNewVisible);
	// ~Blueprint Interface
};
