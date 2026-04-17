// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Misc/BrickAssetManager.h"
#include "Components/ActorComponent.h"
#include "Online/BitfieldSerializer.h"
#include "Player/ReplicatedHitInfo.h"
#include "Projectiles/AmmoType.h"
#include "Projectiles/BrickProjectileTypes.h"
#include "FirearmComponent.generated.h"

// Actors
class ABrickCharacter;
// Components
class UParticleSystemComponent;
class UAudioComponent;
class UInventoryComponent;
// Assets
class UParticleSystem;
class USoundBase;

// Fire mode for firearms
UENUM(BlueprintType)
enum class EFireMode : uint8
{
	Single,
	// After every shot the chamber has to be reloaded
	Semi,
	// A new round is automatically loaded
	Burst,
	// Auto with limited burst count
	Auto,
	// Full auto firing
	Max
};

// Chamber state for firearms
UENUM()
enum class EChamberState : uint8
{
	Empty,
	// Not loaded, not bullet or shell
	EmptyShell,
	// An empty shell is in the chamber
	Cocked // Chamber is cocked with a bullet, ready to fire
};

// Defines an audio visual muzzle fire effect
USTRUCT()
struct FMuzzleEffect
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UParticleSystem> Emitter;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<USoundBase> Sound;
};

struct FMuzzleEffectAttachment
{
	TWeakObjectPtr<USceneComponent> AttachParent;
	FName AttachSocketName;
	FVector RelativeLocation;
	FRotator RelativeRotation;
};

USTRUCT()
struct FFirearmState
{
	GENERATED_BODY()

	// ~Variables
	// The current chamber state
	EChamberState ChamberState;
	// The ammo type loaded
	EAmmoType AmmoType;
	// NOTE: Use the reverse round counter instead of the ammo count so it doesn't have to be replicated initially
	uint16 RoundsFired;
	// Whether the owner is currently auto firing
	bool bIsAutoFiring;
	// ~Variables

