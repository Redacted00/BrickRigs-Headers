// Copyright Fluppisoft, 2017

#pragma once

#include "Engine/StreamableManager.h"
#include "Attachment.h"
#include "Components/FirearmComponent.h"
#include "Items/InventoryItem.h"
#include "Firearm.generated.h"

// Actors
class ASightAttachment;
class ABarrelAttachment;
// Components
class UParticleSystemComponent;
class UAudioComponent;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API AFirearm : public AInventoryItem
{
	GENERATED_BODY()

private:
	// ~Timer Handles
	// Used for async loading of the shell emitter
	TSharedPtr<FStreamableHandle> StreamableHandle_ShellEmitter;
	// ~Timer Handles

	// ~Variables
	// Indicates if the last time a shell has been ejected was from a manual cocking
	uint8 bLastShellFromManualReload : 1;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient)
	UFirearmComponent* FirearmComponent;
	// Instantiated on demand
	UPROPERTY(Transient)
	UParticleSystemComponent* ShellPSC;
	// ~Components

public:
	// ~Constructor
	AFirearm();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void SetOwner(AActor* NewOwner) override;
	virtual void Reset() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual bool CanAim() const override;
	virtual void GetAimParams(FItemAimParams& OutParams) const override;
	virtual void UpdateSight(float AimRatio) override;
	virtual float GetSpreadRadius() const override;
	virtual void SetIsFiring(bool bNewFiring) override;
	virtual void OnEquipped() override;
	virtual void RemoveFirstPersonOnlyAssets() override;

	virtual bool ShouldReplicateInInventory() const override
	{
		// Always replicate firearms for their firearm state and attachments
		return true;
	}

	virtual UClass* GetInventoryComponentClass() const override;
	virtual void InitializeInventoryComponent() override;
	virtual void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event) override;
	virtual void OnInventoryModified() override;
	// ~Super Interface

	// Return our firearm component
	UFirearmComponent* GetFirearmComponent() const;
	// Whether the firearm is firing right now
	bool IsFiring() const;
	// Return the current fire mode
	EFireMode GetFireMode() const;
	// Sets the current fire mode
	void SetFireMode(EFireMode NewMode);
	// Whether the firearm supports the given fire mode
	bool HasFireMode(EFireMode NewMode) const;
	// Returns the number of available fire modes
	int32 GetNumFireModes() const;

private:
	// Returns whether this firearm should be cocked by default
	bool ShouldStartCocked() const;

public:
	// Return whether this firearm has a magazine
	bool HasMagazine() const;
	// Return the current magazine fill amount
	int32 GetMagazine() const;
	// Get the max number of bullets in a magazine
	int32 GetMagazineCapacity() const;
	// Get the current chamber state
	EChamberState GetChamberState() const;
	// Whether the player is allowed to aim while reloading the chamber
	bool CanAimWhileCocking() const;

private:
	// Helper function to get the current recoil range, this should be used instead of the variables since they can be modified by skills and attachments
	void GetRecoilRange(FFloatInterval& OutHorizontalRange, FFloatInterval& OutVerticalRange) const;
	// Updates barrel related properties
	void UpdateBarrel(ABarrelAttachment* Barrel);

public:
	// Called when the bolt has been pulled back
	void OnPulledBolt(bool bInvokedLocally);
	// Called when the bolt moved forward from the back position, when it was released or cocked
	void OnReleasedBolt(bool bInvokedLocally);

private:
	// Let the server know we released the bolt
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnReleasedBolt();

public:
	// Called from the reload action when a new magazine has been inserted
	void OnReloaded();

private:
	// Let the server know we reloaded
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnReloaded();

public:
	// Helper function to reset to the magazine count and chamber state
	void ResetAmmo();
	// Return whether the firearm supports reloading in general and has a replacement magazine
	bool CanReload() const;
	// Whether the firearm should be reloaded
	bool HasToReload() const;
	// Whether the firearm currently has to be cocked
	bool HasToCock() const;
	// Whether the firearm can currently be cocked
	bool CanCock() const;

	// Whether the bullets in the magazine should be hidden when the reload animation starts
	bool ShouldHideBulletsOnReload() const;
	// Hide or unhide bullets in the magazine
	void SetHideBullets(bool bHide);

	// Helper function that will reload or cock if required
	void ReloadIfNeeded();
	// Cock the firearm or release the bolt if required
	void CockIfNeeded();

private:
	// Returns the desired muzzle effect attach point
	FMuzzleEffectAttachment GetMuzzleEffectAttachment(ABarrelAttachment* Barrel) const;
	void OnFire(float DeltaTimeOffset);
	void OnBoltCycled();
	// Helper function to determine where projectiles should be spawned
	void GetProjectileSpawnPoint(FVector& OutLocation, FRotator& OutRotation, FVector& OutVelocity, float DeltaTimeOffset) const;
	// Get the actors that fired projectiles should ignore
	void GetIgnoredActors(TArray<AActor*>& OutActors) const;
	// Helper function to activate the shell particle system
	void EjectShell(bool bFromManualReload);
	// Verison called when the emitter is ready
	void EjectShellInternal();
	// Update the shell emitter parameters
	void UpdateShellEmitter();

public:
	// Returns the socket name the given attachment type should be attached to
	FName GetAttachmentSocketName(const FGameplayTag& AttachmentTag) const;
	// Whether the given attachment could be equipped on this weapon
	bool SupportsAttachment(const AAttachment* AttachmentCDO) const;
	// Return the attachment instance on the given type
	template <class T>
	T* GetAttachment() const;
	// Return the sight attachment if it exists
	ASightAttachment* GetSight() const;
	// Get the current barrel attachment
	ABarrelAttachment* GetBarrel() const;
	// Return whether this firearm has a dedicated sight attached
	bool HasSight() const;

private:
	// Callbacks for the async loading of the effects
	void OnShellEmitterLoaded();
	void OnShellEmitterLoaded(UParticleSystem* LoadedEmitter);
	void OnFireAnimationLoaded();
};
