// Copyright Fluppisoft, 2019

#pragma once

#include "UI/BrickUIStyle.h"
#include "Framework/Commands/InputChord.h"
#include "InputKeyWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/BrickObjectPool.h"
#include "InputChordWidget.generated.h"

/**
 * Displayes a combination of input keys with modifier keys
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputChordWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY(Transient)
	TArray<UInputKeyWidget*> KeyWidgets;
	UPROPERTY(Transient)
	TArray<UUserWidget*> SpacerWidgets;
	// The cached style
	EBrickUIColorStyle ColorStyle;
	EBrickUIStyleState StyleState;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = KeyCombo)
	TSubclassOf<UInputKeyWidget> KeyWidgetClass;
	UPROPERTY(EditAnywhere, Category = KeyCombo)
	TSubclassOf<UUserWidget> SpacerWidgetClass;
	// The input chord to display
	UPROPERTY(EditAnywhere, Category = KeyCombo)
	FInputChord InputChord;

public:
	// Whether the unbound key should be displayed if no key is assigned
	UPROPERTY(EditAnywhere, Category = KeyCombo)
	bool bShowUnboundKey;
	// ~Properties

	// ~Constructor
	UInputChordWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Set the displayed input chord
	UFUNCTION(BlueprintCallable)
	void SetInputChord(FInputChord InInputChord, bool bShowAsGamepadKey = false);
	// Show or hide the unbound key when no action is assigned
	UFUNCTION(BlueprintCallable)
	void SetShowUnboundKey(bool bShow);
	// Updates the style of the input chord
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	UFUNCTION(BlueprintCallable)
	void SetStyleState(EBrickUIStyleState NewState);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSpacerColorStyleAndStyleState(UUserWidget* Widget, EBrickUIColorStyle NewColorStyle, EBrickUIStyleState NewStyleState);
	UFUNCTION(BlueprintImplementableEvent)
	void AddKeyWidget(UInputKeyWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddSpacerWidget(UUserWidget* Widget);
	// ~Blueprint Interface

private:
	// Update the displayed keys
	void UpdateChord();
	// Updates the style of all spacers
	void UpdateSpacerStyle();
};
