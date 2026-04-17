// Copyright Fluppisoft, 2020

#pragma once

#include "ContextMenuWidget.h"
#include "UI/Menu/Popups/PopupContainerWidget.h"
#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "WindowManagerWidget.generated.h"

class UGameOverlayWidget;
class UMainWidgetBase;
class UIntroSequenceWidget;
class UMenuWidget;
class UHUDContainerWidget;
class UCanvasPanel;
class UTooltipWidget;
class UBrickComboBoxWidget;
class UBrickComboBoxMenuWidget;
class UContextMenuWidget;
struct FBrickInputMode;

UENUM()
enum class EMainWidgetType : uint8
{
	None,
	IntroSequence,
	Menu,
	HUD,
	Max
};

/**
 * This widget is always added to the screen and used as the root for the menu, HUD, popups etc.
 */
UCLASS(Abstract)
class BRICKRIGS_API UWindowManagerWidget : public UBrickUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// Handle used to load the active widget class
	FFluAsyncAssetLoader AssetLoader_ActiveWidgetClass;
	// Currently active widget
	UPROPERTY(Transient)
	UMainWidgetBase* ActiveWidget;
	// Currently active widget type
	EMainWidgetType ActiveWidgetType;
	// Current HUD widget class
	TSoftClassPtr<UUserWidget> HUDWidgetClass;

	// List of all displayed popup containers
	UPROPERTY(Transient)
	TArray<UPopupContainerWidget*> PopupContainerWidgets;
	// The last popup handle assigned
	FPopupHandle LastPopupHandle;

	// Currently open menu anchor
	UPROPERTY(Transient)
	UMenuAnchorWidget* MenuAnchorWidget;
	// Path that was focused before the menu anchor was opened
	FWeakWidgetPath PreMenuAnchorFocusedPath;

	// Widget currently displayed as the tooltip
	UPROPERTY(Transient)
	UTooltipWidget* CurrentTooltipWidget;
	// Widget owning the current tooltip
	TWeakObjectPtr<UBrickUserWidget> TooltipOwner;
	// Last time the active tooltip owner has changed
	double TooltipOwnerChangeTime;
	// Last known mouse position, needed because it's moved while captured
	FVector2D LastMousePosition;
	// Whether the menu should currently be open
	uint8 bIsMenuOpen : 1;
	// Cause of the last focus event
	EFocusCause LastFocusCause;
	// Whether the menu anchor is currently being opened/closed
	uint8 bIsOpeningMenuAnchor : 1;
	uint8 bIsClosingMenuAnchor : 1;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCanvasPanel* MainCanvasPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSoftClassPtr<UMainWidgetBase> IntroSequenceWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSoftClassPtr<UMenuWidget> MenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSoftClassPtr<UHUDContainerWidget> HUDContainerWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSubclassOf<UPopupContainerWidget> PopupContainerClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSubclassOf<UContextMenuWidget> ContextMenuWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	TSubclassOf<UTooltipWidget> TooltipWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	float TooltipOffset;
	// How long to wait until the tooltip is displayed
	UPROPERTY(EditDefaultsOnly, Category = WindowManager)
	float TooltipDelay;
	// ~Properties

public:
	// ~Constructor
	UWindowManagerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;
	// ~Super Interface

	// ~Windows Manager
	// Returns the window manager widget instance from the game instance
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Window Manager"))
	static UWindowManagerWidget* Get(const UObject* WorldContextObject);
	// Lets the player update the current input mode
	void UpdateInputMode();
	// Gets the desired input mode for the currently visible widget or popup
	bool GetInputMode(FBrickInputMode& OutInputMode) const;
	// ~Windows Manager

	// ~Pages
	// Get the HUD container widget instance
	UHUDContainerWidget* GetHUDContainer() const;
	// Gets the game overlay from the HUD container
	UGameOverlayWidget* GetGameOverlayWidget() const;
	// Changes the current HUD widget class
	void SetHUDWidgetClass(const TSoftClassPtr<UUserWidget>& InWidgetClass);
	// Get the current HUD widget class
	const TSoftClassPtr<UUserWidget>& GetHUDWidgetClass() const
	{
		return HUDWidgetClass;
	}

	// Called from the intro sequence widget once it is finished
	UFUNCTION(BlueprintCallable)
	void OnIntroSequenceFinished();
	// Called from a popup or page widget when it has faded out
	void OnMainWidgetFadedOut(UMainWidgetBase* InWidget);
	// Updates the active widget if needed
	void UpdateActiveWidget();

