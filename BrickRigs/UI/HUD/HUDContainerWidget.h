// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Misc/FluAsyncAssetLoader.h"
#include "UI/Widgets/MainWidgetBase.h"
#include "HUDContainerWidget.generated.h"

class UGameOverlayWidget;

/**
 * This widget is handled by the window manager and contains the HUD widget as well as the game overlay
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDContainerWidget : public UMainWidgetBase
{
	GENERATED_BODY()

	// ~Variables
	// Used to load the HUD widget class
	FFluAsyncAssetLoader AssetLoader_HUDWidgetClass;
	// Game overlay widget instance
	UPROPERTY(Transient)
	UGameOverlayWidget* GameOverlayWidget;
	// Currently active HUD widget
	UPROPERTY(Transient)
	UUserWidget* ActiveHUDWidget;
	// Whether the game overlay widget is currently added attached to the HUD active widget or to us
	uint8 bIsGameOverlayAddedByThis : 1;
	uint8 bIsGameOverlayAddedByHUD : 1;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UGameOverlayWidget> GameOverlayWidgetClass;
	// ~Properties

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	// ~Super Interface

	// Called from the window manager
	void OnHUDWidgetClassChanged(const TSoftClassPtr<UUserWidget>& NewHUDWidgetClass);

	auto* GetGameOverlayWidget() const
	{
		return GameOverlayWidget;
	}

private:
	// Sets the currently active HUD widget
	void SetActiveHUDWidget(UUserWidget* NewWidget, bool bIsDestructing);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddGameOverlayWidget(UGameOverlayWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddHUDWidget(UUserWidget* Widget);
	// ~Blueprint Interface
};
