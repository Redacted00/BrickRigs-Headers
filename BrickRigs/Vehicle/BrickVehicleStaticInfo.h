// Copyright Fluppisoft, 2020

#pragma once

#include "Components/HUDIconComponent.h"
#include "Player/ViewTargetInterface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BrickVehicleStaticInfo.generated.h"

class USoundBase;
class USoundAttenuation;
class UBrickPattern;

/**
 * Contains immutable properties for the vehicle class
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UBrickVehicleStaticInfo : public UObject
{
	GENERATED_BODY()

public:
	// ~Properties
	// Maximum time allowed to spend on constructing a vehicle per frame
	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	float MaxConstructionLoopFrameTime = 0.01f;
	// Maximum time allowed to spend on construction on total
	UPROPERTY(EditDefaultsOnly, Category = Spawning)
	float MaxConstructionTime = 10.f;

	// Maximum distance at which collision and sliding effects are spawned
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	float MaxCollisionEffectSpawnDist;
	// Minimum impact speed at which a collision effect is played
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	float MinCollisionEffectSpeed;
	// Minimum speed at which a sliding effect is played
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	float MinSlidingEffectSpeed;
	// Minimum time between equals or lower impulse collisions
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	float MinCollisionSoundDelay;
	// Min and max sliding speed
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	FFloatInterval SlidingSpeedRange;
	// Sound played for collisions
	UPROPERTY(EditDefaultsOnly, Category = Collision)
	USoundBase* CollisionSound;

	// Emitter used when bricks are on fire
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	UParticleSystem* FireEmitter;
	// Sound played for bricks on fire
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	USoundBase* FireSound;
	// Pattern to apply to bricks when they are burnt
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	TSubclassOf<UBrickPattern> BrickBurntPattern;
	// Min and max number of burn intervals a brick burn for
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	FInt32Interval NumBurnIntervalsRange;
	// Min and max duration of a burn interval
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	FFloatInterval BurnTimeRange;
	// How long a brick can't burn after being extinguished
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	FFloatInterval ExtinguishCooldownRange;
	// How likely any nearby object is to be ignited for every interval
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	float FireSpreadProbability;
	// How far out from a brick the fire can spread
	UPROPERTY(EditDefaultsOnly, Category = Fire)
	float FireSpreadDistance;

	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundAttenuation* HandlingSoundAttenuation;
	// Sound played when any switch is actuated
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* SwitchSound;

	// Zoom params to use for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetZoomParams ZoomParams;
	// Bounds radius used as the reference factor for the min zoom ratio (since we don't want it depend on the actual vehicle size)
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float MinZoomVehicleBoundsRadius;
	// The default zoom ratio applied at startup
	UPROPERTY(EditDefaultsOnly, Category = Camera, meta = (ClampMin = 0.f, ClampMax = 1.f))
	float DefaultZoomRatio;
	// Start view rotation relative to the vehicle
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FRotator DefaultViewRotation;
	// Rate at which the view rotation interpolates
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float ViewRotationInterpSpeed;
	// Below the min the velocity camera doesn't interpolate and at the max it interpolates at the max rate
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval VelocityCameraInputSpeedRange;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraLocationInterpSpeed;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraVelocityInterpSpeed;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	FHUDIconProperties HUDIconProperties;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	float IconOffsetZ;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	float MaxFriendlyIconDrawDist;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	float MaxHostileIconDrawDist;
	// ~Properties

	// ~Constructor
	UBrickVehicleStaticInfo();
};
