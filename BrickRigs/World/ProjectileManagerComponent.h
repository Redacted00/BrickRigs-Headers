// Copyright Fluppisoft, 2020

#pragma once

#include "Misc/BrickDataSingleton.h"
#include "Projectiles/BrickProjectile.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileManagerComponent.generated.h"

class AController;
class ABrickProjectile;
class USceneComponent;
class UParticleSystemComponent;
class UAudioComponent;
class UImpactDecalComponent;

UINTERFACE()
class BRICKRIGS_API UProjectileImpactInterface : public UInterface
{
	GENERATED_BODY()
};

class IProjectileImpactInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// Can be implemented to customize the impact effects
	virtual void OverrideImpactEffects(const FHitResult& Hit, FSurfaceTypeEffects& InOutEffects)
	{
	}
};

UCLASS()
class BRICKRIGS_API UProjectileManagerComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnProjectileCameraChanged, bool);

	// ~Variables
	// List of decals that have been spawned, be it deferred decals or simple meshes
	TArray<TWeakObjectPtr<UImpactDecalComponent>> SpawnedDecals;
	// All active particle systems, mapped to the actor they have been spawned on
	TMap<TWeakObjectPtr<UParticleSystemComponent>, TWeakObjectPtr<AActor>> ActiveParticleSystemComponents;
	// Pooled components
	TArray<TWeakObjectPtr<UParticleSystemComponent>> InactiveParticleSystemComponents;
	// Impact and flyby audio components
	TArray<TWeakObjectPtr<UAudioComponent>> InactiveAudioComponents;
	TArray<TWeakObjectPtr<UImpactDecalComponent>> InactiveDecalComponents;
	// List of inactive, pooled projectiles
	TMap<UClass*, TArray<TWeakObjectPtr<ABrickProjectile>>> PooledProjectiles;
	// Projectiles that have been spawned in the current frame
	// NOTE: We also store the firearm component for projectiles that aren't actually spawned
	TMap<TWeakObjectPtr<UFirearmComponent>, TWeakObjectPtr<ABrickProjectile>> MergableProjectilesSpawnedThisFrame;
	// Last frame in which projectiles have been spawned
	uint64 LastProjectileSpawnFrame;
	// ~Variables

public:
	// ~Delegates
	FOnProjectileCameraChanged OnProjectileCameraChangedDelegate;
	// ~Delegates

	// ~Constructor
	UProjectileManagerComponent();

	// Spawns a new projectile in the world, returns false if spawning failed or the projectile was merged
	bool SpawnProjectile(UFirearmComponent* Firearm, const FVector& Location, const FRotator& Rotation, const FVector& Velocity, bool bReplicateProjectile, float DeltaTimeCorrection, bool bActuallySpawn);
	// Called when a projectile has finished simulating
	void PoolProjectile(ABrickProjectile* Projectile);
	// Spawn a flyby sound effect
	void SpawnFlybyEffect(USoundBase* FlybySound, const FVector& Location);
	// Spawn impact effects from a projectile hit
	void SpawnImpactEffect(const FHitResult& Hit, AController* Instigator, float ProjectileRadius, UParticleSystem* CustomEmitter = nullptr);
	// Whether an impact effect should be spawned at the specified hit, could be false if it is for example too far away to be seen
	bool ShouldSpawnImpactEffect(const FHitResult& Hit, AController* Instigator) const;
	// Whether a decal should be spawned on a hit, might be false for example if there is already a decal in the same spot
	bool ShouldSpawnDecalAtHit(const FHitResult& Hit, float DecalSize);
	// Removes all decals that are attached to the given actor
	void RemoveDecalsFromActor(AActor* Actor);
	// Removes all active impact effects of a certain type from an actor
	void RemoveImpactEffectsFromActor(AActor* Actor, UParticleSystem* Emitter);

private:
	// Removes and pools a specific decal component 
	void RemoveDecalComponent(int32 Index);

public:
	// Plays a short sound effect at the given location, used for impacts and flybys
	void SpawnSoundEffect(USoundBase* Sound, const FVector& Location, float Volume = 1.f);
	// Spawns a new particle system or uses one from the pool
	UParticleSystemComponent* SpawnEmitter(UParticleSystem* Template, AActor* Actor, USceneComponent* AttachParent, const FName& SocketName, const FVector& Location, const FRotator& Rotation, float ProjectileRadius);

private:
	// Used to pool a component that has finished audio/effects etc
	template <class T>
	void PoolComponent(T* Comp, TArray<TWeakObjectPtr<T>>& Pool);
	// Callback for when an impact particle system has finished
	UFUNCTION()
	void OnParticleComponentFinished(UParticleSystemComponent* PSC);
	// Callback for when an impact audio effect has finished
	void OnAudioComponentFinished(UAudioComponent* AudioComponent);
	// Callback for the decals
	void OnDecalComponentFinished(UImpactDecalComponent* DecalComponent);
	// Clears the mergable projectiles if outdated
	void ResetMergableProjectiles();
};

template <class T>
FORCEINLINE void UProjectileManagerComponent::PoolComponent(T* Comp, TArray<TWeakObjectPtr<T>>& Pool)
{
	// Limit the number of cached effects
	if (Pool.Num() < 20)
	{
		// Detach the component to prevent transform updates
		Comp->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepRelative, false));
		Pool.Add(Comp);
	}
	else
	{
		Comp->DestroyComponent();
	}
}
