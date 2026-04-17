// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Menu/Popups/PopupParams.h"
#include "CoreMinimal.h"
#include "UI/Menu/Settings/MenuSettingsPageWidget.h"
#include "VideoSettingsPageWidget.generated.h"

class UPopupWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UVideoSettingsPageWidget : public UMenuSettingsPageWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	FPopupHandle PopupHandle_ConfirmResolution;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = VideoSettings)
	TSoftClassPtr<UPopupWidget> ConfirmResolutionPopupClass;
	// ~Properties

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateApplyButton(bool bCanApply);
	// ~Blueprint Interface

	// Whether the video settings are currently marked dirty
	UFUNCTION(BlueprintPure)
	bool AreVideoSettingsDirty() const;
	UFUNCTION(BlueprintCallable)
	void ApplyVideoSettings();
	UFUNCTION(BlueprintCallable)
	void RevertVideoSettings();

private:
	// User settings callback
	void OnVideoSettingsMarkedDirty(bool bNewDirty);
};
