// Copyright Fluppisoft, 2019

#pragma once

#include "Player/BrickChatMessage.h"
#include "BrickEditor/BrickEditorTypes.h"
#include "UI/BrickUIStyle.h"
#include "Properties/BrickPropertyInterface.h"
#include "MatchSettings.h"
#include "BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "BrickUserSettings.generated.h"

class UBrickGameInstance;
class USoundClass;
class UInputCategory;

/**
 * NOTE: These settings are saved in the game config and synchronized between devices.
 * Thus no device specific settings should be stored here!
 */
UCLASS(Config = Game)
class BRICKRIGS_API UBrickUserSettings : public UObject, public IBrickPropertyInterface
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnUserSettingsChanged);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoolSettingChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloatSettingChanged, float);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHUDVisibilityChanged, EHUDVisibility, NewVisibility);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeasurementSystemChanged, EMeasurementSystem, NewVisibility);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCameraModeChanged, ECameraMode);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnChatContextChanged, EChatContext);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorUIScaleChanged, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorMirrorModeChanged, EBrickEditorMirrorMode);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorViewModeChanged, EBrickEditorViewMode);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnEditorGizmoWorldSpaceChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBrickUnitsDisplayModeChanged, const EBrickUnitsDisplayMode);

public:
	static constexpr int32 MaxServerPasswordLength = 16;

private:
	// ~Variables
	// The time a key has to be held down to register as a hold
	UPROPERTY(Config)
	float HoldKeyTime;
	// The maximum time a mouse button can be pressed to count as a click
	UPROPERTY(Config)
	float MouseTapTime;
	// ~Variables

	// ~Properties
	// Used to track the project version config was last saved with
	UPROPERTY(Config)
	FString ConfigVersion;

	// ~Server Settings
	// Whether the player wants to host a server in local play
	UPROPERTY(Config)
	EHostServerType HostServerType;
	UPROPERTY(Config)
	FString ServerPassword;
	UPROPERTY(Config)
	FMatchSettings MatchSettings;
	// Whether clients with mismatching mods are allowed to join the server
	UPROPERTY(Config)
	bool bAllowDifferentMods;

public:
	// Whether match settings should be applied immediately when they were changed
	UPROPERTY(Config)
	bool bApplyMatchSettings;
	// Whether match settings should smoothly fade when
	UPROPERTY(Config)
	bool bFadeMatchSettings;
	// ~Server Settings

private:
	// ~UI
	UPROPERTY(Config)
	EHUDVisibility HUDVisibility;
	UPROPERTY(Config)
	EMeasurementSystem MeasurementSystem;
	UPROPERTY(Config)
	EChatContext ChatContext;
	UPROPERTY(Config)
	TSubclassOf<UBrickUIStyle> UIStyle;
	// ~UI

	// ~Gameplay
	// Master volume multiplier of all in game sounds
	UPROPERTY(Config)
	float MasterVolume;
	// Volume of all non 3d music in the game (would not affect an attenuated car radio for example)
	UPROPERTY(Config)
	float MusicVolume;
	// Horizontal or vertical field of view
	UPROPERTY(Config)
	float FieldOfView;
	UPROPERTY(Config)
	ECameraMode CameraMode;
	UPROPERTY(Config)
	ETransmissionMode TransmissionMode;
	UPROPERTY(Config)
	bool bAutoCounterSteering;
	// ~Gameplay

	// ~Input
	UPROPERTY(Config)
	float MouseSensitivity;
	UPROPERTY(Config)
	float GamepadSensitivity;
	UPROPERTY(Config)
	bool bInvertViewPitch;
	UPROPERTY(Config)
	bool bToggleAim;
	UPROPERTY(Config)
	bool bToggleSprint;
	UPROPERTY(Config)
	bool bToggleCrouch;
	// The currently selected input category
	UPROPERTY(Transient)
	UInputCategory* InputCategory;
	UPROPERTY(Transient)
	FString InputMappingSearchText;
	// ~Input

	// ~Brick Editor
	UPROPERTY(Config)
	EBrickEditorMirrorMode EditorMirrorMode;
	UPROPERTY(Config)
	EBrickEditorViewMode EditorViewMode;
	UPROPERTY(Config)
	FWorldSetupParams EditorWorldSetupParams;
	UPROPERTY(Config)
	float EditorUIScale;
	UPROPERTY(Config)
	float EditorMouseMoveSensitivity;
	UPROPERTY(Config)
	float EditorGridSnappingDistance;
	UPROPERTY(Config)
	float EditorGridSnappingAngle;
	UPROPERTY(Config)
	float EditorBrickSnappingDistance;
	UPROPERTY(Config)
	float EditorBrickSnappingAngle;
	UPROPERTY(Config)
	bool bEditorSnappingEnabled;
	UPROPERTY(Config)
	bool bEditorGizmoWorldSpace;
	UPROPERTY(Config)
	EBrickUnitsDisplayMode BrickUnitsDisplayMode;
	// ~Brick Editor

	// ~Camera
	UPROPERTY(Config)
	float EditorZoomRatio;
	UPROPERTY(Config)
	float EditorCameraSpeedRatio;
	UPROPERTY(Config)
	float FreeCamSpeedRatio;
	UPROPERTY(Config)
	float FreeCamShiftSpeedRatio;
	UPROPERTY(Config)
	float ProjectileCamZoomRatio;
	// ~Camera

