// Copyright Fluppisoft, 2020

#pragma once

#include "Projectiles/AmmoType.h"
#include "Interaction/InteractionOptions.h"
#include "InventoryLoadout.h"
#include "InventorySpecialSlot.h"
#include "InventorySlot.h"
#include "Inventory.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/BrickAssetManager.h"
#include "InventoryComponent.generated.h"

// Actors
class ABaseCharacter;
// Components
class UAudioComponent;
class UFirearmComponent;

// This struct contains all customizable properties for inventories, useful to edit in static info assets for example
USTRUCT(BlueprintType)
struct FInventoryProperties
{
	GENERATED_BODY()

	// Display name of the inventory
	UPROPERTY(EditAnywhere, Category = Inventory)
	FText DisplayName;
	// Sound played when anything is added to the inventory
	UPROPERTY(EditAnywhere, Category = Inventory)
	TSoftObjectPtr<USoundBase> ItemAddedSound;
	// Number of slots that can be occupied by any type of item
	UPROPERTY(EditAnywhere, Category = Inventory)
	int32 NumWildcardSlots;
	// Special slots with item restrictions and custom attach points
	UPROPERTY(EditAnywhere, Category = Inventory)
	TArray<FInventorySpecialSlot> SpecialSlots;
	// Maximum number of items that can sit in a single slot
	UPROPERTY(EditAnywhere, Category = Inventory)
	int32 MaxAmountPerSlot;

	// ~Constructor
	FInventoryProperties();

	bool HasAnySlots() const
	{
		return NumWildcardSlots > 0 || SpecialSlots.Num() > 0;
	}
};

enum class EInventorySlotEvent : uint8
{
	PreRemove,
	PostAdd,
	PostChange
};

// This struct is used to pass around the move item function parameters more easily
USTRUCT()
struct FMoveInventoryItemParams
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> FromInventory;
	UPROPERTY()
	FInventorySlotID FromSlotID;
	UPROPERTY()
	TWeakObjectPtr<UInventoryComponent> ToInventory;
	UPROPERTY()
	FInventorySlotID ToSlotID;
	UPROPERTY()
	uint16 Amount;
	// Optional item for pick up operations
	UPROPERTY()
	AInventoryItem* PickedUpItem;

	// ~Constructor
	FMoveInventoryItemParams(
		UInventoryComponent* InFromInventory = nullptr,
		const FInventorySlot& InFromSlot = FInventorySlot(),
		UInventoryComponent* InToInventory = nullptr,
		const FInventorySlotID& InToSlotID = FInventorySlotID())
		: FromInventory(InFromInventory), FromSlotID(InFromSlot.GetSlotID()), ToInventory(InToInventory), ToSlotID(InToSlotID), Amount(InFromSlot.GetAmount()), PickedUpItem(InFromInventory ? nullptr : InFromSlot.GetReplicatedItem())
	{
	}

	// Checks if the params are valid and allowed to be used
	bool IsValid() const;
};

UCLASS(ClassGroup = (BrickRigs), meta = (BlueprintSpawnableComponent))
class BRICKRIGS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FInventoryModifiedDelegate);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FInventorySlotDelegate, const FInventorySlot&, EInventorySlotEvent);
	DECLARE_DELEGATE_RetVal_OneParam(bool, FCanAccessInventory, ABaseCharacter*);

protected:
	struct FRemoveItemsResult
	{
		FInventoryItemRef ItemRef;
		int32 NumRemoved;
		int32 NewAmount;

		FRemoveItemsResult()
			: NumRemoved(0), NewAmount(0)
		{
		}
	};

	// ~Variables
	// Used for async loading
	FSmartStreamableHandle StreamableHandle_ItemAddedSound;

	// The struct that stores information about the inventory
	UPROPERTY(Transient, Replicated)
	FInventory Inventory;
	// The unique key incremented when the inventory was changed
	int32 ReplicationKey;
	// Indicates if the inventory was changed during replication
	uint8 bReplicatedInventoryModified : 1;
	// Indicates if items should be made visible at all, can be false for example if the inventory is behind a closed door
	uint8 bContentHidden : 1;
	// Set to true while one or more items are being removed from the inventory
	uint8 bIsDestroyingItems : 1;
	// Whether the player is currently accessing this item
	uint8 bIsAccessedByPlayer : 1;
	// The component special slot items are attached to
	TWeakObjectPtr<USceneComponent> SpecialSlotAttachParent;
	// ~Variables

	// ~Components
	// The audio component responsible to play the 3D sounds of an inventory, such as when something is added to it
	UPROPERTY(Transient)
	UAudioComponent* AudioComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Inventory)
	FInventoryProperties InventoryProperties;
	// ~Properties

	// ~Delegates
	FCanAccessInventory CanAccessInventoryDelegate;
	FInventoryModifiedDelegate OnInventoryModifiedDelegate;
	FInventorySlotDelegate OnSlotModifiedDelegate;
	// ~Delegates

	// Constructor
	UInventoryComponent();

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PreNetReceive() override;
	virtual void PostNetReceive() override;
	// ~Super Interface

	// ~Debug
	UFUNCTION(BlueprintCallable)
	void DebugSpawnItems(TSubclassOf<AInventoryItem> ItemClass, int32 Amount = 1);
	// ~Debug

	// ~Inventory
	int32 GetReplicationKey() const
	{
		return ReplicationKey;
	}

	// Initializes the inventory properties
	UFUNCTION(BlueprintCallable)
	void SetInventoryProperties(const FInventoryProperties& InProps);
	// Initializes the attach parent for special slots
	UFUNCTION(BlueprintCallable)
	void SetSpecialSlotAttachParent(USceneComponent* NewAttachParent);

	UFUNCTION(BlueprintPure)
	// Get the display name of the inventory or its owner
	FText GetDisplayName() const;
	// Return true if the player is allowed to see and modify the inventory
	virtual bool CanCharacterAccessInventory(ABaseCharacter* InCharacter) const;
	// Marks this inventory as accessed by the local player
	virtual void SetIsAccessedByPlayer(bool bNewAccessed);
	// Whether the player is currently accessing this item
	bool IsAccessedByPlayer() const
	{
		return bIsAccessedByPlayer;
	}

