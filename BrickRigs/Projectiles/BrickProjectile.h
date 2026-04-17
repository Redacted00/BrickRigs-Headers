// Copyright Fluppisoft, 2016

#pragma once

#include "World/ExplosiveMaterial.h"
#include "AmmoType.h"
#include "BrickProjectileTypes.h"
#include "FluMoveSyncKinematicActor.h"
#include "Input/BaseInputComponent.h"
#include "Player/ViewTargetInterface.h"
#include "GameFramework/Actor.h"
#include "BrickProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UBrickProjectileMovementComponent;
class UParticleSystemComponent;
class UProjectileManagerComponent;
class UFirearmComponent;

UCLASS(Abstract, HideCategories = ("Actor", "Replication", "Input", "Rendering", "Actor Tick", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API ABrickProjectile : public AFluMoveSyncKinematicActor, public IViewTargetInterface
{
	GENERATED_BODY()

	// ~Variables
	// The firearm which fired this projectile
	UPROPERTY(Transient, Replicated)
	TWeakObjectPtr<UFirearmComponent> FirearmComponent;
	// The parameters the projectile was launched with
	FProjectileParams ProjectileParams;
	// World time when the projectile has been launched
	float LaunchTime;
	// The ammo type of this projectile, cached since it might change on the firearm
	UPROPERTY(Transient, Replicated)
	EAmmoType AmmoType;
	// Number of projectiles that have been merged into this one
	UPROPERTY(Transient, Replicated)
	uint16 NumMergedProjectiles;
	// Current target of the target seeking projectile
	UPROPERTY(Transient, ReplicatedUsing = OnRep_SeekingTarget)
	AActor* SeekingTarget;
	UFUNCTION()
	void OnRep_SeekingTarget(AActor* OldTarget);
	// Last time the seeking target has been updated
	float LastSeekingTargetUpdateTime;
	// Last distance at which an impact has been spawned
	float LastImpactDistanceTravelled;
	// Calculated maximum range
	float ActualMaxRange;
	// Whether the projectile has moved before
	bool bProjectileMoved;

	// The shared particle system component used for the trail effect
	TWeakObjectPtr<UParticleSystemComponent> TrailPSC;
	// Cached camera zoom values
	mutable FViewTargetZoomCache ZoomCache;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient)
	USphereComponent* SphereComponent;
	UPROPERTY(Transient)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(Transient)
	UBrickProjectileMovementComponent* ProjectileMovementComponent;
	// ~Components

public:
	// ~Properties
	// Collision profile to use for the projectile
	UPROPERTY(EditAnywhere, Category = Projectile)
	FName CollisionProfileName;
	// Static mesh displayed for the projectile
	UPROPERTY(EditAnywhere, Category = Projectile)
	UStaticMesh* StaticMesh;
	// The particle system to spawn as the trail
	UPROPERTY(EditAnywhere, Category = Projectile)
	UParticleSystem* TrailParticleSystem;
	// Particle system to spawn upon the desintegration of the projectile
	UPROPERTY(EditAnywhere, Category = Projectile)
	UParticleSystem* DesintegrationParticleSystem;
	// Scale applied to the projectile mesh
	UPROPERTY(EditAnywhere, Category = Projectile)
	float MeshScale;
	// Determines how likely the projectile is to penetrate vs deflect, 1 means always penetrate 0 never
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f), Meta = (ClampMax = 1.f))
	float PenetrationCoefficient;
	// The rate at which the projectile accelerates towards the target
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float HomingAcceleration;
	// The maximum distance at which targets can be detected/followed
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float MaxHomingDistance;
	// Maximum cone angle in which the projectile can detect and follow targets
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f, ClampMax = 180.f))
	float MaxHomingAngle;
	// Maximum distance the projectile aims in front of a moving target
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float MaxHomingLeadDistance;
	// The higher this value the more likely projectiles are to target this flare
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float FlareEffectiveness;
	// How much the projectile is affected by gravity
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float GravityScale;
	// Sound played when the projectile flies past the listener
	UPROPERTY(EditAnywhere, Category = Projectile)
	USoundBase* FlybySound;
	// Time offset between when the projectile passes the listener and when the flyby sound is played, can be negative
	UPROPERTY(EditAnywhere, Category = Projectile)
	float FlybySoundDelay;
	// Pitch multiplier to apply to the flyby sound
	UPROPERTY(EditAnywhere, Category = Projectile)
	float FlybySoundPitch;
	// Zoom parameters for when the projectile is a view target
	UPROPERTY(EditAnywhere, Category = Projectile)
	FViewTargetZoomParams ZoomParams;
	// Damage type to use when applying impact damage
	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<UDamageType> DamageType;

	// Explosive material used on explosive projectiles
	UPROPERTY(EditDefaultsOnly, Category = ExplosiveProjectile)
	TSubclassOf<UExplosiveMaterial> ExplosiveMaterial;
	// Explosive material used on incendiary projectiles
	UPROPERTY(EditDefaultsOnly, Category = ExplosiveProjectile)
	TSubclassOf<UExplosiveMaterial> IncendiaryMaterial;
	// Amount of explosive in this projectile in liters
	UPROPERTY(EditDefaultsOnly, Category = ExplosiveProjectile)
	float ExplosiveVolume;
	// Damage required in order to explode
	UPROPERTY(EditDefaultsOnly, Category = ExplosiveProjectile)
	float ExplosionDamage;
	// ~Properties

	// ~Constructor
	ABrickProjectile(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Destroyed() override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	virtual void PostNetInit() override;
	virtual void TornOff() override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void LifeSpanExpired() override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void OutsideWorldBounds() override;
	virtual void EnableInput(APlayerController* PlayerController) override;
	virtual void DisableInput(APlayerController* PlayerController) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual FVector GetTargetLocation(AActor* RequestedBy = nullptr) const override;
	// ~Super Interface

	// Used to simulate the projectile trajectory, can be called on the CDO
	FVector EstimateProjectileLocation(const FProjectileParams& InProjectileParams, const FVector& StartLocation, const FVector& StartDirection, const FVector& StartVelocity, float GravityZ, float Distance) const;

	// Get the owning controller of the instigator pawn
	AController* GetInstigatorOwningController() const;
	// Returns the firearm this projectile was shot by
	auto* GetFirearmComponent() const
	{
		return FirearmComponent.Get();
	}

	// Whether the projectile will explode upon impact
	bool IsExplosiveProjectile() const
	{
		return FAmmoType::IsExplosive(AmmoType);
	}

	// Initializes the projectile
	void LaunchProjectile(UFirearmComponent* InFirearmComponent, const FVector& Velocity, float InDeltaTimeCorrection);
	// Stops and removes the projectile, this should be called over Destroy() so the projectile can be pooled
	void TerminateProjectile();
	// Whether the projectile has been terminated yet
	bool IsProjectileTerminated() const;
	// Merges another fired projectile into this one
	void MergeProjectile();
	// Update the current homing target location
	TOptional<FVector> UpdateHomingTarget();
	// Updates the seeking target actor
	void UpdateSeekingTarget();
	// Get the world delta time since the projectile has been launched
	float GetTimeSinceLaunch() const;
	// Get the maximum range of this projectile
	float GetActualMaxRange() const;
	// Returns the approximate projectile radius
	float GetProjectileRadius() const;
	// Called when the projectile has hit anything, returns true if the project should stop (for example after exploding)
	bool OnProjectileHit(const FHitResult& Hit, const FVector& ShotDirection, float DistanceTravelled);
	// Called upon a hit to spawn the impact effects
	void SpawnImpactEffects(const FHitResult& Hit, float DistanceTravelled, bool bTerminateProjectile);
	// Called every time the updated component is moved
	void OnProjectileMoved(const FVector& StartLocation, const FVector& EndLocation, const FVector& MoveDelta, float PrevDistanceTravelled, float DistanceTravelled);
	// Calculate the damage the projectile should apply
	float CalcDamage(float DistanceTravelled) const
	{
		return CalcDamage(ProjectileParams, NumMergedProjectiles, DistanceTravelled);
	}

	// Static version
	static float CalcDamage(const FProjectileParams& InProjectileParams, int32 NumMerged, float DistanceTravelled)
	{
		return InProjectileParams.GetDamage(DistanceTravelled) * (NumMerged + 1);
	}

	// Let the projectile apply damage to the given hit, returns true if the projectile should terminate
	static bool ApplyDamage(UWorld* World, const ABrickProjectile* ProjectileCDO, const FProjectileParams& InProjectileParams, EAmmoType InAmmoType, int32 InNumMerged, bool bApplyDamage, const FHitResult& Hit, const FVector& ShotDirection, float DistanceTravelled, APawn* InInstigator, AController* InInstigatorController, AActor* DamageCauser);
	// Spawns the explosion for a projectile
	static void SpawnExplosion(UWorld* World, const FVector& Location, const FRotator& Rotation, const ABrickProjectile* ProjectileCDO, EAmmoType InAmmoType, int32 InNumMerged, APawn* InInstigator);
	// Returns the owning projectile manager
	UProjectileManagerComponent* GetProjectileManager() const;

private:
	// Pools or destroys the projectile if possible
	void PoolOrDestroyProjectile();
	// Called when the projectile was launched on server and clients
	void OnProjectileLaunched();
	// Called when the projectile was terminated on server and clients
	void OnProjectileTerminated();
	// Updates the material parameters
	void UpdateProjectileMaterial(float DistanceTravelled);
	// Deactivates and pools the trail effect
	void DeactivateTrailPSC();

	// Changes the current seeking target, server only
	void SetSeekingTarget(AActor* NewTarget);
	// Called whenever the seeking target has changed
	void OnSeekingTargetChanged(AActor* OldTarget);

	// ~IViewTargetInterface
	virtual void BecomeViewTargetSubobject(APlayerController* PC) override;
	virtual void EndViewTargetSubobject(APlayerController* PC) override;
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	virtual const FViewTargetZoomParams* GetZoomParams() const override
	{
		return &ZoomParams;
	}

	virtual FViewTargetZoomCache* GetZoomCache() const override
	{
		return &ZoomCache;
	}

	virtual FVector GetViewTargetAirVelocity() const override
	{
		return GetVelocity();
	}

	// ~IViewTargetInterface
};
