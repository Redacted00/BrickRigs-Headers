// Copyright Fluppisoft, 2019

#pragma once

#include "Inventory/InventoryComponent.h"
#include "Components/HUDIconComponent.h"
#include "Actions/CharacterAction.h"
#include "MatineeCameraShake.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterStaticInfo.generated.h"

class UPhysicalMaterial;
class USoundBase;
class USoundAttenuation;

// Indicates if and how far the bones in the hierarchy can flinch
USTRUCT(BlueprintType)
struct FBoneFlinchInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName BoneName;
	UPROPERTY(EditDefaultsOnly)
	float MaxFlinchAngle = 0.f;

	// ~Constructor
	FBoneFlinchInfo()
	{
	}

	// ~Constructor
	FBoneFlinchInfo(const FName& InBoneName, float InFlinchAngle)
		: BoneName(InBoneName), MaxFlinchAngle(InFlinchAngle)
	{
	}
};

// Useful to customize per bone damage
USTRUCT(BlueprintType)
struct FBoneDamageGroup
{
	GENERATED_BODY()
	;

public:
	UPROPERTY(EditAnywhere)
	TArray<FName> BoneNames;
	UPROPERTY(EditAnywhere)
	float DamageScale;
	UPROPERTY(EditAnywhere)
	bool bIncludeChildBones;

	FBoneDamageGroup()
		: DamageScale(1.5f), bIncludeChildBones(true)
	{
	}
};

// This struct is used to share weapon walk sway parameters more easily
USTRUCT(BlueprintType)
struct FWalkSwayParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FVector OffsetDelay = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector OffsetAmplitude = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FRotator RotationDelay = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)
	FRotator RotationAmplitude = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	FVector ForwardOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FVector StrafeOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FRotator ForwardRotation = FRotator::ZeroRotator;
	UPROPERTY(EditAnywhere)
	FRotator StrafeRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere)
	float OffsetInterpSpeed = 0.f;
	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 0.f;

	UPROPERTY(EditAnywhere)
	FVector AimOffsetScale = FVector::ZeroVector;
	UPROPERTY(EditAnywhere)
	FRotator AimRotationScale = FRotator::ZeroRotator;
};

