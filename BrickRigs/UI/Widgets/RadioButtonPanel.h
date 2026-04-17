// Copyright Fluppisoft 2018

#pragma once

#include "RadioButtonWidget.h"
#include "CoreMinimal.h"
#include "Components/GridPanel.h"
#include "RadioButtonPanel.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API URadioButtonPanel : public UGridPanel
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnButtonSelected, URadioButtonWidget *, Button, bool, bNewSelected);

	// ~Variables
	// All currently selected widgets
	TArray<TWeakObjectPtr<URadioButtonWidget>> SelectedWidgets;
	// ~Variables

public:
	// ~Properties
	// Min amount of buttons that have to be checked
	UPROPERTY(EditAnywhere, Category = RadioButton)
	int32 MinSelectedButtons;
	// Max number of buttons that can be selected at the same time
	UPROPERTY(EditAnywhere, Category = RadioButton)
	int32 MaxSelectedButtons;
	// ~Properties

	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnButtonSelected OnButtonSelectedDelegate;
	// ~Delegates

	// ~Constructor
	URadioButtonPanel();

	// ~Super Interface
	virtual void OnSlotRemoved(UPanelSlot* InSlot) override;
	// ~Super Interface

	// Get the first selected widget
	UFUNCTION(BlueprintPure)
	URadioButtonWidget* GetSelectedWidget() const;
	UFUNCTION(BlueprintCallable)
	void GetSelectedWidgets(TArray<URadioButtonWidget*>& OutWidgets);
	UFUNCTION(BlueprintPure)
	int32 GetNumSelectedWidgets() const;
	// This can be called to manually select a certain button without clicking it
	UFUNCTION(BlueprintCallable)
	void SelectButton(URadioButtonWidget* Button);
	// Manually unselect a button
	UFUNCTION(BlueprintCallable)
	void UnselectButton(URadioButtonWidget* Button);
	// Unselect all buttons, regardless if the min limit would be violated
	UFUNCTION(BlueprintCallable)
	void UnselectAll();

	// Called by the radio buttons directly
	void OnRadioButtonClicked(URadioButtonWidget* Widget);

public:
	void OnButtonSelectedInternal(URadioButtonWidget* Widget, bool bNewSelected, bool bCallNotify);
};
