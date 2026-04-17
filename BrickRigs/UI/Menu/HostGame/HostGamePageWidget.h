// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Settings/MenuSettingsPageWidget.h"
#include "HostGamePageWidget.generated.h"

class UMenuButtonWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UHostGamePageWidget : public UMenuSettingsPageWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Starts a game session with the current settings
	UFUNCTION(BlueprintCallable)
	void StartGame();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayButton(bool bCanPlay, bool bIsOnline);
	// ~Blueprint Interface

private:
	// Callback for the settings
	void OnUserSettingsChanged();
};
