// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchStateWidget.generated.h"

class ABrickPlayerState;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UMatchStateWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The currently displayed match state
	FName MatchState;
	// ~Variables

public:
	// ~Delegates
	FSimpleDelegate OnFadedOutDelegate;
	// ~Delegates

	// ~Super Interface
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Called by the parent widget
	void OnMatchStateChanged(const FName& NewState);

protected:
	// To be called when the animation has finished
	UFUNCTION(BlueprintCallable)
	void OnFadedOut();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMatchState(const FText& NewDisplayText, EBrickUIColorStyle NewColorStyle, bool bFadeOut);
	// ~Blueprint Interface

private:
	// Updates the match state
	void SetMatchState(const FName& NewState);
	// Calls the blueprint version with all parameters
	void UpdateMatchState();
	// Callback for the game state
	void OnPlayerAddedOrRemoved(ABrickPlayerState* PlayerState, bool bAdded);
};