	bool operator==(const FFirearmState& Other) const
	{
		return ChamberState == Other.ChamberState && AmmoType == Other.AmmoType && RoundsFired == Other.RoundsFired && bIsAutoFiring == Other.bIsAutoFiring;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		// Compress all values down to a single uint16
		TBitfieldSerializer<uint32> BitfieldSerializer(Ar);
		BitfieldSerializer.Add(ChamberState, 2);
		BitfieldSerializer.Add(AmmoType, 13);
		BitfieldSerializer.Add(RoundsFired, 16);
		BitfieldSerializer.Add(bIsAutoFiring, 1);
		BitfieldSerializer.Finish();

		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FFirearmState> : public TStructOpsTypeTraitsBase2<FFirearmState>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

// This struct is usedful for sharing firearm properties between different actors implementing a firearm component
USTRUCT(BlueprintType)
struct FFirearmProperties
{
	GENERATED_BODY()

	// How much ammo this weapon can hold (aka magazine capacity)
	UPROPERTY(EditAnywhere, meta = (ClampMin = 1))
	int32 AmmoCapacity = 1;
	// The projectile caliber
	UPROPERTY(EditAnywhere)
	FGameplayTag Caliber;
	// Ammo type loaded by default
	UPROPERTY(EditAnywhere)
	EAmmoType DefaultAmmoType = EAmmoType::Default;
	// The projectile to fire
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABrickProjectile> ProjectileClass;
	// Projectile damage info etc.
	UPROPERTY(EditAnywhere)
	FProjectileParams ProjectileParams;
	// Number of projectiles to spawn for every shot, which is useful for shotguns
	UPROPERTY(EditAnywhere)
	uint8 NumProjectilesPerShot = 1;
	// Radius of the spread cone at 50m distance
	UPROPERTY(EditDefaultsOnly, Category = Firearm)
	float SpreadRadius = 50.f;
	// Min delay between shots
	UPROPERTY(EditAnywhere)
	float BoltCycleTime = 0.1f;
	// Whether the weapon has a semi fire mode
	UPROPERTY(EditAnywhere)
	uint8 bHasSemiMode : 1 = false;
	// Whether the weapon has an auto fire mode
	UPROPERTY(EditAnywhere)
	uint8 bHasAutoMode : 1 = false;
	// If 0, no burst mode, otherwise the weapon has a burst mode with this amount of rounds
	UPROPERTY(EditAnywhere)
	uint8 BurstRounds = 0;
	// Normal unsuppressed muzzle effect
	UPROPERTY(EditAnywhere)
	FMuzzleEffect MuzzleEffect;

	// ~Constructor
	FFirearmProperties()
	{
		ProjectileParams.InitialSpeed = 80000.f;
		ProjectileParams.MaxRange = 200000.f;
		ProjectileParams.DropoffStart = 1000.f;
		ProjectileParams.DropoffEnd = 10000.f;
		ProjectileParams.MaxDamage = 0.25f;
		ProjectileParams.MinDamage = 0.1f;
	}
};

UCLASS()
class BRICKRIGS_API UFirearmComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnFire, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFirearmStateChanged, const FFirearmState&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFireModeChanged, EFireMode);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnAmmoTypeChanged, EAmmoType);
	DECLARE_DELEGATE_FourParams(FGetProjectileSpawnPoint, FVector&, FRotator&, FVector&, float);
	DECLARE_DELEGATE_OneParam(FGetIgnoredActors, TArray<AActor*>&);
	DECLARE_DELEGATE_RetVal_OneParam(bool, FCanMergeProjectile, UFirearmComponent*);

	// ~Variables
	// Used for async loading of the fire effects
	FSmartStreamableHandle StreamableHandle_FireEmitter;
	FSmartStreamableHandle StreamableHandle_FireSound;
	// Timer for reverting the firearm state when it failed to be replicated by the server
	FTimerHandle TimerHandle_RevertFirearmState;

	// Stores the current ammo and chamber state
	UPROPERTY(ReplicatedUsing = OnRep_FirearmState)
	FFirearmState FirearmState;
	UFUNCTION()
	void OnRep_FirearmState(const FFirearmState& PrevState);
	// Cached replicated firearm state
	FFirearmState LastConfirmedFirearmState;
	// The currently selected fire mode
	// NOTE: We don't replicate the fire mode, players just set it locally
	// This makes sense since it is more intuitive for players anyways if the fire mode is at the default value when they pick it up
	EFireMode FireMode;
	// Cached fire mode that was engaged when the last burst has been started
	// NOTE: This is important so the correct version of the end fire event can be sent to the server
	EFireMode CurrentBurstFireMode;
	// The random seed to use for the bullet spread
	int32 SpreadRandomSeed;

	// Whether the owner wants to fire currently
	uint8 bWantsToFire : 1;
	// Whether the last burst has been fired by the local controller
	uint8 bWasFiringLocally : 1;
	// Number of rounds fired this burst
	uint16 BurstCount;
	// Last world time a round has been fired
	float LastFireTime;
	// Cached pointer to the character that is controlling this firearm
	TWeakObjectPtr<ABrickCharacter> ControllingCharacter;
	// Caches the last time characters have fired this firearm, useful on servers to validate shots fired
	TMap<TWeakObjectPtr<ABrickCharacter>, TMap<EAmmoType, float>> RemoteClientsFireTime;
	// Components for our firing effects
	UPROPERTY(Transient)
	UAudioComponent* FireAC;
	UPROPERTY(Transient)
	UParticleSystemComponent* FirePSC;
	// These hard pointers are used to prevent the fire effects from being unloaded
	UPROPERTY(Transient)
	UParticleSystem* FireEmitter;
	UPROPERTY(Transient)
	USoundBase* FireSound;

	// Basic properties of this firearm, set via InitializeFirearm
	FFirearmProperties FirearmProperties;
	// Contains info about how the muzzle effects should be attached
	FMuzzleEffectAttachment MuzzleEffectAttachment;
	// Whether the muzzle effects have been initialized yet
	uint8 bInitializedFirePSC : 1;
	uint8 bInitializedFireAC : 1;
	// Whether the firearm is currently the view target
	uint8 bIsViewTarget : 1;

	// The unique replication key used to optimize replication
	int32 ReplicationKey;
	// ~Variables

