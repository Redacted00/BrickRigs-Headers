// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Input/BrickInputMode.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidgetBase.generated.h"

class UWindowManagerWidget;
class UMenuButtonPanelWidget;
class UFadingPanelWidget;

/**
 * This class is used as the base class for all widget managed by the window manager
 */
UCLASS(Abstract)
class BRICKRIGS_API UMainWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The owning window manager
	TWeakObjectPtr<UWindowManagerWidget> WindowManager;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UFadingPanelWidget* FadingPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UMenuButtonPanelWidget* ButtonPanel;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the owning window manager
	void SetWindowManager(UWindowManagerWidget* InWindowManager);
	// Public accessor to the window manager
	UFUNCTION(BlueprintPure)
	UWindowManagerWidget* GetWindowManager() const
	{
		return WindowManager.Get();
	}

	// Public accessor to the button panel
	UFUNCTION(BlueprintPure)
	UMenuButtonPanelWidget* GetButtonPanel() const
	{
		return ButtonPanel;
	}

	// Starts the fade in animation
	void FadeIn();
	// Starts the fade out animation
	void FadeOut(bool bImmediate = false);
	// Whether the widget is already faded in or still in the process
	bool IsFadingInOrFadedIn() const;
	// Whether the widget is currently fading out
	bool IsFadingOut() const;
	// Whether the widget is currently faded out
	bool IsFadedOut() const;
	// Get the desired input mode
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const;
	// Lets the window manager update the focus and input mode
	void UpdateInputMode();

	// ~Blueprint Interface
	// Get the widget that should receive focus by default
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

private:
	// Callback for the fading panel
	UFUNCTION()
	virtual void OnFadedOut();
};
