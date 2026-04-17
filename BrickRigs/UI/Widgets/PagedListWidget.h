// Copyright Fluppisoft, 2020

#pragma once

#include "PagedListEntryWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PagedListWidget.generated.h"

class UPageSelectorWidget;
class UGridPanel;
class UPropertiesPanelWidget;
class UPagedListHeaderWidget;
class UScrollBox;

/**
 * This widget is a template for specific kinds of lists with pages, e.g. for servers, kicks and UGC items
 */
UCLASS(Abstract)
class BRICKRIGS_API UPagedListWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE_RetVal_OneParam(const FPagedListEntry*, FGetLoadedEntry, int32);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadPage, int32, NewPage);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEntrySelected, UPagedListEntryWidget*, Widget, bool, bDoubleClick);

	// ~Variables
	// The outer object responsible for property reflection
	TWeakObjectPtr<> PropertyContainer;
	// List of all entry widgets
	UPROPERTY(Transient)
	TArray<UPagedListEntryWidget*> EntryWidgets;
	// Whether the entries are currently being loaded
	bool bIsLoadingEntries;
	// Number of results the last update has gathered
	int32 TotalNumEntries;
	// Number of accessible pages
	int32 MaxNumPages;
	// The currently selected entry
	TWeakObjectPtr<UPagedListEntryWidget> SelectedEntryWidget;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UScrollBox* ScrollBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPageSelectorWidget* PageSelector;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* EntriesPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPagedListHeaderWidget* SelectedEntryHeaderWidget;
	// ~Widgets

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = PagedList)
	TSubclassOf<UPagedListEntryWidget> EntryWidgetClass;
	UPROPERTY(EditAnywhere, Category = PagedList)
	int32 NumEntriesPerRow;
	UPROPERTY(EditAnywhere, Category = PagedList)
	int32 NumEntriesPerPage;
	// Whether entries should be addressed by their absolute index in the results array, otherwise the relative index on the current page
	UPROPERTY(EditAnywhere, Category = PagedList)
	bool bUseAbsoluteEntryIndices;
	// ~Properties

	// ~Delegates
	FGetLoadedEntry GetLoadedEntryDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnLoadPage OnLoadPageDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnEntrySelected OnEntrySelectedDelegate;
	// ~Delegates

	// ~Constructor
	UPagedListWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	// Adds properties to the properties panel should be called upon NativeConstruct
	UFUNCTION(BlueprintCallable)
	void InitializePropertiesPanel(UObject* Container);
	// Starts reloading the entries on the current page
	UFUNCTION(BlueprintCallable)
	void RefreshEntries();
	// To be called when loading has finished, 
	UFUNCTION(BlueprintCallable)
	void OnEntriesLoaded(int32 InTotalNumEntries, int32 InMaxPages = -1);
	// Returns the currently opened page
	UFUNCTION(BlueprintPure)
	int32 GetCurrentPage() const;
	// Changes the current page
	UFUNCTION(BlueprintCallable)
	void SetCurrentPage(int32 InPage);
	// Get the currently selected entry widget
	UFUNCTION(BlueprintPure)
	UPagedListEntryWidget* GetSelectedEntryWidget() const
	{
		return SelectedEntryWidget.Get();
	}

	// Unselects all entries
	UFUNCTION(BlueprintCallable)
	bool ClearSelectedEntryWidget();

	// Updates the details displayed for the selected entry
	void UpdateSelectedEntryDetails();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsLoading(bool bNewLoading);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsEntrySelected(bool bIsEntrySelected);
	// ~Blueprint Interface

private:
	// Set the loading flag
	bool SetIsLoadingEntries(bool bNewLoading);
	// Callback for the entries
	void OnEntryClicked(UPagedListEntryWidget* Widget, bool bDoubleClick);
	// Sets the currently selected entry
	void SetSelectedEntryWidget(UPagedListEntryWidget* Widget, bool bCallNotify, bool bDoubleClick);
	// Updates the pages on the page selector
	void UpdatePageSelector();
	// Callback for the page selector
	UFUNCTION()
	void OnPageChanged(int32 NewPage);
};
