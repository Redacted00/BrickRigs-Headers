// Copyright Fluppisoft, 2015

#pragma once

#include "Bricks/Misc/ActuatorTypes.h"
#include "Bricks/Misc/BrickCarryInfo.h"
#include "Vehicle/VehicleInput.h"
#include "Misc/BrickTimer.h"
#include "Components/FirearmComponent.h"
#include "Inventory/InventorySlot.h"
#include "Items/InventoryItem.h"
#include "BaseCharacter.h"
#include "BrickCharacter.generated.h"

// Actors
class ABrickVehicle;
class ABrickPlayerState;
class AFirearm;
// Components
class UFirearmComponent;
// Bricks
class UBrick;
class USeatBrick;
class UActuatorBrick;

UCLASS(Abstract)
class BRICKRIGS_API ABrickCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Name of the socket used as the vehicle third person camera pivot
	static FName VehicleCameraSocketName;

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnVehicleChanged, ABrickVehicle*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnVehicleSeatChanged, USeatBrick*);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCurrentItemChanged, AInventoryItem*);

	// ~Variables
	// Used by the client to revert their item if it couldn't be replicated in time
	FBrickTimer Timer_RevertCurrentItem;
	// Whether RegisteredAllComponents has been called yet
	uint8 bRegisteredAllComponents : 1;

	// ~Inventory
	// The currently equipped item
	// NOTE: This should not be accessed directly since it may not be owned by this character
	UPROPERTY(Transient)
	AInventoryItem* CurrentItemPrivate;
	// Replicated version of the current item
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentItem)
	AInventoryItem* RepCurrentItem;
	UFUNCTION()
	void OnRep_CurrentItem(AInventoryItem* PrevItem);
	// Indicates if a current item is set and owned by this character
	uint8 bHasCurrentItem : 1;
	// The item that is waiting to be equipped
	UPROPERTY(Transient)
	AInventoryItem* NextItem;
	// The brick being carried by the character
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CarriedBrick)
	FBrickCarryInfo CarriedBrick;
	UFUNCTION()
	void OnRep_CarriedBrick(const FBrickCarryInfo& OldCarriedBrick);
	// ~Inventory

	// ~Vehicle
	// Replicated pointer to the seat this character is sitting in
	UPROPERTY(Transient, ReplicatedUsing = OnRep_VehicleSeatID)
	FBrickEditorObjectID VehicleSeatID;
	UFUNCTION()
	void OnRep_VehicleSeatID();
	// Cached reference to the vehicle seat brick
	TWeakObjectPtr<USeatBrick> VehicleSeat;
	// Last time the view rotation and/or input has been sent to the server
	float LastVehicleInputSyncTime;
	// The last input version that has been sent
	uint16 LastVehicleInputSyncKey;
	// Set to true while the driver vehicle and seat are being set
	uint8 bIsEnteringDriverSeat : 1;
	// ~Vehicle

	// ~Input
	// Whether the character wants to fire their weapon right now
	uint8 bWantsToFire : 1;
	// True if the character can and wants to fire
	uint8 bIsFiring : 1;
	// Whether the aim rate has been >0 during the last camera update
	uint8 bWasAimingLastCameraUpdate : 1;
	// How much we are currently aimed in
	float AimRatio;
	// ~Input

	// ~Recoil
	// The main recoil target rotation
	FRotator RecoilTargetRotation;
	// Rotation from which the recoil has started
	FRotator RecoilStartRotation;
	// The remaining permanent recoil rotation that should instantly be applied
	FRotator PermanentRecoilRest;
	// Percentage of the permanent recoil target that has yet to be applied
	float PermanentRecoilRemaining;
	// Time it takes for the recoil to reach the target rotation
	float RecoilStartTime;
	// Time until the recoil has taken its full effect
	float RecoilDuration;

	// Target offset location and rotation
	FVector ItemRecoilTargetOffset;
	FRotator ItemRecoilTargetRotation;
	// Location an rotation at the time the recoil was applied
	FVector ItemRecoilStartOffset;
	FRotator ItemRecoilStartRotation;
	// ~Recoil

	// ~Item Sway
	// Control rotation of last frame, used to calculate the delta rotation for item lag
	FRotator LastControlRotation;
	// Randomly generated seed for the item sway animation
	uint8 ItemSwaySeed;
	// The time through our sway animation
	float ItemSwayTime;
	// Current sway rotation of the item
	FRotator ItemSwayRotation;
	// Current lag rotation of the item
	FRotator ItemLagRotation;
	// ~Item Sway
	// ~Variables