protected:
	// Shows a new widget and fades out the other ones
	void SetActiveWidget(EMainWidgetType InWidgetType, const TSoftClassPtr<UMainWidgetBase>& InWidgetClass, bool bInstantFade = false);
	// ~Pages

public:
	// ~Menu
	// Indicates if the menu is currently opened
	UFUNCTION(BlueprintPure)
	bool IsMenuOpen() const
	{
		return bIsMenuOpen;
	}

	// Opens or closes the menu
	UFUNCTION(BlueprintCallable)
	void SetMenuOpen(bool bOpen);
	// Whether the menu can currently be opened or closed
	UFUNCTION(BlueprintPure)
	bool CanOpenOrCloseMenu(bool bOpen) const;
	// ~Menu

	// ~Popups
	bool IsPopupOpen(const FPopupHandle& Handle) const
	{
		return GetPopupFromHandle(Handle) != nullptr;
	}

	// Returns the number of currently open popups
	auto GetNumPopups() const
	{
		return PopupContainerWidgets.Num();
	}

	// Adds a new popup to the screen
	bool OpenPopup(FPopupHandle& Handle, UPopupParams* PopupParams, bool bToggleOpen);
	// Closes the popup created with the given handle, if any exists
	bool ClosePopup(FPopupHandle& Handle, EPopupResult Result = EPopupResult::Cancel, bool bImmediate = false);
	// Closes a specific popup container
	bool ClosePopup(UPopupContainerWidget* Container, EPopupResult Result = EPopupResult::Cancel, bool bImmediate = false, bool bIsBeingDestroyed = false);
	// Closes all open popups, returns the number of popups removed
	int32 CloseAllPopups(bool bIsBeingDestroyed = false);

private:
	// Updates the slot Z order of a popup
	void UpdatePopupZOrder(UPopupContainerWidget* Container, int32 PopupIndex);
	// Try to find an the popup with the given handle
	UPopupContainerWidget* GetPopupFromHandle(const FPopupHandle& Handle) const;
	// Uninitializes and removes a popup container
	void RemovePopupContainer(UPopupContainerWidget* Container);
	// ~Popups

public:
	// ~Menu Anchors
	// Opens up the combo box menu for the given combo box
	bool OpenComboBoxList(UBrickComboBoxWidget* ComboBox);
	// Opens the context menu at the current mouse position
	bool OpenContextMenu(const FContextMenuParams& Params);
	// Removes the given menu anchor from screen
	bool CloseMenuAnchor(UMenuAnchorWidget* Widget, bool bLostFocus = false);

private:
	// Removes the current menu anchor from screen
	bool CloseMenuAnchor(bool bLostFocus = false);
	// Adds a menu anchor to the screen
	bool AddMenuAnchor(UMenuAnchorWidget* Widget);
	// ~Menu Anchors

	// ~Tooltips
	void ShowTooltip(UBrickUserWidget* UserWidget, const TSharedRef<SWidget>& SlateWidget, const FTooltipContent& Content);
	void RemoveTooltip();
	// Version that doesn't reset the tooltip owner
	void RemoveTooltipInternal();
	// ~Tooltips

public:
	// ~Focus
	// Called from the viewport client to update the focus
	void UpdateFocus(const TSharedRef<SWidget>& InWidgetToFocus, bool bFocusDescendant) const;

private:
	// Returns the widget index upon which the path is valid
	static int32 GetWidgetPathValidityIndex(const FWeakWidgetPath& WidgetPath);
	// Returns whether the given path is still valid
	static bool IsValidWidgetPath(const FWeakWidgetPath& WidgetPath);
	// ~Focus

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddActiveWidget(UMainWidgetBase* Widget);
	// ~Blueprint Interface

private:
	// Used to handle the user focus during mouse events
	FReply HandleMouseDownOrMouseMove(const FPointerEvent& InMouseEvent, bool bIsMove);
};
