// Copyright Fluppisoft, 2016

#pragma once

#include "Vehicle/VehicleInput.h"
#include "Engine/StreamableManager.h"
#include "Components/FirearmComponent.h"
#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "Projectiles/BrickProjectile.h"
#include "Bricks/Brick.h"
#include "GunBrick.generated.h"

class UFirearmComponent;
class UParticleSystemComponent;
class USoundCue;
class ABrickPlayerController;

UCLASS(Abstract)
class BRICKRIGS_API UGunBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Variables
	// Cached relative muzzle transform
	UPROPERTY()
	FTransform RelativeMuzzleTransform;
	// ~Variables

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	FFirearmProperties FirearmProperties;
	UPROPERTY(EditDefaultsOnly, Meta = (Bitmask, BitmaskEnum = EAmmoType), Category = Gun)
	uint16 SupportedAmmoTypes;
	// The ammo to add to the inventory
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	FInventoryLoadoutSlot InventoryAmmoSlot;
	// How long it takes to reload the gun
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float ReloadTime;
	// Impulse to add to the gun for every shot
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float RecoilImpulse;
	// Max length at which barrels have an impact
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float MaxBarrelLength;
	// Spread factor applied at the maximum barrel length, interpolated between
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float MinSpreadRadiusScale;
	// Projectile speed factor applied at the minimum barrel length, interpolated up to the max barrel length
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float MinMuzzleVelocityScale;
	// Projectile damage factor applied at the minimum barrel length, interpolated up to the max barrel length
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float MinDamageScale;
	// Emitter for the shell ejection effect
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	TSoftObjectPtr<UParticleSystem> ShellEmitter;
	// Minimum detail mode for the shell emitter to be rendered
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	TEnumAsByte<EDetailMode> ShellEmitterDetailMode;
	// Maximum distance at which the shell emitter is spawned
	UPROPERTY(EditDefaultsOnly, Category = Gun)
	float MaxShellEmitterDrawDistance;
	// ~Properties

	// ~Constructor
	UGunBrickStaticInfo();

	// ~Super Interface
	virtual void GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~Super Interface
};

UCLASS(Abstract)
class BRICKRIGS_API ULauncherBrickStaticInfo : public UGunBrickStaticInfo
{
	GENERATED_BODY()

	// ~Constructor
	ULauncherBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UGunBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// Used to load the shell emitter
	TSharedPtr<FStreamableHandle> StreamableHandle_ShellEmitter;
	// Timer used to delay reloading
	FTimerHandle TimerHandle_Reload;
	// The main firearm component
	UPROPERTY(Transient)
	UFirearmComponent* FirearmComponent;
	// Particle system used for the shell ejection effect
	TBrickEditorComponentPtr<UParticleSystemComponent> ShellPSC;
	// Hard pointer to the shell emitter asset, needed so it doesn't get unloaded again when the PSC is removed
	UPROPERTY(Transient)
	UParticleSystem* ShellEmitterPtr;
	// Current relative transform of the muzzle
	FTransform MuzzleRelativeTransform;
	// Current input channel value
	bool bInputChannelValue;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gun)
	EAmmoType AmmoType;
	// ~Brick properties

public:
	// ~Constructor
	UGunBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void PostConstructVehicle() override;
	virtual void SetupVehicleInventory(FInventoryProperties& OutProperties, FInventoryLoadout& OutLoadout) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void RepairBrick() override;
	// ~Super Interface

	// Public accessor to the firearm component
	UFirearmComponent* GetFirearmComponent() const
	{
		return FirearmComponent;
	}

	// Returns whether the gun is controlled by the given seat
	bool IsControlledBySeat(const USeatBrick* InSeat) const;
	// Returns the index of the index of the fire action associated with the input channel, -1 if not applicable
	int32 GetFireActionIndex() const
	{
		return InputChannel.IsPlayerInput() && FVehicleInput::IsFireAction(InputChannel.InputAxis) ? FVehicleInput::IndexFromFireAction(InputChannel.InputAxis) : INDEX_NONE;
	}

	// Returns the current world transform of the muzzle
	FTransform GetMuzzleTransform() const
	{
		return MuzzleRelativeTransform * GetWorldTransform();
	}

	// Returns true if two guns with identical properties are in the same spot
	bool DoGunsCoincide(UGunBrick* OtherGun) const;
	// Returns the amount of ammo that can be stored in the gun
	int32 GetAmmoCapacity() const;
	// Returns the amount of ammo currently loaded
	int32 GetAmmo() const;
	// Reloads the gun, optionally only if it is empty
	void Reload(bool bEmptyOnly = false);

private:
	// Updates the barrel related properties
	void UpdateBarrels();
	// Callback for async loading
	void OnShellEmitterLoaded();
	// Called after the shell emitter has been loaded, or was already valid
	void OnShellEmitterLoaded(UParticleSystem* LoadedEmitter);
	// Updates and triggers the shell emitter
	void EjectShellInternal();
	// Timer callback
	void OnReloaded();
	// Callbacks for the firearm component
	void OnFire(float DeltaTimeOffset);
	void OnBoltCycled();
	FMuzzleEffectAttachment GetMuzzleEffectAttachment() const;
	void GetProjectileSpawnPoint(FVector& OutLocation, FRotator& OutRotation, FVector& OutVelocity, float DeltaTimeOffset);
	void GetIgnoredActors(TArray<AActor*>& OutIgnoredActors) const;
	// Called when the input channel value has changed
	void OnInputChannelValueChanged();

	// Property callbacks
	static void GetAmmoTypeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static bool IsAmmoTypeSupported(const FBrickPropertyContainer& Container, const EAmmoType AmmoType);
};
