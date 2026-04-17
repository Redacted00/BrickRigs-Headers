// Copyright Fluppisoft, 2023

#pragma once

#include "CoreMinimal.h"
#include "DLSSLibrary.h"
#include "GameFramework/GameUserSettings.h"
#include "Properties/BrickPropertyInterface.h"
#include "MyUserSettings.generated.h"

class UBrickGameInstance;
/**
 * NOTE: These settings are not synchronized and meant for device specific settings
 */
UCLASS()
class BRICKRIGS_API UMyUserSettings : public UGameUserSettings, public IBrickPropertyInterface
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Whether the video settings have pending changes
	uint8 bVideoSettingsDirty : 1;
	// Flag set during the revert settings function
	uint8 bIsRevertingVideoSettings : 1;
	
	UPROPERTY(Config)
	bool bEnableDepthOfField = true;
	UPROPERTY(Config)
	bool bEnableRayTracing;
	UPROPERTY(Config)
	UDLSSMode DLSSMode;
	UPROPERTY(Config)
	float MotionBlurScale = 1.f;
	UPROPERTY(Config)
	TEnumAsByte<EAntiAliasingMethod> AntiAliasingMethod = AAM_TemporalAA;
	// ~Variables

public:
	// ~Delegates
	TMulticastDelegate<void(bool)> OnVideoSettingsMarkedDirtyDelegate;
	// ~Delegates
	
	// Returns our custom game user settings instance
	UFUNCTION(BlueprintPure)
	static UMyUserSettings* Get();
	
	// Called upon the game instance start event
	void OnGameInstanceStart(UBrickGameInstance* GameInstance);
	
	auto IsDepthOfFieldEnabled() const
	{
		return bEnableDepthOfField;
	}

	auto GetMotionBlurScale() const
	{
		return MotionBlurScale;
	}

	// Applies the ray tracing enabled flag
	void ApplyRayTracingEnabled();
	// Applies anti aliasing and DLSS settings
	void ApplyAntiAliasingAndUpscalingSettings();
	
	// Whether the video settings are marked as dirty
	bool AreVideoSettingsDirty() const
	{
		return bVideoSettingsDirty;
	}

	// Applies all pending video settings
	void ApplyVideoSettings(bool& bOutChangedResolution);
	// Reverts the entered video settings
	void RevertVideoSettings();
	// Called when the user confirmed that the new resolution is valid
	void ConfirmResolution();
	// The resolution is not valid, return to the last confirmed one
	void RevertResolution();

private:
	// Sets the video settings dirty flag
	void SetVideoSettingsDirty(bool bNewDirty);

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface

	// Property callbacks
	static void GetAntiAliasingMethodItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetDLSSModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	
};
