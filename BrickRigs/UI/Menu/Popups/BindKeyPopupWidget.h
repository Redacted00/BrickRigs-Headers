// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Input/InputActionAndAxisMapping.h"
#include "UI/Input/InputTableRow.h"
#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "BindKeyPopupWidget.generated.h"

class UControlHintWidget;
class UInputChordWidget;
class UMenuButtonWidget;

UCLASS()
class BRICKRIGS_API UBindKeyPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Variables
	// The input table row associated with the bound action
	FInputTableRow InputTableRow;
	// The entered input chord
	FInputChord CurrentInputChord;
	// ~Variables

	// ~Properties
	// The old mapping that is being rebound
	FInputActionAndAxisMapping InputMapping;
	// Whether the mapping is an action
	uint8 bIsAction : 1;
	// Whether this is an axis binding and we are currently changing the negative axis
	uint8 bIsNegativeAxis : 1;
	// Whether the popup has been opened from the input help
	uint8 bOpenedFromInputHelp : 1;
	// ~Properties

	// ~Super Interface
	virtual void NativeOnPopupOpened() override;
	virtual void NativeOnPopupClosed(const EPopupResult Result) override;
	// ~Super Interface
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBindKeyPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

	// ~Variables
	// Whether the key is currently being bound
	uint8 bIsBindingKey : 1;
	// All currently pressed keys
	TArray<FKey> PressedKeys;
	// Used to store the initial analog value received
	TMap<FKey, float> InitialAnalogValues;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UControlHintWidget* ClearKeyControlHint;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInputChordWidget* InputChordWidget;
	// ~Widgets

	// ~Properties
	// Buttons created by blueprint
	UPROPERTY(BlueprintReadWrite)
	UMenuButtonWidget* ConfirmButton;
	UPROPERTY(BlueprintReadWrite)
	UMenuButtonWidget* CancelButton;
	UPROPERTY(BlueprintReadWrite)
	UMenuButtonWidget* RetryButton;
	// ~Properties

public:
	// ~Constructor
	UBindKeyPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;
	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnFocusLost(const FFocusEvent& InFocusEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnKeyUp(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnAnalogValueChanged(const FGeometry& InGeometry, const FAnalogInputEvent& InAnalogEvent) override;
	virtual FText GetPopupTitle_Implementation() const override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsBindingKey(bool bNewIsBinding);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateConflictedMappings(bool bIsConflicted, const TArray<FText>& ConflictedMappingNames);
	// ~Blueprint Interface

	// Popup button callbacks
	UFUNCTION(BlueprintCallable)
	void OnClickedRetry();

private:
	// Whether the mapping being bound is an action
	bool IsActionMapping() const;
	// Start the key binding process
	void StartBindingKey();
	// Stops the key binding process
	void StopBindingKey();
	// Set whether the key is being bound
	void SetIsBindingKey(bool bNewBinding);
	// Called when a viable input key has been pressed or released
	FReply OnInputKeyDown(const FKey& Key);
	FReply OnInputKeyUp(const FKey& Key);
	// Useful for keys that don't have a down state, like axes or the mouse wheel
	FReply OnInputKeyPressed(const FKey& Key);
	// Get the first pressed key that isn't a modifier
	FKey GetFirstNonModifierKey() const;
	// Get the modifiers from the list of pressed keys
	FModifierKeysState GetModifierKeys() const;
	// Converts the pressed keys to an input chord
	bool GetPressedInputChord(FInputChord& OutInputChord) const;
	// Sets the new input chord
	void SetCurrentInputChord(const FInputChord& InInputChord);
	// Update the conflicted mappings
	void UpdateConflictedState();
	// Update the visibility of all buttons
	void UpdateButtonsVisibility();
	// Called when the currently pressed keys have changed
	void OnPressedKeysChanged();
};
