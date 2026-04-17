// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/FluTimer.h"
#include "MenuAnchorWidget.generated.h"

/**
 * Widgets derived from this class can be overlayed on the screen, for example for context or combo box menus
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuAnchorWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	FFluUITimer Timer_RemoveMenu;
	// ~Variables

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// ~Super Interface

	// Closes the menu anchor
	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface
};
