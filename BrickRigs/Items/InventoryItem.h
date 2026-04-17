// Copyright Fluppisoft, 2016

#pragma once

#include "FluMoveSyncActor.h"
#include "Engine/StreamableManager.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/InteractionComponent.h"
#include "StaticInfo/ItemStaticInfo.h"
#include "GameFramework/Actor.h"
#include "InventoryItem.generated.h"

// Actors
class ABrickCharacter;
// Components
class UMeshComponent;
class UStaticMeshComponent;
class USkeletalMeshComponent;
class UInteractionComponent;
class UAudioComponent;
// Assets
class USoundBase;
class UAnimMontage;
class UMaterialInstanceDynamic;

UCLASS(Abstract, HideCategories = ("Actor", "Replication", "Input", "Rendering", "Actor Tick", "LOD", "Cooking", "Collision"))
class BRICKRIGS_API AInventoryItem : public AFluMoveSyncActor
{
	GENERATED_BODY()

	// ~Variables
	TSharedPtr<FStreamableHandle> StreamableHandle_SkeletalMesh;

protected:
	// Optional MID used for both meshes
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UStaticMeshComponent* StaticMeshComponent;
	UPROPERTY(Transient)
	USkeletalMeshComponent* SkeletalMeshComponent;
	// Interaction component for picking up the tool, only instantiated when the tool is dropped
	UPROPERTY(Transient)
	UInteractionComponent* InteractionComponent;
	// Optional sub inventory component
	UPROPERTY(Transient)
	UInventoryComponent* InventoryComponent;
	// Audio component used for physics collision sounds
	UPROPERTY(Transient)
	UAudioComponent* CollisionAudioComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Item)
	TSubclassOf<UItemStaticInfo> StaticInfoClass;
	// ~Properties

	// ~Constructor
	AInventoryItem();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void PreRegisterAllComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type Reason) override;
	virtual void Destroyed() override;
	virtual void Reset() override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual void SetOwner(AActor* NewOwner) override;
	virtual void SetActorHiddenInGame(bool bNewHidden) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	virtual bool CheckStillInWorld() override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	// ~Super Interface

	// ~Static Info
	// Get the static info CDO, always returns a valid object
	UFUNCTION(BlueprintPure)
	const UItemStaticInfo* GetStaticInfo() const
	{
#if !UE_BUILD_SHIPPING || !UE_BUILD_TEST
		if (!StaticInfoClass)
		{
			UE_LOG(LogTemp, Error, TEXT("Item class %s does not have a static info asset set!"), *GetClass()->GetName());
			return GetDefault<UItemStaticInfo>();
		}
#endif

		return StaticInfoClass.GetDefaultObject();
	}

	// Templated function to get the casted static info CDO
	template <class T>
	const T* GetStaticInfo() const
	{
#if !UE_BUILD_SHIPPING || !UE_BUILD_TEST
		const auto StaticInfoCDO = GetStaticInfo();
		if (!StaticInfoCDO->IsA<T>())
		{
			UE_LOG(LogTemp, Error, TEXT("Tried to access static info of tool class %s as class %s, did not match!"), *GetClass()->GetName(), *T::StaticClass()->GetName());
			return GetDefault<T>();
		}
#endif

		return CastChecked<T>(GetStaticInfo());
	}

	// Used to get a non const pointer to the static info
	template <class T = UItemStaticInfo>
	T* GetStaticInfoMutable() const
	{
		return const_cast<T*>(GetStaticInfo<T>());
	}

	// ~Static Info

	// ~Item
	// Get the actual desired display name, should be used rather than accessing the static info since it might be modified by code
	UFUNCTION(BlueprintPure)
	virtual FText GetDisplayName() const;
	// Get the inventory sorting order (lower numbers first)
	virtual TTuple<int32, int32> GetSortOrder() const;
	// Sort function for inventory items
	bool SortInventoryItem(const AInventoryItem* Other) const;
	// Get the item type tags
	virtual void GetItemTags(FGameplayTagContainer& OutTags) const;
	// Get the price for this item
	UFUNCTION(BlueprintPure)
	virtual float GetItemPrice() const;
	// Returns the owning character, or null
	UFUNCTION(BlueprintPure)
	ABrickCharacter* GetCharacter() const;
	// Get the sub inventory of this item
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const;
	// Gets the outer inventory, if this item is being held by one
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetOwningInventory() const;
	// Return whether this item is equipped in first person view
	bool IsViewTarget() const;
	// Get the player controller that is viewing the character
	UFUNCTION(BlueprintPure)
	APlayerController* GetViewingPlayer() const;
	// Return true if the item is owned by a character that is locally controlled
	bool IsLocallyControlled() const;
	// Return the mesh component that is currently visible, either the static or skeletal mesh
	UMeshComponent* GetVisibleMesh() const;
	// Returns whether the item can ever be equipped
	bool CanBeEquipped() const;
	// Whether items of this type can be stacked in the inventory
	virtual bool CanBeStacked() const
	{
		return !ShouldReplicateInInventory();
	}

	// Sets whether the player currently wants to fire the item
	UFUNCTION(BlueprintNativeEvent)
	void SetIsFiring(bool bNewFiring);

	// Unified event used to release resources such as the skeletal mesh when it is probably no longer needed
	virtual void RemoveFirstPersonOnlyAssets();
	// Whether the item needs to be replicated while in an inventory
	virtual bool ShouldReplicateInInventory() const
	{
		return false;
	}

	// Creates the MID if needed and returns true if it can be used
	bool InitializeMID();

