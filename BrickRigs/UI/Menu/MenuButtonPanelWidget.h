// Copyright Fluppisoft, 2020

#pragma once

#include "MenuButtonWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuButtonPanelWidget.generated.h"

class UGridPanel;
class UGridSlot;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuButtonPanelWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY(Transient)
	TArray<UMenuButtonWidget*> Buttons;
	// Number of buttons currently displayed
	int32 NumButtonsUsed;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* ButtonPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditAnywhere, Category = Defaults)
	TSubclassOf<UMenuButtonWidget> ButtonClass;
	// Number of buttons to add per row, 0 means infinite
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0), Category = Defaults)
	int32 NumButtonsPerRow;
	// ~Properties

public:
	// ~Constructor
	UMenuButtonPanelWidget(const FObjectInitializer& ObjectInitializer);

	// Adds a new button to the panel
	UFUNCTION(BlueprintCallable)
	UMenuButtonWidget* CreateButton(FText DisplayText, int32 IconIndex, const FOnClickedMenuButton& Delegate);
	// Removes all buttons at once, optionally manually removing them after adding new buttons with RemoveUnusedButtons
	UFUNCTION(BlueprintCallable)
	void ClearButtons(bool bManualRemove = false);
	// Removes newly unused buttons, only needed if ClearButtons has been executed with the manual removal flag
	UFUNCTION(BlueprintCallable)
	void RemoveUnusedButtons();
	// Sets the desired number of buttons per row
	UFUNCTION(BlueprintCallable)
	void SetNumButtonsPerRow(int32 NewNum);

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

private:
	// Adds a new button to the panel
	void AddButton(UMenuButtonWidget* Widget, int32 Index);
	// Initializes or updates a button slot
	void UpdateButtonSlot(UGridSlot* GridSlot, int32 Index);
};
