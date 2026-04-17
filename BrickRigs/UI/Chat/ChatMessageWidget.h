// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Player/BrickChatMessage.h"
#include "Blueprint/UserWidget.h"
#include "ChatMessageWidget.generated.h"

class UChatWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UChatMessageWidget : public UUserWidget
{
	GENERATED_BODY()

	friend class UChatMessageInputComponent;

	// ~Variables
	// Outer chat widget
	TWeakObjectPtr<UChatWidget> ChatWidget;
	// Player associated to this message who can be kicked
	FChatMessagePlayerInfo PlayerToKick;
	// ~Variables

public:
	// Sets the owning chat widget
	void SetChatWidget(UChatWidget* InWidget);

	// Initializes the displayed message
	void InitializeChatMessage(const FBrickChatMessage& InMessage);

	// Whether the associated player can be kicked
	UFUNCTION(BlueprintPure)
	bool CanKickPlayer() const;
	// Opens a popup to kick the associated player
	UFUNCTION(BlueprintCallable)
	void KickPlayer();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanKickPlayer(bool bNewCanKick);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMessageText(const FText& NewText);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle NewColorStyle);
	// ~Blueprint Interface
};