public:
	// ~Delegates
	// Called when any setting has been changed
	FOnUserSettingsChanged OnUserSettingsChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnHUDVisibilityChanged OnHUDVisibilityChangedDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnMeasurementSystemChanged OnMeasurementSystemChangedDelegate;
	FOnCameraModeChanged OnCameraModeChangedDelegate;
	FOnChatContextChanged OnChatContextChangedDelegate;
	FSimpleMulticastDelegate OnInputMappingFilterChangedDelegate;
	FOnEditorUIScaleChanged OnEditorUIScaleChangedDelegate;
	FOnEditorMirrorModeChanged OnEditorMirrorModeChangedDelegate;
	FOnEditorViewModeChanged OnEditorViewModeChangedDelegate;
	FSimpleMulticastDelegate OnEditorWorldSetupParamsChangedDelegate;
	FSimpleMulticastDelegate OnEditorSnappingSettingsChanged;
	FOnEditorGizmoWorldSpaceChanged OnEditorGizmoWorldSpaceChanged;
	FOnBrickUnitsDisplayModeChanged OnBrickUnitsDisplayModeChanged;
	// ~Delegates

	// Static accessor for the user settings singleton
	UFUNCTION(BlueprintPure)
	static UBrickUserSettings* GetUserSettings();

	// ~Constructor
	UBrickUserSettings();

	// Called when the game instance was initialized
	void OnGameInstanceInit(UBrickGameInstance* GameInstance);

	// ~UI
	float GetHoldKeyTime() const
	{
		return HoldKeyTime;
	}

	float GetMouseTapTime() const
	{
		return MouseTapTime;
	}

	UClass* GetUIStyle() const
	{
		return UIStyle;
	}

	// ~UI

	auto GetFieldOfView() const
	{
		return FieldOfView;
	}

	// Camera Mode
	ECameraMode GetCameraMode() const
	{
		return CameraMode;
	}

	FText GetCameraModeDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), CameraMode, FGetEnumItems::CreateStatic(&ThisClass::GetCameraModeItems));
	}

	void CycleCameraMode()
	{
		FEnumBrickPropertyBase::CycleEnumPropertyExternal(FBrickPropertyContainer(this), CameraMode, FGetEnumItems::CreateStatic(&ThisClass::GetCameraModeItems));
	}

	// Measurement system
	EMeasurementSystem GetMeasurementSystem() const
	{
		return MeasurementSystem;
	}

	FText GetMeasurementSystemDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), MeasurementSystem, FGetEnumItems::CreateStatic(&ThisClass::GetMeasurementSystemItems));
	}

	void CycleMeasurementSystem()
	{
		FEnumBrickPropertyBase::CycleEnumPropertyExternal(FBrickPropertyContainer(this), MeasurementSystem, FGetEnumItems::CreateStatic(&ThisClass::GetMeasurementSystemItems));
	}

	// HUD visibility
	EHUDVisibility GetHUDVisibility() const
	{
		return HUDVisibility;
	}

	FText GetHUDVisibilityDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), HUDVisibility, FGetEnumItems::CreateStatic(&ThisClass::GetHUDVisibilityItems));
	}

	void CycleHUDVisibility()
	{
		FEnumBrickPropertyBase::CycleEnumPropertyExternal(FBrickPropertyContainer(this), HUDVisibility, FGetEnumItems::CreateStatic(&ThisClass::GetHUDVisibilityItems));
	}

	// Chat
	EChatContext GetChatContext() const
	{
		return ChatContext;
	}

	FText GetChatContextDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), ChatContext, FGetEnumItems::CreateStatic(&ThisClass::GetChatContextItems));
	}

	void CycleChatContext()
	{
		FEnumBrickPropertyBase::CycleEnumPropertyExternal(FBrickPropertyContainer(this), ChatContext, FGetEnumItems::CreateStatic(&ThisClass::GetChatContextItems));
	}

	// Transmission mode
	ETransmissionMode GetTransmissionMode() const
	{
		return TransmissionMode;
	}

	FText GetTransmissionModeDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), TransmissionMode, FGetEnumItems::CreateStatic(&ThisClass::GetTransmissionModeItems));
	}

	void CycleTransmissionMode()
	{
		FEnumBrickPropertyBase::CycleEnumPropertyExternal(FBrickPropertyContainer(this), TransmissionMode, FGetEnumItems::CreateStatic(&ThisClass::GetTransmissionModeItems));
	}

	// Auto counter steering
	auto IsAutoCounterSteeringEnabled() const
	{
		return bAutoCounterSteering;
	}

	void ToggleAutoCounterSteeringEnabled()
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), bAutoCounterSteering, !bAutoCounterSteering);
	}

	FText GetAutoCounterSteeringValueText() const;

	// ~Input
	float GetMouseSensitivity() const
	{
		return MouseSensitivity;
	}

	float GetGamepadSensitivity() const
	{
		return GamepadSensitivity;
	}

	bool GetInvertViewPitch() const
	{
		return bInvertViewPitch;
	}

	bool GetToggleAim() const
	{
		return bToggleAim;
	}

	bool GetToggleSprint() const
	{
		return bToggleSprint;
	}

	bool GetToggleCrouch() const
	{
		return bToggleCrouch;
	}

	// ~Input

	// ~Brick Editor
	EBrickEditorMirrorMode GetEditorMirrorMode() const
	{
		return EditorMirrorMode;
	}

	void SetEditorMirrorMode(EBrickEditorMirrorMode NewMode)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), EditorMirrorMode, NewMode);
	}

	EBrickEditorViewMode GetEditorViewMode() const
	{
		return EditorViewMode;
	}

	void SetEditorViewMode(EBrickEditorViewMode NewMode)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), EditorViewMode, NewMode);
	}

	FText GetEditorViewModeDisplayName() const
	{
		return FEnumBrickPropertyBase::GetEnumPropertyValueText(FBrickPropertyContainer(this), EditorViewMode, FGetEnumItems::CreateStatic(&ThisClass::GetEditorViewModeItems));
	}

	const auto& GetEditorWorldSetupParams() const
	{
		return EditorWorldSetupParams;
	}

	float GetEditorUIScale() const
	{
		return EditorUIScale;
	}

	float GetEditorMouseMoveSensitivity() const
	{
		return EditorMouseMoveSensitivity;
	}

	float GetEditorGridSnappingDistance() const
	{
		return EditorGridSnappingDistance;
	}

	float GetEditorGridSnappingAngle() const
	{
		return EditorGridSnappingAngle;
	}

	float GetEditorBrickSnappingDistance() const
	{
		return EditorBrickSnappingDistance;
	}

	float GetEditorBrickSnappingAngle() const
	{
		return EditorBrickSnappingAngle;
	}

	bool IsEditorSnappingEnabled() const
	{
		return bEditorSnappingEnabled;
	}

	void SetEditorSnappingEnabled(bool bNewEnabled)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), bEditorSnappingEnabled, bNewEnabled);
	}

	bool IsEditorGizmoWorldSpace() const
	{
		return bEditorGizmoWorldSpace;
	}

	void SetEditorGizmoWorldSpace(bool bNewWorldSpace)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), bEditorGizmoWorldSpace, bNewWorldSpace);
	}

	auto GetBrickUnitsDisplayMode() const
	{
		return BrickUnitsDisplayMode;
	}

	void SetBrickUnitsDisplayMode(const EBrickUnitsDisplayMode& NewMode)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), BrickUnitsDisplayMode, NewMode);
	}

	// ~Brick Editor

	// ~Camera
	float GetEditorZoomRatio() const
	{
		return EditorZoomRatio;
	}

	void SetEditorZoomRatio(float NewZoomRatio)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), EditorZoomRatio, NewZoomRatio);
	}

	float GetEditorCameraSpeedRatio() const
	{
		return EditorCameraSpeedRatio;
	}

	void SetEditorCameraSpeedRatio(float NewSpeedRatio)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), EditorCameraSpeedRatio, NewSpeedRatio);
	}

	float GetFreeCamSpeedRatio() const
	{
		return FreeCamSpeedRatio;
	}

	void SetFreeCamSpeedRatio(float NewSpeedRatio)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), FreeCamSpeedRatio, NewSpeedRatio);
	}

	float GetFreeCamShiftSpeedRatio() const
	{
		return FreeCamShiftSpeedRatio;
	}

	void SetFreeCamShiftSpeedRatio(float NewSpeedRatio)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), FreeCamShiftSpeedRatio, NewSpeedRatio);
	}

	float GetProjectileCamZoomRatio() const
	{
		return ProjectileCamZoomRatio;
	}

	void SetProjectileCamZoomRatio(float NewZoomRatio)
	{
		FBrickProperty::SetPropertyValueExternal(FBrickPropertyContainer(this), ProjectileCamZoomRatio, NewZoomRatio);
	}

	// ~Camera

	// ~Server Settings
	EHostServerType GetHostServerType() const
	{
		return HostServerType;
	}

	bool IsMultiplayerEnabled() const
	{
		return HostServerType != EHostServerType::None;
	}

	FString GetServerPassword() const
	{
		return ServerPassword;
	}

	const FMatchSettings& GetMatchSettings() const
	{
		return MatchSettings;
	}

	void SetMatchSettings(const FMatchSettings& NewSettings);

	auto GetAllowDifferentMods() const
	{
		return bAllowDifferentMods;
	}

	// ~Server Settings

private:
	// Apply the current volume settings to the sound classes
	void ApplyVolumeSettings();
	// Core delegates callback
	void OnSafeFrameChanged();

	// Property callbacks
	static void GetCameraModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetTransmissionModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetMeasurementSystemItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetHUDVisibilityItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetChatContextItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetHostServerTypeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetEditorMirrorModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetEditorViewModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface
};
