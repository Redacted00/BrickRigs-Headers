// Copyright Fluppisoft, 2016

#pragma once

#include "World/ExplosiveMaterial.h"
#include "AmmoType.h"
#include "Input/BaseInputComponent.h"
#include "Player/ViewTargetInterface.h"
#include "GameFramework/Actor.h"
#include "BrickProjectile.generated.h"

class UStaticMeshComponent;
class UBrickProjectileMovementComponent;
class UParticleSystemComponent;
class UProjectileManagerComponent;
class UFirearmComponent;

USTRUCT(BlueprintType)
struct FProjectileParams
{
	GENERATED_BODY()

	// The launch speed
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 80000.f;
	// Maximum damage applied before the dropoff starts
	UPROPERTY(EditAnywhere)
	float MaxDamage = 0.2f;
	// Minimum damage applied after the dropoff end
	UPROPERTY(EditAnywhere)
	float MinDamage = 0.1f;
	// Distance at which the damage starts to drop
	UPROPERTY(EditAnywhere)
	float DropoffStart = 2000.f;
	// Distance at which the minimum damage is reached
	UPROPERTY(EditAnywhere)
	float DropoffEnd = 10000.f;
	// Maximum range the projectile can travel
	UPROPERTY(EditAnywhere)
	float MaxRange = 100000.f;

	// Get the damage at the given distance travelled
	float GetDamage(float InDistance) const;
	// Get the distance at which the given damage would be applied
	float GetDistance(float InDamage) const;
};

UCLASS()
class BRICKRIGS_API UProjectileInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

public:
	// ~Constructor
	UProjectileInputComponent()
	{
		bForceConsumeInput = true;
	}

	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface
};

UCLASS(Abstract, HideCategories = ("Actor", "Replication", "Input", "Rendering", "Actor Tick", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API ABrickProjectile : public AActor, public IViewTargetInterface
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
	UPROPERTY(Transient)
	APawn* SeekingTarget;
	// Last time the seeking target has been updated
	float LastSeekingTargetUpdateTime;
	// Last distance at which an impact has been spawned
	float LastImpactDistanceTravelled;
	// The shared particle system component used for the trail effect
	TWeakObjectPtr<UParticleSystemComponent> TrailPSC;
	// Cached camera zoom values
	mutable FViewTargetZoomCache ZoomCache;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(Transient)
	UBrickProjectileMovementComponent* ProjectileMovementComponent;
	// ~Components

public:
	// ~Properties
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
	// How much the projectile is affected by gravity
	UPROPERTY(EditAnywhere, Category = Projectile, Meta = (ClampMin = 0.f))
	float GravityScale;
	// The sound played when the projectile flies past the listener
	UPROPERTY(EditAnywhere, Category = Projectile)
	USoundBase* FlybySound;
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
	// ~Properties

	// ~Constructor
	ABrickProjectile(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void Destroyed() override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	virtual void PostNetInit() override;
	virtual void OnRep_ReplicatedMovement() override;
	virtual void TornOff() override;
	virtual void LifeSpanExpired() override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void OutsideWorldBounds() override;
	virtual void EnableInput(APlayerController* PlayerController) override;
	virtual void DisableInput(APlayerController* PlayerController) override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
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
	bool UpdateHomingTarget(FVector& OutTargetLocation, float DeltaTime);
	// Get the world delta time since the projectile has been launched
	float GetTimeSinceLaunch() const;
	// Get the maximum range of this projectile
	float GetMaxRange() const;
	// Returns the approximate projectile radius
	float GetProjectileRadius() const;
	// Called when the projectile has hit anything, returns true if the project should stop (for example after exploding)
	bool OnProjectileHit(const FHitResult& Hit, const FVector& ShotDirection, float DistanceTravelled);
	// Called upon a hit to spawn the impact effects
	void SpawnImpactEffects(const FHitResult& Hit, float DistanceTravelled, bool bTerminateProjectile);
	// Called every time the updated component is moved
	void OnProjectileMoved(const FVector& StartLocation, const FVector& EndLocation, const FVector& MoveDelta, float DistanceTravelled);
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
