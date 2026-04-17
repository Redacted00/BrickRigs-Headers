#pragma once

#include "BonePose.h"
#include "Animation/AnimSequenceBase.h"
#include "CoreMinimal.h"

namespace FAnimationHelpers
{
#if WITH_EDITOR
	FORCEINLINE void GetBoneTransforms(UAnimSequenceBase* Sequence, float Time, TMap<FName, FTransform>& OutBoneTransforms)
	{
		if (Sequence && Sequence->GetSkeleton())
		{
			const FReferenceSkeleton& RefSkeleton = Sequence->GetSkeleton()->GetReferenceSkeleton();

			// Build the bone index array
			TArray<FBoneIndexType> BoneIndices;
			BoneIndices.SetNumUninitialized(RefSkeleton.GetRawBoneNum());
			for (int32 i = 0; i < BoneIndices.Num(); ++i)
			{
				BoneIndices[i] = i;
			}

			FBoneContainer RequiredBones;
			RequiredBones.SetUseRAWData(true);
			RequiredBones.InitializeTo(BoneIndices, FCurveEvaluationOption(false), *Sequence->GetSkeleton());

			FCompactPose Pose;
			Pose.SetBoneContainer(&RequiredBones);
			FBlendedCurve Curve;
			FStackCustomAttributes Attributes;
			FAnimationPoseData PoseData(Pose, Curve, Attributes);
			Sequence->GetAnimationPose(PoseData, FAnimExtractContext(Time, false));

			for (auto& Pair : OutBoneTransforms)
			{
				const FCompactPoseBoneIndex BoneIndex = FCompactPoseBoneIndex(RefSkeleton.FindBoneIndex(Pair.Key));
				if (Pose.IsValidIndex(BoneIndex))
				{
					Pair.Value = Pose[BoneIndex];
				}

				// Convert the pose to component space
				for (int32 ParentIdx = RefSkeleton.GetParentIndex(BoneIndex.GetInt()); ParentIdx != INDEX_NONE; ParentIdx = RefSkeleton.GetParentIndex(ParentIdx))
				{
					Pair.Value = Pair.Value * Pose[FCompactPoseBoneIndex(ParentIdx)];
				}
			}
		}
	}

	// Version that only takes one bone
	FORCEINLINE FTransform GetBoneTransform(UAnimSequenceBase* Sequence, float Time, const FName& BoneName)
	{
		TMap<FName, FTransform> BoneTransforms;
		FTransform& OutTransform = BoneTransforms.Add(BoneName, FTransform::Identity);
		GetBoneTransforms(Sequence, Time, BoneTransforms);
		return OutTransform;
	}
#endif
}
