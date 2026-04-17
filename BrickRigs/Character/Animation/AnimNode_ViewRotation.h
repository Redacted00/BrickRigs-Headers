// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BoneContainer.h"
#include "BonePose.h"
#include "Animation/AnimNodeBase.h"
#include "AnimNode_ViewRotation.generated.h"

/**
 *
 */
USTRUCT(BlueprintInternalUseOnly)
struct BRICKRIGS_API FAnimNode_ViewRotation : public FAnimNode_Base
{
	GENERATED_BODY()

private:
	// ~Variables
	// Relative rotation of the camera socket
	FQuat CameraSocketRotation;
	// ~Variables

public:
	// The input pose
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Links)
	FPoseLink InPose;
	// World space target view rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	FRotator ViewRotation = FRotator::ZeroRotator;
	// How much the arms should follow the view rotation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	float ArmsAlpha = 0.f;
	// Whether the root should be moved back and forth with view pitch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	bool bUseRootPitchAdjustement = false;
	// How much the root bone should be moved back or forth depending on view pitch degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings, meta = (PinShownByDefault))
	float RootPitchAdjustmentRate = 0.25f;

	// ~Properties
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference RootBone;
	UPROPERTY(EditAnywhere, Category = Config)
	FBoneReference HeadBone;
	UPROPERTY(EditAnywhere, Category = Config)
	TArray<FBoneReference> NeckBoneChain;
	UPROPERTY(EditAnywhere, Category = Config)
	TArray<FBoneReference> ArmBones;
	// ~Properties

	// ~Constructor
	FAnimNode_ViewRotation();

	// ~Super Interface
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context) override;
	virtual void Update_AnyThread(const FAnimationUpdateContext& Context) override;
	virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	// ~Super Interface
};
