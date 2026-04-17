// Copyright Fluppisoft, 2020

#pragma once

#include "Animation/AnimNodeBase.h"
#include "Animation/AnimInstanceProxy.h"
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ItemAnimInstance.generated.h"

class AInventoryItem;

// Used for generically overriding tool ref poses, for flipped iron sights for example
USTRUCT(BlueprintType)
struct FItemRefPoseOverride
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName BoneName;
	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;
};

struct FItemBoneModifier
{
	FItemBoneModifier()
		: BoneRef(NAME_None), Transform(FTransform::Identity)
	{
	}

	FItemBoneModifier(const FName& InBoneName, const FTransform& InTransform = FTransform::Identity)
		: BoneRef(InBoneName), Transform(InTransform)
	{
	}

	FORCEINLINE void Initialize(const FBoneContainer& RequiredBones)
	{
		BoneRef.Initialize(RequiredBones);
	}

	FBoneReference BoneRef;
	FTransform Transform;
};

USTRUCT()
struct FItemAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()
	;

private:
	// ~Variables
	FSlotNodeWeightInfo DefaultSlotWeightData;

public:
	TArray<FItemBoneModifier> BoneModifiers;
	// ~Variables

	// ~Constructor
	FItemAnimInstanceProxy()
	{
	}

	FItemAnimInstanceProxy(UAnimInstance* Instance);

	// ~Super Interface
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual bool Evaluate(FPoseContext& Output) override;
	// ~Super Interface

	// Return the associated item
	AInventoryItem* GetItem() const;

protected:
	// Helper function to convert ref pose overrides to tool bone modifiers
	void InitializeBoneModifiers(const TArray<FItemRefPoseOverride>& BoneOverrides, TArray<FItemBoneModifier>& OutModifiers);
	// Helper function to initialize the bone references for bone modifiers
	void CacheBoneModifiers(TArray<FItemBoneModifier>& OutModifiers);
	// Helper function to apply bone modifiers to the pose
	void ApplyBoneModifiers(FPoseContext& Output, const TArray<FItemBoneModifier>& Modifiers);
	// Apply a single bone modifier
	bool ApplyBoneModifier(FPoseContext& Output, const FItemBoneModifier& Modifier);
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UItemAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	// ~Super Interface

	// Return the owning item
	AInventoryItem* GetItem() const;
};