/**
 *
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UCharacterStaticInfo : public UObject
{
	GENERATED_BODY()

public:
	// ~Properties
	// ~Misc
	// Maximum distance at which the character will be drawn and replicated
	UPROPERTY(EditDefaultsOnly, Category = Character)
	float MaxDrawDist;
	// Physical material to use for the skeletal mesh
	UPROPERTY(EditDefaultsOnly, Category = Character)
	UPhysicalMaterial* PhysicalMaterial;
	// How much to inflate the character capsule for overlapping bricks
	UPROPERTY(EditDefaultsOnly, Category = Character)
	float BrickPickupCapsuleInflation;
	// ~Misc

	// ~Health
	// Max or start health of this character
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float MaxHealth;
	// The minimum damage value required to cause damage
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float MinDamage;
	// Factor applied to radial damage
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float RadialDamageScale;
	// Factor applied to collision damage
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float CollisionDamageScale;
	// Time required to fully heal up, 0 means no auto heal
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float AutoHealTime;
	// Time until the health will regenerate after taking damage
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float AutoHealDelay;
	// Useful to modify per bone damage
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	TArray<FBoneDamageGroup> BoneDamageGroups;
	// Min is the maximum height at which 0 collision damage is applied, gradually increases to 1 at Max
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	FFloatInterval FallDamageHeightRange;
	// Min speed at which the character is killed when run over
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float RunOverSpeed;
	// ~Health

	// ~Fire
	// Damage to apply frequently while on fire
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float FireDamage;
	// How often fire damage is applied
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float FireDamageInterval;
	// Time until the character is automatically extinguished after catching fire
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	float AutoExtinguishDelay;
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	UParticleSystem* FireEmitter;
	UPROPERTY(EditDefaultsOnly, Category = Damage)
	USoundBase* FireSound;
	// ~Fire

	// ~Inventory
	// Properties used to initialize the inventory component
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FInventoryProperties InventoryProperties;
	// ~Inventory

	// ~Animation
	// List of bones that will flinch when directly hit
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<FBoneFlinchInfo> BoneFlinchMap;
	// How long any flinch animation should last for
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float FlinchAnimationLength;
	// Minimum rotation rate at which the rotation animation is shown
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float MinRotationAnimationRate;
	// ~Animation

	// ~Actions
	// Action performed when the character landed after falling
	UPROPERTY(EditDefaultsOnly, Category = Actions)
	TSubclassOf<UCharacterAction> LandActionClass;
	// Melee attack action class
	UPROPERTY(EditDefaultsOnly, Category = Actions)
	TSubclassOf<UCharacterAction> MeleeActionClass;
	// ~Actions

	// ~Camera
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval ViewPitchRange;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval ViewYawRange;
	// Offset below the top of the collision capsule where the eyes are
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float EyeHeightOffset;
	// Speed at which the replicated view rotation is interpolated
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float RemoteViewRotationInterpSpeed;
	// Speed at which the view is interpolated towards the vehicle third person view rotation
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float VehicleViewInterpSpeed;
	// Determines how much the view should follow the vehicle view
	UPROPERTY(EditDefaultsOnly, Category = Camera, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float VehicleViewBlendWeight;
	// Minimum angle difference needed to change the direction when looking backwards
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float VehicleViewDirectionChangeThreshold;
	// Camera shake effect played when landing on the ground
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> LandedCameraShake;
	// ~Camera

	// ~Footsteps
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	FFloatInterval FootstepVolumeModulationRange;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	FFloatInterval FootstepPitchModulationRange;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float WalkFootstepVolume;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float WalkFoostepInterval;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float SprintFootstepVolume;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float CrouchFootstepVolume;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float WalkFootstepInterval;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float SprintFootstepInterval;
	UPROPERTY(EditDefaultsOnly, Category = Footsteps)
	float CrouchFootstepInterval;
	// ~Footsteps

	// ~Sound
	// Attenuation settings to apply to our foley sounds
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundAttenuation* FoleyAttenuationSettings;
	// Volume multiplyer applied to all foley sounds
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	float FoleySoundVolume;
	// Sound played when the character ragdoll collides
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* RagdollCollisionSound;
	// Minimum impact delta speed at which an impact sound will be played
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	float MinCollisionSoundImpactSpeed;
	// Minimum delay between ragdoll collision sounds
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	float MinCollisionSoundDelay;
	// Played when the player aims their weapon
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AimInSound;
	// Played when the player stops aiming
	UPROPERTY(EditDefaultsOnly, Category = Sounds)
	USoundBase* AimOutSound;
	// ~Sound

	// ~UI
	UPROPERTY(EditDefaultsOnly, Category = UI)
	FHUDIconProperties HUDIconProperties;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	float IconOffsetZ;
	UPROPERTY(EditAnywhere, Category = UI)
	float MaxFriendlyIconDrawDist;
	UPROPERTY(EditAnywhere, Category = UI)
	float MaxNeutralIconDrawDist;
	UPROPERTY(EditAnywhere, Category = UI)
	float MaxHostileIconDrawDist;
	// Socket or bone our interactive point is attached to
	UPROPERTY(EditAnywhere, Category = UI)
	FName InteractionSocketName;
	// ~UI

	// ~Sprinting
	UPROPERTY(EditAnywhere, Category = Movement)
	float StartSprintTime;
	UPROPERTY(EditAnywhere, Category = Movement)
	float StopSprintTime;
	// The maximum time at which the solider is allowed to fire before the sprint animation has fully finished
	UPROPERTY(EditAnywhere, Category = Movement)
	float SprintFireThreshold;
	// ~Sprinting

	// ~Aiming
	// Time it takes to fully aim in
	UPROPERTY(EditAnywhere, Category = Aim)
	float AimInTime;
	// Time it takes to fully aim out
	UPROPERTY(EditAnywhere, Category = Aim)
	float AimOutTime;
	// Aim ratio at which the aim in sound will be played, applies reversely to the aim out sound
	UPROPERTY(EditAnywhere, Category = Aim)
	float AimSoundTimeRatio;
	// ~Aiming

	// ~Item Sway
	UPROPERTY(EditAnywhere, Category = ItemSway)
	FWalkSwayParams WalkSwayParams;
	// Max sway angles in XYZ directions
	UPROPERTY(EditAnywhere, Category = ItemSway)
	FRotator ItemSwayMaxAngle;
	// Sway speed in XYZ directions
	UPROPERTY(EditAnywhere, Category = ItemSway)
	FRotator ItemSwaySpeed;
	// Scale applied to the item sway while aiming
	UPROPERTY(EditAnywhere, Category = ItemSway)
	float ItemSwayAimScale;
	// Max angle the item can lag behind in XYZ directions
	UPROPERTY(EditAnywhere, Category = ItemSway)
	FRotator ItemLagMaxAngle;
	// Speed at which the current item lag is interpolated
	UPROPERTY(EditAnywhere, Category = ItemSway)
	float ItemLagInterpSpeed;
	// Rotation speed at which the lag angle is at its maximum
	UPROPERTY(EditAnywhere, Category = ItemSway)
	float ItemLagMaxInputSpeed;
	// Scale applied to the item lag while aimed in
	UPROPERTY(EditAnywhere, Category = ItemSway)
	float ItemLagAimScale;
	// ~Item Sway

	// ~Recoil
	// Power of the recoil function, the lower the value the quicker the recoil will rise and the slower it will fall
	UPROPERTY(EditAnywhere, Category = Recoil, meta = (ClampMin = 0.f))
	float RecoilFalloff;
	// ~Recoil

	// ~Item Dropping
	// Speed applied when items are simply dropped
	UPROPERTY(EditAnywhere, Category = Throwing)
	float ItemDropSpeed;
	// Speed at which items are thrown
	UPROPERTY(EditAnywhere, Category = Throwing)
	float ItemThrowSpeed;
	// Speed at which explosives are thrown after being primed
	UPROPERTY(EditAnywhere, Category = Throwing)
	float ExplosiveThrowSpeed;
	// Angular speed applied to an item after being thrown as a fraction of the throw speed
	UPROPERTY(EditAnywhere, Category = Throwing)
	float ItemThrowAngularSpeedScale;
	// Location offset to apply to items being thrown
	UPROPERTY(EditAnywhere, Category = Throwing)
	FVector ItemDropLocation;
	// Relative rotation of items being thrown
	UPROPERTY(EditAnywhere, Category = Throwing)
	FRotator ItemDropRotation;
	// ~Item Dropping

	// ~Brick Carrying
	// Maximum mass of a brick part to carry
	UPROPERTY(EditAnywhere, Category = Bricks)
	float MaxBrickCarryMass;
	// Distance to keep the bounds of the carried brick away from the camera
	UPROPERTY(EditAnywhere, Category = Bricks)
	float BrickCarryDistance;
	// Determines how fast bricks follow the character with respect to the current distance
	UPROPERTY(EditAnywhere, Category = Bricks)
	float BrickCarrySpeedCoefficient;
	// The maximum acceleration used to follow the character
	UPROPERTY(EditAnywhere, Category = Bricks)
	float BrickCarryAcceleration;
	// Speed at which bricks are thrown
	UPROPERTY(EditAnywhere, Category = Bricks)
	float BrickThrowSpeed;
	// ~Brick Carrying
	// ~Properties

	// ~Constructor
	UCharacterStaticInfo();
};