protected:
	// Called whenever bIsAccessedByPlayer has changed
	virtual void OnIsAccessedByPlayerChanged()
	{
	}

public:
	// Used for the loadout container inventory class
	virtual bool IsLoadoutContainer() const;
	// Helper function to return whether there is anything in the inventory or not
	bool IsEmpty() const;
	// Get the inventory state
	UFUNCTION(BlueprintPure)
	const FInventory& GetInventory() const;
	// Copies the inventory state
	void CopyInventory(const FInventory& InInventory);
	// Public accessor to the inventory slots
	const TArray<FInventorySlot>& GetInventorySlots() const;
	// Helper function to return the contained items in the order they should be displayed
	void GetSlotsSorted(TArray<FInventorySlot>& OutSlots) const;
	// Get all item instances referenced by the inventory
	void GetItems(TArray<AInventoryItem*>& OutItems) const;
	// Fills the inventory with the given loadout, server only
	void SpawnInventoryLoadout(const FInventoryLoadout& Loadout);
	// Tries to consume (destroy) the desired amount of items from the given slot, returns the amount of items consumed
	int32 ConsumeItems(const FInventorySlotID& SlotID, int32 Amount);
	// ~Inventory

	// ~Adding and Removing
	// Empty the inventory and delete all items
	UFUNCTION(BlueprintCallable)
	void EmptyInventory(bool bCallDelegates = true);
	// Called when a item is being destroyed in order to safely remove it from the inventory
	void OnItemDestroyed(AInventoryItem* InItem);
	// Useful to add items to the inventory from scratch, returns the number of items actually added
	int32 SpawnItems(TSubclassOf<AInventoryItem> ItemClass, int32 Amount, TArray<AInventoryItem*>* OutDroppedItems = nullptr);
	// Returns if and how many items can be moved to the other inventory
	static int32 CanMoveItems(const FMoveInventoryItemParams& Params);
	// Try to move items from one inventory to the other
	static int32 MoveItems(const FMoveInventoryItemParams& Params, TArray<AInventoryItem*>& OutDroppedItems);

protected:
	// Returns how many items could be added to the given slot
	virtual int32 CanAddItems(const FInventorySlot& InSlot, bool bSpawningItems, bool bIgnoreIfOccupied = false) const;
	// Add items to the inventory, returns how many items have been added
	int32 AddItems(FInventorySlot SlotToAdd, bool bSpawningItems, TArray<AInventoryItem*>* OutSpawnedItems = nullptr);
	// Returns the number of items that can be removed from the given slot
	virtual int32 CanRemoveItems(const FInventorySlotID& SlotID, int32 Amount) const;
	// Remove a certain type of item from the inventory, returns the amount of items that have been removed
	FRemoveItemsResult RemoveItems(const FInventorySlotID& SlotID, int32 Amount, bool bMovedToNewSlot = false);
	// Spawn items after they have been removed from the inventory
	void SpawnRemovedItems(FRemoveItemsResult& Result, int32 NumItemsRecycled, TArray<AInventoryItem*>& OutDroppedItems);

public:
	// Spawns a new item that is not part of the inventory
	AInventoryItem* SpawnedDroppedItem(UClass* ItemClass);

protected:
	// To be called when any item has been added or picked up (but not spawned)
	void OnPickedUpAnyItem();
	// Used to let clients play the picked up sound
	UFUNCTION(NetMulticast, Reliable)
	void MulticastOnPickedUpItems();
	// ~Adding and Removing

	// ~Spawning
	// Return whether the given item should be spawned and replicated when added to the inventory
	virtual bool ShouldReplicateItem(const AInventoryItem* InItem) const;
	// Spawns a single item instanced, could be a local instance or replicated
	void SpawnItem(FInventorySlot& ItemSlot, bool bReplicate);
	// ~Spawning

