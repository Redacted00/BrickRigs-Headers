// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Input/KeyDisplayTableRow.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputKeyWidget.generated.h"

class UDataTable;

const static FName GamepadInvalidKeyName = FName("Gamepad_Invalid");

/**
 * This widget is used to display a single key (spacebar, E, enter, gamepad A etc.), not an input action
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputKeyWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The data table we get the key display info from, not transient so it will be cooked
	UPROPERTY()
	UDataTable* KeyDisplayTable;
	// ~Variables

protected:
	// ~Properties
	// The key that this widget should represent
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Key, meta = (ExposeOnSpawn = "true"))
	FKey Key;
	// Whether the key is a modifier key used as such, will strip the 'left' from left shift for example
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Key, meta = (ExposeOnSpawn = "true"))
	bool bIsUsedAsModifier;
	// Render opacity to apply when the key is unbound
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Key)
	float UnboundRenderOpacity;
	// ~Properties

public:
	// ~Constructor
	UInputKeyWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	// ~Super Interface

	// Set the displayed key
	UFUNCTION(BlueprintCallable)
	void SetKey(const FKey& InKey, bool bInUsedAsModifier = false);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDisplayInfo(const FKeyDisplayTableRow& DisplayInfo, bool bIsGamepadKey);
	// ~Blueprint Interface

private:
	// Helper function to update the widget visuals
	void UpdateKey();
};
