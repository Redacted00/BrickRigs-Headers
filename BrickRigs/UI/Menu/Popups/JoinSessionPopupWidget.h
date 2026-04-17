// Copyright Fluppisoft, 2020

#pragma once

#include "Game/BrickGameInstance.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "JoinSessionPopupWidget.generated.h"

class UBrickTextBoxWidget;

UCLASS()
class BRICKRIGS_API UJoinSessionPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Properties
	FOnlineSessionSearchResult SearchResult;
	// ~Properties
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UJoinSessionPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached flag whether the server requires a password
	bool bPasswordRequired;
	// Cached session join state
	EJoinSessionState JoinState;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBoxWidget* PasswordTextBox;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = JoinSessionPopup)
	FText JoiningSessionText;
	UPROPERTY(EditDefaultsOnly, Category = JoinSessionPopup)
	FText CancellingJoinText;
	// ~Properties

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool CanPopupBeClosed_Implementation() const override;
	virtual bool ConfirmPopup_Implementation() override;
	virtual bool CancelPopup_Implementation() override;
	virtual FText GetPopupTitle_Implementation() const override;
	// ~Super Interface

	// Join the currently pending session
	UFUNCTION(BlueprintCallable)
	void JoinSession();
	// Cancel the pending join
	UFUNCTION(BlueprintCallable)
	void CancelJoinSession();

protected:
	// ~Blueprint Interface
	// Updates the buttons, throbber etc.
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateJoinState(EJoinSessionState InJoinState, bool bInPasswordRequired, bool bInHasPassword);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with the correct parameters
	void UpdateJoinState();
	// Callback for the game instance
	void OnJoinSessionStateChanged(EJoinSessionState NewState);
	// Internal version that is called by the delegate and initially
	void SetJoinSessionState(EJoinSessionState NewState);
	// Callbacks for the text box
	UFUNCTION()
	void OnPasswordChanged(const FText& NewText, EValueChangedEventType EventType);
};
