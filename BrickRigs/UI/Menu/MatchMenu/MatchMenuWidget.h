// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/MenuPageWidget.h"
#include "MatchMenuWidget.generated.h"

class UPropertiesPanelWidget;
struct FMatchSettings;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UMatchMenuWidget : public UMenuPageWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Callback for the restart button
	UFUNCTION(BlueprintCallable)
	void RestartMatch();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateRestartMatchButton(bool bCanRestart, bool bSameLevelAndMode);
	// Updates the properties panel enabled state
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIsAdmin(bool bNewIsAdmin);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with the correct parameters
	void UpdateRestartMatchButton();
	// Callback for the game state
	void OnNextMatchSettingsChanged(const FMatchSettings& MatchSettings);
};
