// Copyright Fluppisoft, 2018

#pragma once

#include "UI/Menu/Popups/PopupParams.h"
#include "CoreMinimal.h"
#include "Misc/FluAsyncAssetLoader.h"
#include "UI/Widgets/MainWidgetBase.h"
#include "MenuWidget.generated.h"

class UMenuPageWidget;
class UPopupWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuWidget : public UMainWidgetBase
{
	GENERATED_BODY()

	// ~Variables
	// Used to load the requested menu page
	FFluAsyncAssetLoader AssetLoader_MenuPage;
	FPopupHandle PopupHandle_NetworkError;
	// The menu context that is currently open
	FName MenuContext;
	// The currently open menu page
	UPROPERTY(Transient)
	UMenuPageWidget* CurrentMenuPage;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Menu)
	TSoftClassPtr<UPopupWidget> JoinSessionPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = Menu)
	TSoftClassPtr<UPopupWidget> NetworkErrorPopupClass;
	// ~Properties

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	virtual UWidget* GetWidgetToFocus_Implementation() const override;
	// ~Super Interface

	// ~Menu
	// Whether the menu is opened as the main menu
	UFUNCTION(BlueprintPure)
	bool IsMainMenu() const;
	// Go back in the menu hierarchy
	UFUNCTION(BlueprintCallable)
	bool StepBack();
	// Closes the in game menu
	UFUNCTION(BlueprintCallable)
	void CloseMenu();
	// Open the given menu context
	UFUNCTION(BlueprintCallable)
	void OpenMenu(FName InMenu);
	// Display a specific menu page widget, this should only be called upon OnMenuContextChanged
	UFUNCTION(BlueprintCallable)
	void ShowMenuPage(TSoftClassPtr<UMenuPageWidget> WidgetClass, FText TitleText);

private:
	// Helper function to split a menu context at the last dot, if there is none left is empty and right is the entire string
	void SplitMenuContext(const FName& Context, FString& OutLeft, FString& OutRight) const;
	// Return the menu context that should be opened by default
	FName GetRootMenuContext() const;
	// Helper function to exchange the menu page
	void SetCurrentMenuPage(UMenuPageWidget* NewPage);
	// ~Menu

public:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayMenuTransition();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void CreateBackButton();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnClickedQuickGame();
	// Called after changing the menu, the blueprint should switch on the context and change the menu displayed
	UFUNCTION(BlueprintImplementableEvent)
	void OnMenuContextChanged(const FName& NewContext);
	// Set the text to display as the title
	UFUNCTION(BlueprintImplementableEvent)
	void SetTitleText(const FText& InText);
	UFUNCTION(BlueprintImplementableEvent)
	void AddMenuPage(UMenuPageWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void RemoveMenuPage(UMenuPageWidget* Widget);
	// ~Blueprint Interface

protected:
	// Callback for the back button
	UFUNCTION(BlueprintCallable)
	void OnClickedBack();

private:
	// Callback for the game instance
	void OnNetworkError();
};
