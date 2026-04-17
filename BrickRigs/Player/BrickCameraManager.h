// Copyright Fluppisoft, 2016

#pragma once

#include "Character/CharacterDamageInfo.h"
#include "Camera/PlayerCameraManager.h"
#include "BrickCameraManager.generated.h"

class UAudioComponent;

USTRUCT(BlueprintType)
struct FCameraFadeParams
{
	GENERATED_BODY()
	;

public:
	UPROPERTY(EditAnywhere)
	float FadeInTime;
	UPROPERTY(EditAnywhere)
	float FadeOutTime;
	UPROPERTY(EditAnywhere)
	bool bFadeHUD;
	UPROPERTY(EditAnywhere)
	FLinearColor FadeColor;

	// ~Constructor
	FCameraFadeParams()
		: FadeInTime(0.25f), FadeOutTime(0.25f), bFadeHUD(true), FadeColor(FLinearColor::White)
	{
	}
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached values for smooth interpolation
	float LastFocalDistance;
	// Whether the UI should also be affected by the current screen fade
	bool bFadeHUD;
	// Save fade in time to use after fading out completely
	float FadeInTime;
	// Current time remaining for the hit effect
	float HitEffectTimeRemaining;
	// View rotation to apply for the current hit effect
	FRotator HitEffectFlinchRotation;
	// Current visibility of the restricted area overlay
	float RestrictedAreaEffectAlpha;
	// Current volume of the air noise sound
	float AirNoiseVolume;
	// ~Variables

	// ~Components
	// Used to play air noise when moving fast
	UPROPERTY(Transient)
	UAudioComponent* AirNoiseAudioComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = DepthOfField)
	FFloatInterval FocalDistanceRange;
	UPROPERTY(EditDefaultsOnly, Category = DepthOfField)
	float FocusInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	float HitEffectLength;
	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	float HitEffectFalloff;
	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	float HitEffectFOVScale;
	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	float HitEffectMaxFlinchAngle;
	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	float HitEffectFilmToe;
	UPROPERTY(EditDefaultsOnly, Category = HitEffect)
	FLinearColor HitEffectColorScale;

	UPROPERTY(EditDefaultsOnly, Category = RestrictedArea)
	float RestrictedAreaEffectInterpSpeed;
	UPROPERTY(EditDefaultsOnly, Category = RestrictedArea)
	float RestrictedAreaEffectSaturation;

	UPROPERTY(EditDefaultsOnly, Category = AirNoise)
	USoundBase* AirNoiseSound;
	// Speed range from which the air noise sound starts to fade in and reaches full volume
	UPROPERTY(EditDefaultsOnly, Category = AirNoise)
	FFloatInterval AirNoiseSpeedRange;
	// How quickly air noise volume increases or decreases
	UPROPERTY(EditDefaultsOnly, Category = AirNoise)
	float AirNoiseVolumeInterpSpeed;
	// ~Properties

	// ~Constructor
	ABrickCameraManager();

	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void DoUpdateCamera(float DeltaTime) override;
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;
	virtual void ApplyCameraModifiers(float DeltaTime, FMinimalViewInfo& InOutPOV) override;
	virtual void SetManualCameraFade(float InFadeAmount, FLinearColor Color, bool bInFadeAudio) override;
	virtual void StopCameraFade() override;
	virtual void LimitViewPitch(FRotator& ViewRotation, float InViewPitchMin, float InViewPitchMax) override;
	virtual void LimitViewYaw(FRotator& ViewRotation, float InViewYawMin, float InViewYawMax) override;
	virtual void LimitViewRoll(FRotator& ViewRotation, float InViewPitchMin, float InViewPitchMax) override;
	// ~Super Interface

	// ~Fading
	// Instantly transitions to the given fade parameters
	void SetManualBrickCameraFade(const FCameraFadeParams& InFadeParams);
	// Starts a fade in our out animation
	void StartBrickCameraFade(const FCameraFadeParams& InFadeParams, bool bFadeOut);
	// Applies the current fade amount to the HUD
	void ApplyHUDFade();
	// Public accessor to the menu fading flag
	bool GetFadeHUD() const
	{
		return bFadeHUD;
	}

private:
	// To be called when the fade amount has changed
	void OnFadeAmountChanged();
	// ~Fading

public:
	// Get the current clipping plane offset
	float GetClipPlaneOffset() const;
	// Called from the player controller
	void OnCharacterReceivedDamage(const FCharacterDamageInfo& DamageInfo, float HitDirection);

private:
	// Whether depth of field is enabled from the quality settings, we use this to skip our calculations
	bool IsDepthOfFieldEnabledByScalability() const;
	// Update the current DOF settings
	void UpdateDepthOfField(float DeltaTime, FTViewTarget& OutVT);
};
