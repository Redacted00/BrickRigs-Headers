// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputHelpWidget.generated.h"

class UGameOverlayWidget;
class UInputActionListWidget;

/**
 * Displays a list of currently active input actions and axes
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputHelpWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Owning game overlay
	TWeakObjectPtr<UGameOverlayWidget> GameOverlay;
	// Whether the panel currently has user focus
	uint8 bIsFocused : 1;
	// Whether the input help panel is expanded
	uint8 bIsOpen : 1;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInputActionListWidget* ActionListWidget;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	// ~Super Interface

	void SetGameOverlay(UGameOverlayWidget* Widget);

	// Returns the owning game overlay
	UFUNCTION(BlueprintPure)
	UGameOverlayWidget* GetGameOverlay() const
	{
		return GameOverlay.Get();
	}

	bool IsFocused() const
	{
		return bIsFocused;
	}

	// Updates the opened state
	bool SetIsOpen(bool bNewOpen);

	bool IsOpen() const
	{
		return bIsOpen;
	}

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

private:
	// Updates the focused state
	void SetIsFocused(bool bNewFocused, bool bIsDestructing = false);
	// Updates the input action list mode
	void UpdateActionListMode() const;
};