public:
	// ~Ammo
	// Whether the inventory has infinite ammo
	bool HasInfiniteAmmo() const;
	// Get the amount of ammo compatible with the given ammo types, if non are supplied all ammo is returned
	int32 GetAmmo(const TArray<FAmmoInfo>& InAmmoTypes = {}) const;
	// Version that supports a single ammo type
	int32 GetAmmo(const FAmmoInfo& InAmmo) const
	{
		return GetAmmo(TArray<FAmmoInfo>({InAmmo}));
	}

	// Consume ammo of the given type, returns the amount that can be used
	int32 ConsumeAmmo(const FAmmoInfo& InAmmo, int32 NumRequired);
	// ~Ammo

	// ~Getters
	// Try to find an inventory slot with the given ID
	FInventorySlot* FindInventorySlot(const FInventorySlotID& InSlotID) const;
	// Find the inventory slot for a specific item
	FInventorySlot* FindInventorySlotForItem(AInventoryItem* InItem) const;
	// Return whether a slot with the given ID exists
	bool HasInventorySlot(const FInventorySlotID& InSlotID) const;
	// Return whether the given item requires a special slot to be stored
	bool RequiresSpecialSlot(const AInventoryItem* InItem, TSet<FInventorySlotID>& OutSlots) const;
	// Return whether the item can be added to the given slot
	bool DoesSpecialSlotSupportItem(const AInventoryItem* InItem, int32 SlotIndex) const;
	// Get the special slot info associated with the given slot ID
	const FInventorySpecialSlot* FindSpecialSlotByID(const FInventorySlotID& InSlotID) const;
	// Get the slot ID for the given special slot index
	FInventorySlotID GetSpecialSlotID(int32 Index) const;
	// Get the wildcard slot ID for the given index
	FInventorySlotID GetWildcardSlotID(int32 Index) const;
	// Get the minimum valid wildcard slot ID
	FInventorySlotID GetMinWildcardSlotID() const;
	// Get the max valid slot ID
	FInventorySlotID GetMaxSlotID() const;
	// Try to find a special slot that is unused
	FInventorySlotID FindFreeSpecialSlotID(const FInventoryItemRef& InItem, const TSet<FInventorySlotID>& InSlotIDs) const;
	// Try to find an unused (or identical) wildcard slot
	FInventorySlotID FindFreeWildcardSlotID(const FInventoryItemRef& InItem) const;
	// Tries to find a free slot ID the item could be added to, optionally outputting a slot that could be replaced if nothing is free
	FInventorySlotID FindFreeSlotID(const FInventoryItemRef& InItem, bool bTryToReplace) const;
	// Get the index of the given wildcard slot
	int32 GetWildcardSlotIndex(const FInventorySlotID& InSlotID) const;
	// Return whether the given slot ID is not occupied yet or by the same item type
	bool IsSlotFreeOrCompatible(const FInventorySlotID& InSlotID, const FInventoryItemRef& InItem) const;
	// Returns whether the given slot is compatible with the given item
	static bool IsSlotCompatible(const FInventorySlot& InSlot, const FInventoryItemRef& InItem);
	// Return whether the given ID refers to a special slot
	bool IsSpecialSlotID(const FInventorySlotID& InSlotID) const;
	// Return whether the given ID refers to a valid wildcard slot
	bool IsWildcardSlotID(const FInventorySlotID& InSlotID) const;
	// Return whether the given slot ID is part of this inventory
	bool IsValidSlotID(const FInventorySlotID& InSlotID) const;
	// Get the total number of items with the same class
	int32 GetNumItemsOfClass(UClass* InClass) const;
	// ~Getters

	// ~Attachment and Visibility
	// Attach an item to the inventory, i.e. attach it to the owner actor
	void StoreItem(AInventoryItem* InItem);
	// Utility function used by AttachItem to determine where to attach the item to, returns false if the item should be hidden instead
	virtual bool GetItemAttachInfo(AInventoryItem* InItem, USceneComponent*& OutAttachParent, FName& OutAttachSocket, FVector& OutLocation, FRotator& OutRotation) const;
	// Show or hide the content
	void SetContentHidden(bool bHidden);
	// Return whether the content is currently hidden
	bool IsContentHidden() const;
	// ~Attachment and Visibility

	// These functions are to be called when slots are added, modified or removed
	// The server will call them directly, on clients they will be called from the fast array serializer events
	void OnSlotAdded(FInventorySlot& InSlot);
	void OnSlotModified(FInventorySlot& InSlot);
	void OnSlotRemoved(const FInventorySlot& InSlot);

private:
	void OnSlotAddedOrModified(FInventorySlot& InSlot);
	// To be called when the inventory was modified in any way
	void OnInventoryModified();

public:
	// Helper function to create a generic access inventory option
	FInteractionOption MakeInteractionOption(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_Inventory(ABrickPlayerController* PC);
};