public:
	// ~Delegates
	FOnVehicleChanged OnVehicleChangedDelegate;
	FOnVehicleSeatChanged OnVehicleSeatChangedDelegate;
	FOnCurrentItemChanged OnCurrentItemChangedDelegate;
	// ~Delegates

	// ~Constructor
	ABrickCharacter(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void SetOwner(AActor* NewOwner) override;
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual FRotator GetViewRotation() const override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual bool ShouldHideCharacter() const override;
	virtual bool ShouldEnableMovement() const override;
	virtual void OnRep_IsCrouched() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnStateOfHealthChanged() override;
	virtual FVector GetVelocity() const override;
	virtual void InitialRestart() override;
	virtual void ResetInput() override;
	virtual void ResetViewTargetState() override;
	virtual bool CanAim() const override;
	virtual bool PickUpItemInternal(AInventoryItem* InItem, bool bTestOnly) override;
	virtual void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event) override;
	virtual void PlayActionMontage(UAnimMontage* CharacterMontage, UAnimMontage* ItemMontage, float PlayRate) override;
	virtual void StopActionMontage(UAnimMontage* CharacterMontage, UAnimMontage* ItemMontage, float BlendTime) override;
	virtual bool ShouldIconBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const override;
	virtual FRotator GetInitialViewRotation() const override;
	virtual void GetViewRotationLimit(FRotator& OutMinRotation, FRotator& OutMaxRotation) const override;
	virtual void SetRestartTransform(const FVector& InLocation, const FRotator& InRotation) override;
	virtual FVector GetDesiredMeshLocation() const override;
	// ~Super Interface

private:
	// ~View
	// Used to update the view rotation while in a vehicle and otherwise
	void UpdateViewRotationInternal(FRotator& InOutRotation, float DeltaTime, bool bIsVehicleThirdPersonView = false);
	// ~View

public:
	// ~Inventory
	// Whether the character has an item equipped
	UFUNCTION(BlueprintPure)
	bool HasCurrentItem() const
	{
		return bHasCurrentItem;
	}

	// Returns the currently equipped item
	UFUNCTION(BlueprintPure)
	AInventoryItem* GetCurrentItem() const
	{
		return HasCurrentItem() ? CurrentItemPrivate : nullptr;
	}

	// Templated version
	template <class T>
	T* GetCurrentItem() const
	{
		return Cast<T>(GetCurrentItem());
	}

	// Gets the currently equipped item, or the next one if items are being switched right now
	AInventoryItem* GetCurrentOrNextItem() const;

	// Gets the inventory slot for the current item
	const FInventorySlot* GetCurrentSlot() const;
	// Gets the slot ID for the current item
	FInventorySlotID GetCurrentSlotID() const;

	// Get the first item in the inventory that can be equipped
	AInventoryItem* GetDefaultItem() const;
	// Iterate through the items and find the next equippable one, optionally only of a specific type
	AInventoryItem* GetNextEquippableItem(int8 Dir, const FGameplayTag& ItemTag) const;

	// Whether the tool in the given slot can be equipped currently or not
	UFUNCTION(BlueprintPure)
	bool CanEquipItem(AInventoryItem* InItem) const;
	// Try to equip the given item, optionally skipping the unequip animation. Can be called on the owning client only
	UFUNCTION(BlueprintCallable)
	bool EquipItem(AInventoryItem* InItem, bool bSkipUnequip);
	// Equips an item in the given slot index
	void EquipItemFromIndex(int32 SlotIndex);
	// Cycles to the next item with a matching tag
	void CycleItems(int8 Dir, const FGameplayTag& ItemTag);
	// Called when an equip action has finished
	void OnItemEquipped(bool bLocallyInvoked);
	// Called by the equip action when it has finished, store the current item and equip the new one
	void OnItemUnequipped(bool bLocallyInvoked);