private:
	// Updates the material on both meshes
	void UpdateMaterial();
	// ~Item

public:
	// ~Thumbnail
	// Whether an MID needs to be created when rendering the item thumbnail
	UFUNCTION(BlueprintNativeEvent)
	bool NeedsThumbnailMID() const;
	// Allows the item to customize its thumbnail capture MID
	UFUNCTION(BlueprintNativeEvent)
	void UpdateThumbnailMID(UMaterialInstanceDynamic* InMID) const;
	// ~Thumbnail

	// ~Stackables
	// Set the number of items, server only
	UFUNCTION(BlueprintNativeEvent)
	void SetNumItems(int32 Num);
	// Get the number of items this actor represents, usually 1 but can be more for ammo boxes for example
	UFUNCTION(BlueprintNativeEvent)
	int32 GetNumItems() const;
	// Get the number of items this tool spawns with
	UFUNCTION(BlueprintNativeEvent)
	int32 GetNumDefaultItems() const;
	// ~Stackables

	// ~Animations
	// Play an animation on the mesh
	void PlayActionMontage(UAnimMontage* Montage, float PlayRate = 1.f);
	// Stop the animation currently being played and return to ref pose
	void StopActionMontage(UAnimMontage* Montage, float BlendTime);
	// ~Animations

	// ~Sounds
	// Helper function to play a handling sound on the character
	void PlayHandlingSound(USoundBase* Sound);
	// ~Sounds

	// ~Aim
	// Whether this item can be aimed
	virtual bool CanAim() const;
	// Gather the desired aim parameters, like the aim offset, eye distance, magnification etc.
	virtual void GetAimParams(FItemAimParams& OutParams) const;
	// Called from the character whenever the sight needs to be updated
	virtual void UpdateSight(float AimRatio);
	// Get the bullet spread radius of the item
	virtual float GetSpreadRadius() const;
	// ~Aim

	// ~Inventory
	// Whether this item can currently be picked up
	UFUNCTION(BlueprintPure, BlueprintNativeEvent)
	bool CanBePickedUp() const;
	// Return true if the item is dropped on the ground by itself and not part of any inventory
	UFUNCTION(BlueprintPure)
	bool IsDropped() const;
	// Called when the item is dropped, server and clients
	UFUNCTION(BlueprintNativeEvent)
	void OnDropped();
	// Called on the server when the item is dropped to initialize the physics state
	void DropItem(APawn* DroppedBy, const FVector& Location, const FQuat& Rotation, const FVector& Velocity, const FVector& AngularVelocity);
	// Called when the item is being transferred into an inventory, server and clients
	UFUNCTION(BlueprintNativeEvent)
	void OnPickedUp();
	// Called when the item is being equipped with the animation, server and clients
	UFUNCTION(BlueprintNativeEvent)
	void OnEquipped();
	// Called when the item is put out of the hands, either stored in the inventory or dropped, or the soldier died/passed out, server and clients
	UFUNCTION(BlueprintNativeEvent)
	void OnUnequipped();

protected:
	// Called when the owner was changed to pick up or drop the item
	void OnOwnerChanged();
	// Helper function that enables physics and collision when the item is dropped
	void InitDroppedPhysics();
	// Implement to get the desired class for the inventory component
	virtual UClass* GetInventoryComponentClass() const;
	// Called after inventory creation
	virtual void InitializeInventoryComponent();
	// Callbacks for the inventory
	virtual void OnInventoryModified();
	virtual void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event);
	// ~Inventory

	// ~Interaction
	// Should be called any time a condition that affects the interaction component visibility is changed, like IsDropped() and bHidden
	void UpdateInteractionComponent();
	// Get the desired location for the interaction icon
	FVector GetInteractionLocation() const;
	// Return the interaction options for this item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	// Pick up this item from interaction
	UFUNCTION()
	void Interact_PickUp(ABrickPlayerController* PC);
	// ~Interaction

	// Callback for when the mesh component enters physics sleeping
	UFUNCTION()
	void OnStaticMeshSleep(UPrimitiveComponent* Primitive, FName BoneName);
	// This function should be used to toggle between the static and skeletal mesh
	void SetShowSkeletalMesh(bool bShow);
	// Cancel loading of the skeletal mesh
	void CancelSkeletalMeshLoad();
	// Called when the item mesh was loaded
	void OnSkeletalMeshLoaded();
	// Internal version that takes an already loaded asset
	void OnSkeletalMeshLoaded(USkeletalMesh* LoadedMesh);

private:
	// Helper function to play a collision sound
	void PlayCollisionSound();
};