public:
	// ~Delegates
	// Can be bound to play animations etc. when the weapon is fired
	FOnFire OnFireDelegate;
	// Called when the bolt has been cycled
	FSimpleDelegate OnBoltCycledDelegate;
	// Called when the firearm state (ammo and/or chamber state) has changed
	FOnFirearmStateChanged OnFirearmStateChangedDelegate;
	// Called when the fire mode has changed
	FOnFireModeChanged OnFireModeChangedDelegate;
	// Called when the loaded ammo type has changed
	FOnAmmoTypeChanged OnAmmoTypeChangedDelegate;
	// Bind this delegate to provide the current muzzle location to the firearm
	FGetProjectileSpawnPoint GetProjectileSpawnPointDelegate;
	// Implement this to all actors that should be ignored by the projectile
	FGetIgnoredActors GetIgnoredActorsDelegate;
	// Bind to allow projectiles to be merged with other ones shot in the same frame
	FCanMergeProjectile CanMergeProjectileDelegate;
	// Called whenever the replication has been dirtied
	FSimpleMulticastDelegate OnReplicationKeyChangedDelegate;
	// ~Delegates

	// ~Constructor
	UFirearmComponent();

	// ~Super Interface
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// ~Super Interface

	// Public accessor for the replication key
	auto GetReplicationKey() const
	{
		return ReplicationKey;
	}

	// Set the owning character
	void SetCharacter(ABrickCharacter* InCharacter);
	// Return the owning character
	ABrickCharacter* GetCharacter() const;
	// Get the controller owning this firearm
	AController* GetController() const;
	// Returns the instigator to use for spawned projectiles
	APawn* GetProjectileInstigator() const;
	// Get the actors that should be ignored by fired projectiles
	void GetIgnoredActors(TArray<AActor*>& OutActors) const;
	// Returns whether the given hit can be confirmed
	bool ValidateClientHit(ABrickCharacter* InCharacter, const FProjectileHitInfo& HitInfo) const;
	// Return whether the given shot is within the max shot age range
	bool IsShotTimeRelevant(float ShotTime) const;

	// Estimate the location where the gun would hit when fired
	struct FGetPredictedImpactPointRet
	{
		bool bHitSomething;
		bool bIsBehindSomething;
		FVector HitLocation;
	};

	FGetPredictedImpactPointRet GetPredictedImpactPoint(int32 CurrentStep, int32 NumSteps, float MaxRange, float DistributionExponent, const FVector& ViewLocation) const;

	// Initialize the firearm parameters
	void InitializeFirearm(const FFirearmProperties& InProperties, const FMuzzleEffectAttachment& InEffectAttachment, bool bStartCocked = false, int32 InSpreadRandomSeed = -1);
	// Whether the firearm may be fired
	bool MayUseFirearm() const;
	// To be called when the owning firearm becomes or ends being a view target
	void BecomeViewTarget(APlayerController* PC);
	void EndViewTarget(APlayerController* PC);
	// Whether the firearm is being viewed
	bool IsViewTarget() const;

private:
	// Updates the view target state
	void SetIsViewTarget(bool bNewIsViewTarget);

public:
	// Gets the associated projectile class
	const TSubclassOf<ABrickProjectile>& GetProjectileClass() const;
	// Get the projectile params from the firearm properties
	const FProjectileParams& GetProjectileParams() const;
	// Get the maximum ammo capacity (magazine)
	int32 GetAmmoCapacity() const;
	// Get the associated ammo information
	FAmmoInfo GetAmmoInfo() const;
	// Get the currently loaded ammo type
	EAmmoType GetAmmoType() const;
	// Whether the firearm can currently use infinite ammo
	bool HasInfiniteAmmo() const;
	// Resets the state of the firearm, like ammo and bolt cycle timer
	void ResetFirearm(bool bStartCocked);
	// Changes the spread radius
	void SetSpreadRadius(float NewSpreadRadius);
	// Changes the projectile parameters
	void SetProjectileParams(const FProjectileParams& NewParams);
	// Change the current muzzle effect
	void SetMuzzleEffect(const FMuzzleEffect& NewEffect);
	// To be called when the muzzle effect properties have been changed
	void OnMuzzleEffectChanged();
	// Updates the attachment of all muzzle effect components
	void SetMuzzleEffectAttachment(const FMuzzleEffectAttachment& NewAttachment);

