// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "Items/Animation/ItemAnimInstance.h"
#include "Items/Animation/ItemAnimation.h"
#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemStaticInfo.generated.h"

// Assets
class UAnimInstance;
class UAnimSequenceBase;
class USoundBase;
class UStaticMesh;
class USkeletalMesh;
class UPhysicalMaterial;

// Used to identify the inventory slot an item has to be put into
// NOTE: The order also determines the sorting order in an inventory
UENUM(BlueprintType)
enum class EItemTag : uint8
{
	None,
	Weapon,
	Firearm,
	Attachment,
	AmmoBox,
	Explosive,
	Grenade,
	FirstAidKit,
	FireExtinguisher,
	Paint,
	Wearable,
};

// Used to share these parameters between function ca��s
USTRUCT(BlueprintType)
struct FItemAimParams
{
	GENERATED_BODY()

	FTransform SightOffset;
	UPROPERTY(EditAnywhere)
	float EyeDistance = 0.f;
	UPROPERTY(EditAnywhere)
	float MinEyeDistance = 0.f;
	UPROPERTY(EditAnywhere)
	float Magnification = 0.f;
};

/**
 *
 */
UCLASS(Blueprintable, Abstract)
class BRICKRIGS_API UItemStaticInfo : public UObject
{
	GENERATED_BODY()

	// ~Variables
	// Cached component space transform of the item bone in the idle sequence
	UPROPERTY()
	FTransform ItemIdleTransform;
	// ~Variables
public:
	// ~Properties
	// The main static mesh for this item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMesh* StaticMesh;
	// The optional skeletal mesh for this item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;
	// Base material override for the static and skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UMaterialInterface* MaterialOverride;
	// An optional alternative mesh used to render the thumbnail
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMesh* ThumbnailStaticMesh;
	// Max distance to draw the mesh from
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	float MaxDrawDistance;
	// Anim instance class for the first person skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	TSubclassOf<UAnimInstance> AnimInstanceClass;
	// Physical material to use on the item
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	UPhysicalMaterial* PhysicalMaterial;
	// The display name of this item to be shown in the UI
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	FText DisplayName;
	// Whether the item can ever be equipped
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	bool bCanBeEquipped;
	// Determined from the item tags added through C++
	int32 ItemTypeSortOrder;
	// Can be used to customize item sorting within the same categoriy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	int32 SortOrder;
	// Item type tags
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FGameplayTagContainer ItemTags;
	// Inventory slot tags
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FGameplayTagContainer SlotTags;
	// Cost for this item at loadout containers
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	float Price;
	// Used for zoom, eye distance etc
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Item)
	FItemAimParams AimParams;
	// Relative location to apply when attached to the character socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	FVector CharacterSocketRelativeLoc;
	// Relative rotation to apply when attached to the character socket
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Mesh)
	FRotator CharacterSocketRelativeRot;

	// ~Sounds
	// Sound played when the item collides with anything
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds)
	USoundBase* CollisionSound;
	// ~Sounds

	// ~Animations
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	UAnimSequenceBase* IdleSequence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	UAnimSequenceBase* SprintSequence;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	FEquipAnimation EquipAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animations)
	FUnequipAnimation UnequipAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TechnicalAnimations)
	FTransform IdlePoseOffsetFP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = TechnicalAnimations)
	FTransform IdlePoseOffsetTP;
	// ~Animations
	// ~Properties

	// ~Constructor
	UItemStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
#endif
	// ~Super Interface

protected:
#if WITH_EDITOR
	// Called from a delegate when PIE is started or ended
	virtual void OnBeginPIE(bool bIsSimulating);
	virtual void OnEndPIE(bool bIsSimulating);
	// Used to calculate the cached idle transform
	void CalcItemBoneIdleTransform();
#endif

public:
	// Used to load animations as the game is initialized
	void LoadStartupAnimations();
	// Used to load animations when the item is picked up
	void LoadItemAnimations();
	// Called by items when they have loaded the skeletal mesh asset
	virtual void OnSkeletalMeshLoaded(USkeletalMesh* SkeletalMeshPtr);
	// Returns the cached transform of the item bone in the idle pose
	FTransform GetItemBoneIdleTransform();

protected:
	// ~Tags
	// Helper to add an enum tag
	void AddItemTag(EItemTag InTag);
	// ~Tags
};