protected:
	// Sest the current equipped item, server or owning client only
	void SetCurrentItem(AInventoryItem* NewItem);
	// Tell the server to update the slot if the client changed it
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCurrentItem(AInventoryItem* NewItem);
	// Used to revert the current item on the client
	void RevertCurrentItem();
	// Called whenever the current item was changed
	void OnCurrentItemChanged(AInventoryItem* PrevItem);
	// Sets the bHasCurrentItem flag and calls the current item delegate
	void UpdateCurrentItem(AInventoryItem* InItem);
	// Forcefully equip a new item, regardless if the character is alive etc.
	void ForceEquipItem(AInventoryItem* InItem);

public:
	// Performs the equip action
	UFUNCTION(BlueprintCallable)
	bool EquipCurrentItem();

private:
	// Performs the unequip action
	bool UnequipCurrentItem();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnequipCurrentItem();
	// Whether the unequip action is currently being performed
	bool IsPerformingUnequipAction() const;
	// Store the given item in the inventory
	void StoreItem(AInventoryItem* InItem);
	// Updates the attachment, view target state etc of the current item
	void InitCurrentItem();
	// Uninitializes the current item, ends it as a view target etc
	void UninitCurrentItem(AInventoryItem* InItem, bool bIsBeingDropped);

public:
	// Called by items when their owner is set to this character
	void OnPickedUpItem(AInventoryItem* InItem);
	// Called by items before their owner is changed, returns true if the item was equipped
	bool OnPreDroppedItem(AInventoryItem* InItem);
	// Called from items after their owner has been cleared
	void OnPostDroppedItem(AInventoryItem* InItem, bool bWasEquipped);
	// Called from the firearm when an attachment has been added or removed from it
	void OnAttachmentAddedOrRemoved(AFirearm* Firearm);

	// Try to throw the item the character is currently carrying, server or owning client only
	UFUNCTION(BlueprintCallable)
	void ThrowCurrentItem();
	// Internal version that takes the view point
	void ThrowCurrentItem(const FPlayerViewPoint& ViewPoint);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerThrowCurrentItem(const FPlayerViewPoint& ViewPoint);
	// Throws the current item, server only
	void ForceThrowCurrentItem(const FPlayerViewPoint& ViewPoint, float ThrowSpeed, bool bPrimeExplosives);

public:
	// Reload the current item
	UFUNCTION(BlueprintCallable)
	void ReloadItem();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReloadItem();

public:
	// Reload the chamber of the current item
	UFUNCTION(BlueprintCallable)
	void CockItem();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCockItem();

public:
	// Change the fire mode of the current weapon
	UFUNCTION(BlueprintCallable)
	void SetFireMode(EFireMode NewMode);
	// Cycle through the modes of the current weapon
	void CycleFireMode();

	// Used to start and stop the extinguish action
	void StartExtinguish();
	void StopExtinguish();

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStartExtinguish();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerStopExtinguish();

public:
	// Starts the throw animation
	UFUNCTION(BlueprintCallable)
	void ThrowExplosive();
	// Whether the character is currently throwing an explosive item
	bool IsThrowingExplosive() const;

private:
	// Starts the throw action on the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerThrowExplosive();

public:
	// Called when the throw animation is finished
	void OnThrewExplosive();

private:
	// Internal version useful for replicating the direction
	void OnThrewExplosive(const FPlayerViewPoint& ViewPoint);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnThrewExplosive(const FPlayerViewPoint& ViewPoint);

