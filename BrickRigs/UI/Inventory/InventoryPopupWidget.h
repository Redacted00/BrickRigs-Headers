// Copyright Fluppisoft, 2020

#pragma once

#include "InventorySlotWidget.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "InventoryPopupWidget.generated.h"

class UInventoryComponent;
class UInventoryContainerWidget;
class UInventoryDragDropOperation;
class UCanvasPanel;
class UBrickButtonWidget;
class ABaseCharacter;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInventoryPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

	// ~Variable
	// The current drag operation
	TSharedPtr<FInventoryDragOperation> DragOperation;
	// The slot widget used as the drag and drop visualization
	UPROPERTY(Transient)
	UInventorySlotWidget* DragWidget;
	// The slot that is currently hovered through the drag operation
	TWeakObjectPtr<UInventorySlotWidget> DragOverSlotWidget;
	// Whether the drop border is being hovered while dragging
	bool bDragOverDropArea;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UCanvasPanel* DragCanvasPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInventoryContainerWidget* OwnInventoryContainerWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInventoryContainerWidget* OtherInventoryContainerWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* DropButton;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInventorySlotWidget> SlotWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TSubclassOf<UInventoryContainerWidget> SubInvetoryContainerWidgetClass;
	// ~Properties

public:
	// ~Constructor
	UInventoryPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	// ~Super Interface

	// Get the player character from the player controller
	ABaseCharacter* GetPlayerCharacter() const;
	// Creates a single slot widget, or reuses it from the pool
	UInventorySlotWidget* CreateSlotWidget();
	// Adds newly unused widgets back to the pool
	void PoolSlotWidgets(TArray<UInventorySlotWidget*>& InSlotWidgets);
	// Creates a new sub inventory widget or pops one from the pool
	UInventoryContainerWidget* CreateSubInventoryWidget();
	// Adds a sub inventory widget back to the pool
	void PoolSubInventoryWidget(UInventoryContainerWidget* InWidget);
	// Get the current drag operation
	const TSharedPtr<FInventoryDragOperation>& GetDragOperation() const;
	// Start dragging an inventory slot via mouse or keyboard
	void StartInventoryDrag(const FInventoryDragOperation& InOperation, const FGeometry& InButtonGeometry, const FPointerEvent* InMouseEvent);
	// Cancel the current drag operation
	void StopInventoryDrag();

private:
	// Whether something is currently being dragged
	bool IsDraggingSlot() const;
	// Updates the position of the dragged widget on the canvas panel
	void UpdateDragWidgetPosition(const FPointerEvent& InMouseEvent);
	// Sets the hovered slot
	void SetDragOverSlotWidget(UInventorySlotWidget* NewWidget);
	// Set whether the drop area is being hovered
	void SetDragOverDropArea(bool bNewDragOver, bool bUpdateAppearance);
	// Called when the drag and drop operation was finished over the drop area
	void OnInventoryDragDropped();
	// Generates the params for the drop items request
	FMoveInventoryItemParams GetDropItemsParams() const;

protected:
	// Updates the drop area appearance
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDropArea(bool bInIsDragging, bool bInCanDrop, bool bInIsDragOver);
	// Callback for the drop button
	UFUNCTION(BlueprintCallable)
	void OnDropButtonClicked();

private:
	// Get the own inventory widget from the container
	UInventoryWidget* GetOwnInventoryWidget() const;
	// Get the other inventory widget from the container
	UInventoryWidget* GetOtherInventoryWidget() const;
	// Calls the blueprint version with the correct parameters
	void UpdateDropArea();
	// Execute a function for each inventory widget
	template <typename FunctionType>
	void ForEachInventoryWidget(FunctionType Func);
	// Execute a function for each slot widget
	template <typename FunctionType>
	void ForEachSlotWidget(FunctionType Func);
	// Callback for the player controller
	void OnAccessedInventoryChanged(UInventoryComponent* AccessedInventory);
	// Callback for the player state
	void OnMoneyChangedInternal(float NewMoney);
	void OnMoneyChanged(float NewMoney, float AmountAdded);
};
