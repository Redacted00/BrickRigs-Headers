// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "BrickComboBoxItemContainerWidget.generated.h"

class UBrickComboBoxWidget;
class UBrickComboBoxItemWidget;
class UBrickButtonWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickComboBoxItemContainerWidget : public UBrickUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The owning combo box
	TWeakObjectPtr<UBrickComboBoxWidget> ComboBox;
	// The item widget being held by the combo box
	UPROPERTY(Transient)
	UBrickComboBoxItemWidget* ItemWidget;
	// The item associated with this widget
	int32 Item;
	// Whether the item is selected
	uint8 bIsSelected : 1;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

	// Sets the owning combo box
	void SetComboBox(UBrickComboBoxWidget* InComboBox);
	// Adds the item widget to the container
	void SetItemWidget(UBrickComboBoxItemWidget* InWidget);
	// Updates the associated item and selected state
	void InitializeItem(int32 InItem, bool bInIsSelected, bool bInIsEnabled);
	// Removes the associated item widget
	void RemoveItemWidget();
	// Returns the associated item widget
	UBrickComboBoxItemWidget* GetItemWidget() const
	{
		return ItemWidget;
	}

	// Marks the item as selected or unselected
	void SetIsSelected(bool bNewSelected);
	// Updates the selection state based on the currently selected item
	void UpdateIsSelected(int32 SelectedItem);
	// Called upon button click
	UFUNCTION(BlueprintCallable)
	void SelectItem();

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsSelected(bool bNewSelected);
	UFUNCTION(BlueprintImplementableEvent)
	void AddItemWidget(UBrickComboBoxItemWidget* Widget);
	// ~Blueprint Interface

private:
	// Callback for the button
	UFUNCTION()
	void OnUpdateButtonContentStyle(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InContentStyleState);
};