public:
	// Whether the given brick can be picked up
	UFUNCTION(BlueprintPure)
	bool CanPickUpBrick(UBrick* Brick) const;
	// Whether the character can currently pick up bricks at all
	bool CanPickUpAnyBrick() const
	{
		return !IsInVehicle() && IsConscious();
	}

	// Whether the character is currently carrying a brick
	UFUNCTION(BlueprintPure)
	bool IsCarryingBrick() const
	{
		return CarriedBrick.Brick.IsValid() && CanCarryCurrentBrick();
	}

	// Returns the carried brick
	UFUNCTION(BlueprintPure)
	UBrick* GetCarriedBrick() const
	{
		return IsCarryingBrick() ? CarriedBrick.Brick.Get() : nullptr;
	}

	// Whether the currently carried brick is still allowed to be carried
	bool CanCarryCurrentBrick() const;
	// Used to pick up and carry bricks
	UFUNCTION(BlueprintCallable)
	void PickUpBrick(UBrick* Brick);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerPickUpBrick(UBrick* Brick);

public:
	// Drops or throws the brick being carried
	UFUNCTION(BlueprintCallable)
	void DropCarriedBrick(bool bYeet);

private:
	void DropCarriedBrickInternal(const FVector& BrickLocation, const FRotator& BrickRotation, const FVector& BrickVelocity);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDropCarriedBrick(const FVector& BrickLocation, const FRotator& BrickRotation, const FVector& BrickVelocity);
	// To be called when the currently carried brick has been dropped
	void OnCarriedBrickDropped(UBrick* Brick, const FRestartTransformVersion& RestartTransformVersion, const FVector& BrickLocation, const FRotator& BrickRotation, const FVector& BrickVelocity);
	// ~Inventory

public:
	// ~Vehicle
	// Whether the character is currently in a vehicle as the driver
	UFUNCTION(BlueprintPure)
	bool IsInDriverSeat() const;
	// Get the seat ID the character is sitting in
	UFUNCTION(BlueprintPure)
	const FBrickEditorObjectID& GetVehicleSeatID() const
	{
		return VehicleSeatID;
	}

	// Get the cached vehicle seat
	UFUNCTION(BlueprintPure)
	USeatBrick* GetVehicleSeat() const
	{
		return VehicleSeat.Get();
	}

	// Whether the player is allowed to enter the given vehicle and seat
	UFUNCTION(BlueprintPure)
	bool CanEnterVehicle(ABrickVehicle* InVehicle, FBrickEditorObjectID& InOutSeatID) const;
	// Let the character enter a different vehicle and seat, server or owning client
	UFUNCTION(BlueprintCallable)
	bool EnterVehicle(ABrickVehicle* InVehicle, FBrickEditorObjectID InSeatID);
	// Allows the player to exit the current vehicle, server or owning client
	UFUNCTION(BlueprintCallable)
	void ExitVehicle();
	// Switches to the next or previous seat
	void CycleVehicleSeats(bool bNextSeat);
	// Forcefully ejects the character from the seat on the spot, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ForceEjectFromVehicle();
	// Get the third person camera pivot world location
	FVector GetVehicleCameraPivot() const;
	// Used to update the view rotation while the vehicle is the view target
	void UpdateVehicleViewRotation(const FRotator& VehicleViewRotation, float DeltaTime);
	// Called from the player controller when any vehicle has been constructed
	void OnAnyVehicleConstructed(ABrickVehicle* InVehicle);

private:
	// Used to enter, exit or change seats in a vehicle, server only
	void ChangeVehicleSeat(ABrickVehicle* InVehicle, const FBrickEditorObjectID& InSeatID = FBrickEditorObjectID());
	// Should be called every time the seat has changed
	void OnVehicleSeatIDChanged();
	// Updates the current seat
	void UpdateVehicleSeat();
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEnterVehicle(ABrickVehicle* InVehicle, const FBrickEditorObjectID& InSeatID);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExitVehicle();
	// Calls the RPC periodically or instantly if needed
	void ServerUpdateVehicleInput(bool bForceUpdate);
	// Used to send the vehicle input, actuator states and view rotation to the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateVehicleInput(const FVehicleInput& NewInput, const TArray<FRepActuatorState>& NewActuatorStates, USeatBrick* Seat, const FRestartTransformVersion& RestartVersion, int32 PackedView, bool bNewAiming);
	// Version that only sends the view rotation in an unreliable way
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdateVehicleViewRotation(USeatBrick* Seat, int32 PackedView, bool bNewAiming);
	// Callbacks for the vehicle
	void OnVehicleConstructed();
	void OnVehicleRepaired();
	void OnVehiclePlayerStateChanged(ABrickPlayerState* NewPlayerState);
	// ~Vehicle

