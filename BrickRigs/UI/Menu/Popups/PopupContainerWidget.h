// Copyright Fluppisoft, 2019

#pragma once

#include "PopupParams.h"
#include "CoreMinimal.h"
#include "Misc/BrickAssetManager.h"
#include "UI/Widgets/MainWidgetBase.h"
#include "PopupContainerWidget.generated.h"

class UMenuButtonPanelWidget;
class UPopupWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPopupContainerWidget : public UMainWidgetBase
{
	GENERATED_BODY()

	// ~Variables
	// Handle used to load the popup content class
	FSmartStreamableHandle StreamableHandle_PopupClass;
	// The handle of this popup
	FPopupHandle PopupHandle;
	// The cached popup params associated with this popup
	UPROPERTY(Transient)
	UPopupParams* PopupParams;
	// The contained popup
	UPROPERTY(Transient)
	UPopupWidget* PopupWidget;
	// Whether the button panel is currently visible
	bool bIsButtonPanelVisible;
	// ~Variables

public:
	// ~Constructor
	UPopupContainerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	virtual FNavigationReply NativeOnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent, const FNavigationReply& InDefaultReply) override;
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& KeyEvent) override;
	virtual UWidget* GetWidgetToFocus_Implementation() const override;
	// ~Super Interface

	// Initializes the popup container and content
	void InitializePopup(const FPopupHandle& InHandle, UPopupParams* InParams);
	// Uninitializes the popup
	void UninitializePopup();
	// Get the popup params casted to a specific type, returns null if it doesn't match or is invalid
	template <class T = UPopupParams>
	T* GetPopupParams() const
	{
		return Cast<T>(PopupParams);
	}

	// Returns the associated handle
	const FPopupHandle& GetPopupHandle() const
	{
		return PopupHandle;
	}

	// Get the class of the associated popup
	TSoftClassPtr<UPopupWidget> GetPopupClass() const;
	// Updates the displayed title
	void UpdateTitleText();
	// Updates the visibility and alignment of the popup
	void UpdatePopupSlot();
	// Whether the popup can currently be closed
	bool CanPopupBeClosed() const;
	// Called to close the popup
	void ClosePopup(EPopupResult Result);
	// Sets the color of the popup
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle InColorStyle);
	// Can be used to show or hide the button panel
	UFUNCTION(BlueprintCallable)
	void SetButtonPanelVisibility(bool bNewVisible);

protected:
	// Callbacks for the default confirm and cancel buttons
	UFUNCTION(BlueprintCallable)
	void ConfirmPopup();
	UFUNCTION(BlueprintCallable)
	void CancelPopup();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle InColorStyle);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTitleText(const FText& NewTitle);
	UFUNCTION(BlueprintImplementableEvent)
	void AddPopupWidget(UPopupWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonPanelVisibility(bool bNewVisible);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateContentSlot(bool bShowContent, EPopupSizeRule SizeRule);
	// Adds a standard confirm button
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UMenuButtonWidget* CreateConfirmButton();
	// Adds a standard cancel button
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UMenuButtonWidget* CreateCancelButton();
	// ~Blueprint Interface
};
