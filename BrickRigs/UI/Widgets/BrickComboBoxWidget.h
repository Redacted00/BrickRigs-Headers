// Copyright Fluppisoft, 2019

#pragma once

#include "BrickComboBoxItemWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickComboBoxWidget.generated.h"

class UBrickButtonWidget;
class UBrickComboBoxMenuWidget;
class UBrickComboBoxItemContainerWidget;
class UBrickComboBoxItemWidget;
class UGridPanel;

/**
 * Base class for all combo boxes
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickComboBoxWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FOnInitializeItem, int32, Item, FBrickComboBoxItemParams&, OutParams);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemSelected, int32, Item, EValueChangedEventType, EventType);

	// ~Variables
	// The unique item IDs
	// IMPORTANT: This should not be used directly, as it might not always be populated
	TArray<int32> ItemsInternal;
	// Total number of items
	int32 NumItems;
	// The selected item ID
	int32 SelectedItem;
	// Whether the item list should be used instead of a dropdown
	bool bUseItemList;
	// Widget displayed as the selected item
	UPROPERTY(Transient, BlueprintReadOnly)
	UBrickComboBoxItemWidget* SelectedItemWidget;
	// List of all displayed item container widgets
	UPROPERTY(Transient)
	TArray<UBrickComboBoxItemContainerWidget*> ItemContainerWidgets;
	// The dropdown menu widget currently displayed
	UPROPERTY(Transient)
	UBrickComboBoxMenuWidget* MenuWidget;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UGridPanel* ItemsPanel;
	// ~Widgets

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = ComboBox)
	TSubclassOf<UBrickComboBoxItemWidget> ItemWidgetClass;
	UPROPERTY(EditAnywhere, Category = ComboBox)
	TSubclassOf<UBrickComboBoxItemContainerWidget> ItemContainerWidgetClass;
	UPROPERTY(EditAnywhere, Category = ComboBox)
	TSubclassOf<UBrickComboBoxMenuWidget> MenuWidgetClass;
	// Max number of items at which a list will be displayed instead of a drop down, -1 means always list, 0 means never list
	UPROPERTY(EditAnywhere, Category = ComboBox)
	int32 MaxListItems;
	// Maximum number of items to show in a single row, 0 means infinite
	UPROPERTY(EditAnywhere, Category = ComboBox)
	int32 MaxItemsPerRow;
	// ~Properties

	// ~Delegates
	UPROPERTY()
	FOnInitializeItem OnInitializeItemDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnItemSelected OnItemSelectedDelegate;
	// ~Delegates

	// ~Constructor
	UBrickComboBoxWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Initialize the options with continous indices
	UFUNCTION(BlueprintCallable)
	void InitItems(int32 InNumItems, int32 InSelectedItem);
	/** Initialize the items with a list of unique indices, can contain holes and can be ordered in any way
		IMPORTANT: This should not be used for combo boxes that could contain any number of members, like the workshop page list */
	UFUNCTION(BlueprintCallable)
	void InitItemsComplex(const TArray<int32>& InItems, int32 InSelectedItem);
	// Returns the number of selectable items
	UFUNCTION(BlueprintPure)
	int32 GetNumItems() const
	{
		return NumItems;
	}

	// Sets the currently selected item
	UFUNCTION(BlueprintCallable)
	void SetSelectedItem(int32 InItem);
	// Get the currently selected item
	UFUNCTION(BlueprintPure)
	int32 GetSelectedItem() const
	{
		return SelectedItem;
	}

	// Changes how many items can be displayed at once max
	// NOTE: This has to be called BEFORE InitItems(-Complex)
	UFUNCTION(BlueprintCallable)
	void SetMaxListItems(int32 InMaxListItems);
	// Changes the number of items to display per row
	UFUNCTION(BlueprintCallable)
	void SetMaxItemsPerRow(int32 InMaxItemsPerRow);

	// Expands or closes the combo box menu
	UFUNCTION(BlueprintCallable)
	void SetComboBoxExpanded(bool bNewExpanded);
	// Toggles the combo box menu open and close
	UFUNCTION(BlueprintCallable)
	void ToggleComboBoxExpanded();
	// Whether the box is currently expanded
	UFUNCTION(BlueprintPure)
	bool IsComboBoxExpanded() const;

	// Sets the associated menu widget
	void SetComboBoxMenuWidget(UBrickComboBoxMenuWidget* Widget);
	// Called from item containers when they have been clicked
	void SelectListItem(int32 InItem);

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void AddItemContainerWidget(UBrickComboBoxItemContainerWidget* Widget, int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void AddSelectedItemWidget(UBrickComboBoxItemWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUseItemList(bool bInUseItemList);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsExpanded(bool bInIsExpanded);
	// ~Blueprint Interface

	// Return whether the index is within the range
	bool IsValidItemIndex(int32 Index) const
	{
		return Index >= 0 && Index < NumItems;
	}

	// Converts an item ID to its index
	int32 GetIndexFromItem(int32 InItem) const
	{
		return ItemsInternal.Num() ? ItemsInternal.Find(InItem) : InItem;
	}

	// Convert the item index to the associated item ID
	int32 GetItemFromIndex(int32 InIndex) const
	{
		return ItemsInternal.Num() ? ItemsInternal[InIndex] : InIndex;
	}

	// Updates the displayed items
	void UpdateItems();
	// Create a new item widget
	UBrickComboBoxItemWidget* CreateItemWidget();
	// Initialize a new or existing item widget, returns whether the item is enabled
	bool InitializeItemWidget(UBrickComboBoxItemWidget* ItemWidget, int32 Item, bool bIsListItem);

public:
	// NOTE: These are public so they can be called on the menu combo box
	// Called by InitItems and InitItemsComplex
	void InitItemsInternal(const TArray<int32>* InItems, int32 InNumItems, int32 InSelectedItem);
	// Internal version
	bool SetSelectedItemInternal(int32 NewItem, EValueChangedEventType EventType, bool bCallNotify = true, bool bFromComboBoxMenu = false);

private:
	// Initializes the items of the menu widget
	void InitMenuItems();
	// Callback for the combo box menu
	UFUNCTION()
	void OnComboBoxMenuItemSelected(int32 Item, EValueChangedEventType EventType);
};
