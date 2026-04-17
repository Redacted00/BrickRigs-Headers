// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Player/BrickChatMessage.h"
#include "Blueprint/UserWidget.h"
#include "Input/BaseInputComponent.h"
#include "ChatMessageWidget.generated.h"

class UChatWidget;

// Input component used for the context menu
UCLASS()
class BRICKRIGS_API UChatMessageInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

	// ~Variables
	FBrickChatMessage ChatMessage;
	// ~Variables

public:
	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetContextMenuOwner(UObject* InOwner) override;
	// ~Super Interface

private:
	UFUNCTION()
	void OnPressedKickPlayer();
	UFUNCTION()
	bool GetKickPlayerEnabled() const;
	UFUNCTION()
	void OnPressedViewVehicle();
};

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
	// Displayed message info
	FBrickChatMessage ChatMessage;
	// ~Variables

public:
	// Sets the owning chat widget
	void SetChatWidget(UChatWidget* InWidget);

	// Initializes the displayed message
	void InitializeChatMessage(const FBrickChatMessage& InMessage);

	const auto& GetChatMessage() const
	{
		return ChatMessage;
	}

	// Opens the chat message context menu
	UFUNCTION(BlueprintCallable)
	void OpenContextMenu();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMessageText(const FText& NewText);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle NewColorStyle);
	// ~Blueprint Interface
};
