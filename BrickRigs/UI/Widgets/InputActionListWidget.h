// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Menu/Popups/BindKeyPopupWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputActionListWidget.generated.h"

class UInputActionCategoryWidget;
class UInputActionWidget;
class UGridPanel;
class UBaseInputComponent;
class UInputCategory;

UENUM()
enum class EInputActionListMode : uint8
{
	InputHelpClosed,
	InputHelpOpen,
	ContextMenu,
	EditorTopBar
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputActionListWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActionTriggered, const FName&, ActionName, bool, bReleased);

	// ~Variables
	FPopupHandle PopupHandle_BindKey;
	UPROPERTY(Transient)
	TArray<UInputActionCategoryWidget*> CategoryWidgets;
	UPROPERTY(Transient)
	TArray<UInputActionWidget*> ActionWidgets;
	// Current action displayed under quick access
	FName QuickAccessActionName;
	// Whether the quick access action has a secondary action
	bool bQuickAccessActionHasSecondary;
	// Time passed since the quick access action was selected
	float QuickAccessTime;
	// Maps the input components to their last known version
	TMap<TWeakObjectPtr<UBaseInputComponent>, int32> InputComponentStack;
	// Optional override component to display actions for
	TWeakObjectPtr<UBaseInputComponent> InputComponentOverride;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* MainGridPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditAnywhere, Category = Visuals)
	TSubclassOf<UInputActionCategoryWidget> CategoryWidgetClass;
	UPROPERTY(EditAnywhere, Category = Visuals)
	TSubclassOf<UInputActionWidget> ActionWidgetClass;
	// Popup to open when rebinding keys
	UPROPERTY(EditDefaultsOnly, Category = Visuals)
	TSoftClassPtr<UBindKeyPopupWidget> BindKeyPopupClass;

	// This is the currently opened input category(-ies)
	UPROPERTY(EditAnywhere, Category = InputActions)
	TArray<UInputCategory*> InputCategories;
	// Specifies the behavior of the list
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputActions)
	EInputActionListMode InputActionListMode;
	// How long quick access actions are displayed before being swapped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = InputActions)
	float QuickAccessDisplayTime = 4.f;
	// ~Properties

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnActionTriggered OnActionTriggeredDelegate;
	// ~Delegates

	// ~Constructor
	UInputActionListWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	// ~Super Interface

	// Changes the action list mode
	void SetInputActionListMode(EInputActionListMode NewMode);
	// Called by the category widgets
	void OpenInputCategory(UInputCategory* InCategory);
	// Sets the optional input component override
	void SetInputComponentOverride(UBaseInputComponent* InIC, UInputCategory* InCategory);

	// Opens the key rebinding popup
	void RebindAction(const FInputActionAndAxisMapping& InputMapping, bool bIsNegativeAxis);

private:
	// Callbacks for the bind key popup
	void OnBindKey(const FInputActionAndAxisMapping& PrevMapping, const FInputActionAndAxisMapping& NewMapping, bool bInIsAction);

public:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void FadeIn();
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInputActionListMode(EInputActionListMode NewMode);
	UFUNCTION(BlueprintNativeEvent)
	void AddCategoryWidget(UInputActionCategoryWidget* Widget, int32 CategoryIndex);
	UFUNCTION(BlueprintNativeEvent)
	void AddActionWidget(UInputActionWidget* Widget, int32 ActionIndex);
	// ~Blueprint Interface

private:
	// Checks if the input context has changed and updates the widget if needed
	bool UpdateInputContext(bool bIsInitializing = false, bool bAdvanceQuickAccess = false);
	// Updates the displayed actions or categories
	void UpdateActions(bool bFadeIn, bool bAdvanceQuickAccess = false);
};
