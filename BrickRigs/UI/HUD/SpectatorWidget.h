// Copyright Fluppisoft, 2021

#pragma once

#include "UI/HUD/HUDWidgetInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpectatorWidget.generated.h"

class ABaseCharacter;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API USpectatorWidget : public UUserWidget, public IHUDWidgetInterface
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Used to switch to the next or previous character
	UFUNCTION(BlueprintCallable)
	void SpectateNextCharacter(bool bNext);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetWidgetToFocus() const;
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSpectatedPlayer(const FText& PlayerName, bool bIsValidPlayer);
	// ~Blueprint Interface

private:
	// Callback for the player controller
	void OnSpectatedCharacterChanged(ABaseCharacter* NewCharacter);

	// ~IHUDWidgetInterface
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	// ~IHUDWidgetInterface
};
