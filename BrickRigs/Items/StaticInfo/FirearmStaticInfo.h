// Copyright Fluppisoft, 2020

#pragma once

#include "MatineeCameraShake.h"
#include "Components/FirearmComponent.h"
#include "CoreMinimal.h"
#include "ItemStaticInfo.h"
#include "FirearmStaticInfo.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UFirearmStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

private:
	// ~Variables
	// Whether the aim offset has been calculated and cached yet
	uint8 bCalculatedAimOffset : 1;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
	FFirearmProperties FirearmProperties;
	// Name of the bone that should be hidden while reloading when the magazine is empty
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
	FName BulletsBoneName;
	// Whether the player is allowed to aim while reloading the chamber
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
	bool bCanAimWhileCocking;

	// Min to max recoil range in degrees
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	FFloatInterval HorizontalRecoilRange;
	// Min to max recoil range in degrees
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	FFloatInterval VerticalRecoilRange;
	// Max random recoil offset
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	FVector RecoilOffsetDeviation;
	// Max random recoil rotation
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	FRotator RecoilRotationDeviation;
	// How long it takes for the recoil to fully take place and recover
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	float RecoilDuration;
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	TSubclassOf<UCameraShakeBase> FiringCameraShake;
	// Intensity of the firing camera shake
	UPROPERTY(EditDefaultsOnly, Category = Recoil)
	float CameraShakeScale;

	// Whether there is something on the weapon potentially blocking scopes from extending over the rear end of the sight rail
	UPROPERTY(EditDefaultsOnly, Category = Sight)
	uint8 bSightRailRearBlocked : 1;
	// Same as bSightRailRearBlocked but for the front
	UPROPERTY(EditDefaultsOnly, Category = Sight)
	uint8 bSightRailFrontBlocked : 1;
	// Total length of the attachable area of the sight rail
	UPROPERTY(EditDefaultsOnly, Category = Sight)
	float SightRailLength;

	// The types of attachments the firearm supports
	UPROPERTY(EditAnywhere, Category = Attachments)
	FGameplayTagContainer AttachmentTags;
	// Sound played when an attachment was added
	UPROPERTY(EditAnywhere, Category = Attachments)
	TSoftObjectPtr<USoundBase> AttachmentAddedSound;
	// Index of the material layer that applies paint
	UPROPERTY(EditAnywhere, Category = Attachments)
	int32 PaintLayerIndex = 1;

	// The particle system for ejecting shells
	UPROPERTY(EditDefaultsOnly, Category = Shells)
	TSoftObjectPtr<UParticleSystem> ShellEmitter;
	// Scale applied to the shell meshes
	UPROPERTY(EditDefaultsOnly, Category = Shells)
	FVector ShellScale;
	// Velocity at which shells are ejected
	UPROPERTY(EditDefaultsOnly, Category = Shells)
	FVector ShellEjectionVelocity;
	// Angular speed applied to shells
	UPROPERTY(EditDefaultsOnly, Category = Shells)
	FVector ShellEjectionAngularVelocity;
	// Speed scale of the shell when ejected through a manual reload rather than automatic cycling
	UPROPERTY(EditDefaultsOnly, Category = Shells)
	float ManualShellEjectionSpeed;
	// Max deviation from the default ejection speed in percent
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f), Category = Shells)
	float ShellEjectionSpeedDeviation;

	// ~Sounds
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* SwitchFireModeSound;
	// ~Sounds

	// ~Animations
	UPROPERTY(EditAnywhere, Category = Animations)
	FFireAnimation FireAnimation;
	UPROPERTY(EditAnywhere, Category = Animations)
	FReloadAnimation ReloadAnimation;
	UPROPERTY(EditAnywhere, Category = Animations)
	FCockAnimation CockAnimation;

	UPROPERTY(EditDefaultsOnly, Category = TechnicalAnimations)
	TArray<FItemRefPoseOverride> SightPoseOverrides;
	// Transform applied to the hammer when it is cocked
	UPROPERTY(EditDefaultsOnly, Category = TechnicalAnimations)
	FTransform HammerCockedTransform;
	// Ref pose overrides applied when the bolt is locked
	UPROPERTY(EditDefaultsOnly, Category = TechnicalAnimations)
	TArray<FItemRefPoseOverride> BoltLockedPoseOverrides;
	// ~Animations
	// ~Properties

	// ~Constructor
	UFirearmStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void OnEndPIE(bool bIsSimulating) override;
#endif
	virtual void OnSkeletalMeshLoaded(USkeletalMesh* SkeletalMeshPtr) override;
	// ~Super Interface
};
