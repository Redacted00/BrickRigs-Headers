// Copyright Fluppisoft, 2020

#pragma once

#include "BoneContainer.h"
#include "BonePose.h"
#include "Animation/AnimTypes.h"
#include "Animation/AnimNodeBase.h"
#include "Animation/AnimData/BoneMaskFilter.h"
#include "CoreMinimal.h"
#include "AnimNode_ItemBlend.generated.h"

struct FBrickCharacterAnimInstanceProxy;

USTRUCT(BlueprintInternalUseOnly)
struct BRICKRIGS_API FAnimNode_ItemBlend : public FAnimNode_Base
{
	GENERATED_BODY()

private:
	// ~Variables
	// Weights for an entire upper body blend
	TArray<FPerBoneBlendWeight> UpperBodyLayeredBlendWeights;
	// Same as upper body but excludes the left arm, for items that are held with one arm only
	//TArray<FPerBoneBlendWeight>	RightArmLayeredBlendWeights;
	// Filter for only the hands, so they can keep their stance while vaulting etc.
	TArray<FPerBoneBlendWeight> HandLayeredBlendWeights;
	// Current weights to apply
	TArray<FPerBoneBlendWeight> CurrentBoneBlendWeights;
	// Used to blend to body pose if the character is moving while being prone or playing a full body anim montage
	float FullBodyAnimWeight = 0.f;
	// The camera socket transform in parent bone space
	FTransform CameraSocketTransform;
	// ~Variables

public:
	// ~Properties
	// The base character pose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FPoseLink BasePose;
	// The item pose to blend
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FPoseLink ItemPose;

	// Bone references
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference LeftHandBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference LeftElbowBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference RightHandBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference RightElbowBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference HeadBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference ItemBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference LeftClavicleBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference RightClavicleBone;

	// Used to customize the aim in and out transition
	UPROPERTY(EditAnywhere, Category = Config)
	float AimCurveExponent = 2.f;
	UPROPERTY(EditAnywhere, Category = Config)
	FVector AimTransitionLocationOffset = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = Config)
	FRotator AimTransitionRotationOffset = FRotator::ZeroRotator;
	// ~Properties

	// ~Super Interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	// ~Super Interface

private:
	// Returns the anim instance, but only if the type actually matches
	const FBrickCharacterAnimInstanceProxy* GetCharacterAnimInstance(FAnimInstanceProxy* AnimInstanceProxy) const;
	// Update the blend weights
	void UpdateBoneBlendWeights(FAnimInstanceProxy* AnimInstanceProxy);
	// Rebuild the blend filters, should be done when caching bones
	void RebuildBlendFilters(FAnimInstanceProxy* AnimInstanceProxy);

	// Calculates the IK setup for both hands
	void UpdateItemIK(const FBrickCharacterAnimInstanceProxy& AnimInstanceProxy, FPoseContext& Output, const FPoseContext& BasePoseContext, const FPoseContext& ToolPoseContext,
	                  FTransform& OutLeftHandTarget, FTransform& OutRightHandTarget,
	                  FTransform& OutLeftJointTarget, FTransform& OutRightJointTarget);
	// Helper function to apply the IK target
	void UpdateHandIK(FComponentSpacePoseContext& Output, const FCompactPoseBoneIndex& BoneIndex, const FTransform& TargetTransform, const FVector& JointTargetLocation, float InBlendWeight);
};
