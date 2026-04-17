// Copyright Fluppisoft, 2020

#pragma once

#include "Inventory/InventorySlot.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Inventory/InventoryComponent.h"
#include "InventoryWidget.generated.h"

class UInventoryPopupWidget;
class UInventorySlotWidget;
class UGridPanel;
class UInventoryComponent;
class UInventoryDragDropOperation;
struct FInventorySpecialSlot;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The outer popup widget
	TWeakObjectPtr<UInventoryPopupWidget> PopupWidget;
	// The currently viewed inventory component
	TWeakObjectPtr<UInventoryComponent> InventoryComponent;
	// All slots widgets used on this inventory
	UPROPERTY(Transient)
	TArray<UInventorySlotWidget*> SlotWidgets;
	// Number of slots on the grid panel currently occupied
	int32 NumGridSlotsOccupied;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* SlotsPanel;
	// ~Widgets

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int32 NumSlotsPerRow;
	// ~Properties

	// ~Constructor
	UInventoryWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Sets the outer popup widget
	void SetInventoryPopupWidget(UInventoryPopupWidget* InWidget);
	// Sets the associated inventory component
	void SetInventoryComponent(UInventoryComponent* InInventory, bool bInIsSubInventory);
	// Returns the associated inventory component
	UInventoryComponent* GetInventoryComponent() const;
	// Gets the outer popup widget
	UInventoryPopupWidget* GetInventoryPopupWidget() const;
	// Get the array of all contained slot widgets
	const TArray<UInventorySlotWidget*>& GetSlotWidgets() const;

private:
	// Creates and adds a new slot widget
	UInventorySlotWidget* CreateSlotWidget(const FInventorySlotID& InSlotID, const FInventorySpecialSlot* InSpecialSlot, int32 InNumColumns, const FText& InDisplayName);
	// Try to find the widget associated with the given slot ID
	UInventorySlotWidget* FindSlotWidget(const FInventorySlotID& InSlotID) const;
	// Callbacks for the inventory
	virtual void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event);
	virtual void OnInventoryModified();
};
