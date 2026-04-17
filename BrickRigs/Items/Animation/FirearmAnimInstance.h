// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "ItemAnimInstance.h"
#include "FirearmAnimInstance.generated.h"

USTRUCT()
struct FFirearmAnimInstanceProxy : public FItemAnimInstanceProxy
{
	GENERATED_BODY()
	;

private:
	// ~Variables
	FSlotNodeWeightInfo BoltSlotWeightData;
	// Whether the firearm has a sight, in that case the iron sights should be flipped
	bool bHasSight;
	// Whether the firearm is cocked
	bool bIsCocked;
	// Bone modifiers for the sight attached pose
	TArray<FItemBoneModifier> SightBoneModifiers;
	// Bone modifiers applied when the bolt is locked
	TArray<FItemBoneModifier> BoltLockedBoneModifiers;
	// Modifier for the hammer bone (when it is cocked)
	FItemBoneModifier HammerBoneModifier;
	// Whether the super anim should be evaluated
	bool bSuperAnimRelevant;
	// ~Variables
public:
	// ~Constructor
	FFirearmAnimInstanceProxy()
	{
	}

	FFirearmAnimInstanceProxy(UAnimInstance* Instance);

	// ~Super Interface
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void CacheBones() override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual bool Evaluate(FPoseContext& Output) override;
	// ~Super Interface

private:
	// Return whether the super anim graph should be updated/evaluated
	bool IsSuperAnimRelevant() const;
	// Helper function to initialize a slot node
	void InitAnimSlot(const FName& SlotName, FSlotNodeWeightInfo& WeightData);
	// Helper function to update a slot node
	void UpdateAnimSlot(const FName& SlotName, FSlotNodeWeightInfo& WeightData);
	// Helper function to evaluate an anim slot node
	void EvaluateAnimSlot(FPoseContext& Output, const FName& SlotName, const FSlotNodeWeightInfo& WeightData, const TArray<FCompactPoseBoneIndex>& BonesToIgnore);
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UFirearmAnimInstance : public UItemAnimInstance
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	// ~Super Interface
};
