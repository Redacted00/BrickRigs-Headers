// Copyright Fluppisoft, 2018

#pragma once

#include "PopupParams.h"
#include "PopupContainerWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Input/HoldKeyTimer.h"
#include "PopupWidget.generated.h"

class UPopupContainerWidget;
class UMenuButtonPanelWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPopupWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Weak reference to the container that holds this popup
	TWeakObjectPtr<UPopupContainerWidget> OwningContainer;
	FHoldKeyTimer HoldKeyTimer_ClosePopup;
	// ~Variables

	// ~Properties
	// The text to display as the title
	UPROPERTY(EditDefaultsOnly, Category = Popup)
	FText TitleText;
	// The default desired popup size
	UPROPERTY(EditDefaultsOnly, Category = Popup)
	EPopupSizeRule PopupSizeRule;
	// Optional action that is used to open and close the popup
	UPROPERTY(EditDefaultsOnly, Category = Popup)
	FName CloseActionName;
	// ~Properties

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	// Initializes the popup
	void InitializePopup(UPopupContainerWidget* InContainer);
	// Unbinds delegates etc.
	void UninitializePopup(UPopupContainerWidget* InContainer);
	// Accessor to the container widget
	UFUNCTION(BlueprintPure)
	UPopupContainerWidget* GetPopupContainer() const
	{
		return OwningContainer.Get();
	}

	// Get the button panel from the popup conttainer
	UFUNCTION(BlueprintPure)
	UMenuButtonPanelWidget* GetButtonPanel() const;
	// Access the popup parameters
	template <class T>
	T* GetPopupParams() const
	{
		if (OwningContainer.IsValid())
		{
			return OwningContainer->GetPopupParams<T>();
		}

		return nullptr;
	}
	UFUNCTION(BlueprintPure, Meta = (DeterminesOutputType = "Class"))
	UPopupParams* GetPopupParams(TSubclassOf<UPopupParams> Class);

	// Updates the title displayed
	UFUNCTION(BlueprintCallable)
	void UpdatePopupTitle();
	// Called this to close the popup
	UFUNCTION(BlueprintCallable)
	void ClosePopup(EPopupResult Result = EPopupResult::Cancel);

	// ~Blueprint Interface
	// Get the sub widget that should receive focus by default
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus();
	// Whether this widget should be displayed inside the property container
	UFUNCTION(BlueprintNativeEvent)
	bool ShouldBeVisibleInPopupContainer() const;
	// Returns the desired popup size
	UFUNCTION(BlueprintNativeEvent)
	EPopupSizeRule GetPopupSizeRule() const;
	// Whether this popup can currently be closed
	UFUNCTION(BlueprintNativeEvent)
	bool CanPopupBeClosed() const;
	// Called upon pressing escape for example, gives the popup a chance to consume the event
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool StepBack();
	// Gives the popup a chance to intervene the confirm and cancel events
	UFUNCTION(BlueprintNativeEvent)
	bool ConfirmPopup();
	UFUNCTION(BlueprintNativeEvent)
	bool CancelPopup();
	// Gets the desired title text for the popup
	UFUNCTION(BlueprintNativeEvent)
	FText GetPopupTitle() const;
	// ~Blueprint Interface

protected:
	// Called on key and mouse button down and up events
	FReply HandleKeyEvent(const FKey& Key, bool bKeyDown, bool bIsRepeat, const FKeyEvent* KeyEvent = nullptr);
};