private:
	// Used to setup the fire PSC and AC
	void InitializeFirePSC();
	void InitializeFireAC();
	// Update the view target dependent parameters of the audio component
	void UpdateFireACViewTargetParams();
	// Attaches the muzzle effect emitter or sound
	void SetupMuzzleEffectAttachment(USceneComponent* Comp);

public:
	// Public accessor to the current ammo count
	int32 GetAmmo() const;
	// Get the amount of compatible ammo in an inventory
	int32 GetAmmoInInventory(const UInventoryComponent* AmmoInventory) const;
	// Return the current chamber state
	EChamberState GetChamberState() const;

private:
	// Sets the current firearm state
	void SetFirearmState(int32 NewRoundsFired, EChamberState NewChamberState, bool bRevert = true);
	// Called whenever the firearm state has been changed
	void OnFirearmStateChanged(const FFirearmState& PrevState);
	// Revert the firearm state after it failed to be confirmed by the server
	void RevertFirearmState();
	// Set and replicate the auto firing state, server only
	void SetIsAutoFiring(bool bNewAutoFiring);

public:
	// Whether the chamber can currently be reloaded
	bool CanReloadChamber() const;
	// Chamber has been reloaded, add a round from the magazine to the chamber
	void OnReloadedChamber();
	// Whether the firearm could currently be reloaded
	bool CanReload(const UInventoryComponent* AmmoInventory) const;
	// Called when the firearm has been reloaded
	void OnReloaded(UInventoryComponent* AmmoInventory);
	// Return whether this firearm supports the given fire mode
	bool HasFireMode(EFireMode Mode) const;
	// Whether the firearm can be shot automatically
	bool HasAutoOrBurstFireMode() const;
	// Whether the firearm is semi automatic and/or bolt action
	bool HasSemiOrSingleFireMode() const;
	// Return the current fire mode
	EFireMode GetFireMode() const;
	// Change the current fire mode
	void SetFireMode(EFireMode NewMode);
	// Returns the number of available fire modes
	int32 GetNumFireModes() const;

	// Return whether the firearm is currently firing
	bool IsFiring() const;
	// Try to fire the gun
	bool Fire();
	// To be called when the owner can't/doesn't want to fire any longer
	void StopFire();
	// Return whether we can currently fire
	bool CanFire() const;

private:
	// Spawn the projectile, start effects etc.
	void OnFire(bool bFromAutoFire, float DeltaTimeOffset);
	// Called after the client or server have fired any rounds
	void OnFiredNumRounds(int32 NumRounds);

public:
	// Get the current spawn transform
	void GetProjectileSpawnPoint(FVector& OutLocation, FRotator& OutRotation, FVector& OutVelocity, const float DeltaTimeOffset, const bool bIncludeSpread, const int32 ProjectileIndex = 0) const;

private:
	// Spawn the actual projectile, returns false if spawning failed or the projectile was merged
	bool SpawnProjectile(const FRotator* SpawnRotationOverride, bool bReplicateProjectile, float DeltaTimeOffset, bool bActuallySpawn);
	// Whether the projectiles spawned should be replicated
	bool ShouldReplicateProjectile() const;
	// Called from the bolt cycle timer
	void OnBoltCycled(float DeltaTimeOffset);
	// Helper function to determine if the owner can continue firing
	bool CanContinueFire() const;

	// Called from firing replication
	void OnRemoteSingleFire();
	void OnStartRemoteAutoFire();
	void OnStopRemoteAutoFire();

public:
	// Called from the server RPC events
	void OnServerStartFire(ABrickCharacter* InCharacter, bool bAutoFire);
	void OnServerStartReplicatedFire(ABrickCharacter* InCharacter, bool bAutoFire, int32 MuzzlePitchAndYaw);
	void OnServerStopAutoFire(uint16 RoundsFired);

private:
	// To be called when any replicated property was changed
	void MarkReplicationDirty();
};
