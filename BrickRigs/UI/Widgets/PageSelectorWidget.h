// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickSliderWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PageSelectorWidget.generated.h"

/**
 * Used to display and switch pages
 */
UCLASS(Abstract)
class BRICKRIGS_API UPageSelectorWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPageChanged, int32, Page);

	// ~Variables
	// The currently selected page
	int32 CurrentPage;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Pages, meta = (UIMin = 0))
	int32 NumPages;
	// ~Properties

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickSliderWidget* Slider;
	// ~Widgets

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnPageChanged OnPageChangedDelegate;
	// ~Delegates

	// ~Constructor
	UPageSelectorWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Used to initialize the page selector
	UFUNCTION(BlueprintCallable)
	void InitializePages(int32 InNumPages, int32 InCurrentPage, int32 InNumResults);
	// Sets the currently selected page
	UFUNCTION(BlueprintCallable)
	void SetCurrentPage(int32 NewPage);
	// Get the currently selected page
	UFUNCTION(BlueprintPure)
	int32 GetCurrentPage() const;
	// Selects the next, previous, first or last page
	UFUNCTION(BlueprintCallable)
	void GotoNextPage(bool bForward, bool bSkipToEnd);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtons(bool bCanGoBack, bool bCanGoForward);
	// ~Blueprint Interface

private:
	// Updates the current page, buttons etc.
	void UpdateCurrentPage();
	// Selects a new page
	void SetCurrentPageInternal(int32 NewPage, bool bCallNotify);
	// Callbacks for the slider
	UFUNCTION()
	void OnSliderValueChanged(float NewValue, EValueChangedEventType EventType);
};
