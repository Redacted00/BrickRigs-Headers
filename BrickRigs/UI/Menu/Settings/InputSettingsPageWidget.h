// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Settings/MenuSettingsPageWidget.h"
#include "InputSettingsPageWidget.generated.h"

class ABasePlayerController;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputSettingsPageWidget : public UMenuSettingsPageWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Called to revert input mappings to default values
	UFUNCTION(BlueprintCallable)
	void RevertInputMappings();
};
