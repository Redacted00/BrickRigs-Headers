// Copyright Fluppisoft, 2020

#pragma once

#include "Inventory/InventoryComponent.h"
#include "UI/BrickUIStyle.h"
#include "InventoryDragOperation.h"
#include "Inventory/InventorySlot.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

class UInventoryPopupWidget;
class UInventoryContainerWidget;
class UInventoryWidget;
class UBrickButtonWidget;
class UBrickTextBlock;
class UBrickImage;
class UInventoryComponent;
class UTexture2D;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The owning inventory
	TWeakObjectPtr<UInventoryWidget> InventoryWidget;
	// Cached display name of the slot
	FText SlotDisplayName;
	// Whether this widget represents a special slot
	uint8 bIsSpecialSlot : 1;
	// Whether this widget is being used as the drag visual
	uint8 bIsDragWidget : 1;
	// Whether the drag and drop content is hovering this widget, for the drag widget indicates if it is hovering
	uint8 bIsDragOver : 1;
	// The slot this widget represents
	FInventorySlot InventorySlot;
	// The item to use as the thumbnail for a special slot
	FSoftClassPath ThumbnailItemClass;
	// Widget used for the sub inventory
	UPROPERTY(Transient)
	UInventoryContainerWidget* SubInventoryContainerWidget;
	// The cached drag operation
	TSharedPtr<FInventoryDragOperation> DragOperation;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* AmountTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the outer inventory
	void SetInventoryWidget(UInventoryWidget* InWidget);
	// Initializes the slot widget
	void InitializeSlot(const FInventorySlot& InSlot, bool bInIsSpecialSlot, const FInventorySpecialSlot* SpecialSlot, const FText& InDisplayName);
	// Returns the outer inventory of this slot
	UInventoryComponent* GetInventoryComponent() const;
	// Get the slot this widget represents
	const FInventorySlot& GetInventorySlot() const;
	// Gets the outer popup widget
	UInventoryPopupWidget* GetInventoryPopupWidget() const;
	// Get the contained sub inventory widget
	UInventoryWidget* GetSubInventoryWidget() const;
	// Get the cached geometry from the button widget
	const FGeometry& GetButtonGeometry() const;
	// Used for the drag widget instance
	void InitializeDragWidget(const FInventorySlot& InSlot, bool bInIsSpecialSlot);
	// Called from the inventory widget
	void OnSlotAdded(const FInventorySlot& InSlot);
	void OnSlotModified(const FInventorySlot& InSlot);
	void OnSlotRemoved(const FInventorySlot& InSlot);
	// Drag and drop
	void OnInventoryDragStarted(const TSharedRef<FInventoryDragOperation>& InDragOperation);
	void OnInventoryDragFinished();
	void OnInventoryDragDropped(const TSharedRef<FInventoryDragOperation>& InDragOperation);
	void SetInventoryDragOver(bool bNewIsDragOver);
	// Called when the money of the owning player has changed
	void OnMoneyChanged(float NewMoney);

protected:
	// Callbacks for the button
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();
	UFUNCTION(BlueprintCallable)
	FEventReply OnButtonDragged(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	// ~Blueprint Interface
	// Updates the slot style based on the slot state
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSlotApperance(bool bInIsDragWidget, bool bInIsSpecialSlot, bool bInIsValidSlot, bool bInCanDrag, bool bInIsDragging, bool bInCanDrop, bool bInDropReplace, bool bInIsDragOver);
	// Implement to add the sub inventory widget to this slot
	UFUNCTION(BlueprintImplementableEvent)
	void AddSubInventoryWidget(UInventoryContainerWidget* Widget);
	// ~Blueprint Interface

private:
	// Whether this slot is part of a loadout container
	bool IsInLoadoutContainer() const;
	// Used to initialize or uninitialize the slot
	void OnSlotAddedOrRemoved(const FInventorySlot& InSlot);
	// Calls the blueprint version with the correct parameters
	void UpdateSlotApperance();
	// Updates the item displayed
	void UpdateInventoryItem();
	// Updates the displayed amount
	void UpdateAmount();
	// Starts a drag operation on the popup
	void StartInventoryDrag(const FPointerEvent* InMouseEvent);
	// Generates the params for the move items request
	FMoveInventoryItemParams GetMoveItemsParams(const TSharedRef<FInventoryDragOperation>& InDragOperation) const;
	// Sets the current drag operation and updates the drag appearance
	void SetDragOperation(const TSharedPtr<FInventoryDragOperation>& InDragOperation);
	// Sets the name displayed on the text block
	void SetNameText(const FText& InName);
};