public:
	// ~Gunplay
	// Set whether the owning player wants to fire the current weapon
	UFUNCTION(BlueprintCallable)
	void SetWantsToFire(bool bInWantsToFire);
	// Whether the character state allows it to fire the weapon currently
	UFUNCTION(BlueprintPure)
	bool CanFire() const;
	// Return whether the character wants to and can fire
	UFUNCTION(BlueprintPure)
	bool ShouldFire() const;
	// Whether the owner or remote client is firing
	UFUNCTION(BlueprintPure)
	bool IsFiring() const;

private:
	// Update the owner's firing state
	void UpdateIsFiring(bool bForceUpdate = false);

public:
	// Get the amount the character is aimed in, 0-1
	UFUNCTION(BlueprintPure)
	float GetAimRatio() const;
	// Sets the current aim ratio
	UFUNCTION(BlueprintCallable)
	void SetAimRatio(float InAimRatio);
	// Returns the currently desired FOV and camera offset while aiming with an item
	bool GetItemZoomParams(const FItemAimParams& InAimParams, float& OutTargetFOV, float& OutDefaultFOV, float& OutFOVAlpha, float& OutCameraDepthOffset) const;
	// Get the effective rotation offset of the item relative to the camera
	FRotator GetItemSwayRotation() const;
	// Get the location and direction projectiles should be spawned at
	void GetProjectileSpawnPoint(FVector& OutLocation, FRotator& OutRotation, FVector& OutVelocity, float DeltaTimeOffset) const;
	// Get the current item recoil parameters
	void GetItemRecoil(FVector& OutRecoilOffset, FRotator& OutRecoilRotation, float DeltaTimeOffset) const;
	// Add recoil from firing a weapon
	void AddRecoil(const FFloatInterval& HorizontalRange, const FFloatInterval& VerticalRange,
	               const FVector& OffsetDeviation, const FRotator& RotationDeviation,
	               float InRecoilDuration,
	               TSubclassOf<UCameraShakeBase> RecoilCameraShake, float CameraShakeScale,
	               float DeltaTimeOffset);

private:
	// Get the current recoil function value
	bool GetRecoilValue(float& OutValue, bool& bOutIsRecovering, float DeltaTimeOffset) const;
	// Get the current recoil rotation
	FRotator GetRecoilRotation(float DeltaTimeOffset) const;
	// Get the current view rotation including recoil, separate to GetViewRotation so the DeltaTimeOffset can be used for more precision
	FRotator GetViewRotationWithRecoil(float DeltaTimeOffset) const;
	// Resets all recoil parameters and stops the animation
	void ResetRecoil();
	// Updates the sway rotation
	void UpdateItemSway(float DeltaTime);
	// Updates the item lag rotation
	void UpdateItemLag(float DeltaTime);
	// ~Gunplay

public:
	// ~Firing
	// These are called from the FirearmComponent class to replicate the firing state to the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnSingleFire(UFirearmComponent* FirearmComp);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnReplicatedSingleFire(UFirearmComponent* FirearmComp, int32 MuzzlePitchAndYaw);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnStartAutoFire(UFirearmComponent* FirearmComp);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnReplicatedAutoFire(UFirearmComponent* FirearmComp, int32 MuzzlePitchAndYaw);
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnStopAutoFire(UFirearmComponent* FirearmComp, uint8 RoundsFired);
	// Validates that the given firearm can be used
	bool ValidateFirearmParam(UFirearmComponent* FirearmComp, bool bAutoFire) const;
	// ~Firing

	// ~IBrickPawnInterface
	virtual APawn* GetPawnToPossess() const override;
	virtual AController* GetOwningController() const override;
	virtual ABrickPlayerState* GetOwningPlayerState() const override;
	// ~IBrickPawnInterface

	// ~IViewTargetInterface
	virtual void UpdateViewRotation(FRotator& InOutRotation, float DeltaTime) override;
	virtual bool OverrideFocalDistance(float& InOutFocalDistance) override;
	// ~IViewTargetInterface
};
