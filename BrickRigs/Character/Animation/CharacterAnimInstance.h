// Copyright Fluppisoft, 2016

#pragma once

#include "Animation/AnimInstanceProxy.h"
#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FCharacterAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

protected:
	struct FFlinchBoneInfo
	{
		FBoneReference BoneRef;
		float FlinchRatio;
		FQuat StartRotation;
		FQuat TargetRotation;

		FQuat GetCurrentRotation() const
		{
			const auto PeakRatio = 0.25f;
			if (FlinchRatio < PeakRatio)
			{
				const auto LerpValue = FMath::GetMappedRangeValueUnclamped(FVector2D(0.f, PeakRatio), FVector2D(0.f, 1.f), FlinchRatio);
				return FQuat::Slerp(StartRotation, TargetRotation, LerpValue);
			}

			const auto LerpValue = FMath::GetMappedRangeValueUnclamped(FVector2D(PeakRatio, 1.f), FVector2D(0.f, 1.f), FlinchRatio);
			return FQuat::Slerp(TargetRotation, FQuat::Identity, LerpValue);
		}
	};

	// ~Variables
	// Cached bone index of our root bone
	FBoneReference RootBoneReference;
	// Rotation of the character in world space
	FRotator CharacterRotation = FRotator::ZeroRotator;

	// World space velocity
	FVector Velocity = FVector::ZeroVector;
	// Velocity relative to the character rotation
	FVector RelativeVelocity = FVector::ZeroVector;
	// Cached blending speeds of the sprint animation
	float SprintBlendInSpeed = 0.f;
	float SprintBlendOutSpeed = 0.f;

	// View limits, copied over from the static info
	FFloatInterval ViewPitchRange;
	FFloatInterval ViewYawRange;

	// List of our currently flinching bones
	TArray<FFlinchBoneInfo> FlinchingBones;
	// Duration of the flinch animation
	float FlinchAnimLength = 0.f;

public:
	// ~Misc
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator ViewRotation = FRotator::ZeroRotator;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsViewTarget = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsConscious = false;
	// ~Misc

	// ~Movement
	// The movement direction to be used with the speed
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float MovementDirection = 0.f;
	// The current movement speed of the character
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float Speed = 0.f;
	// The current max movement speed
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float MaxSpeed = 0.f;
	// Percentage of walk speed the character is moving at
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float WalkSpeedRatio = 0.f;
	// Percentage of crouch speed the character is moving at
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float CrouchSpeedRatio = 0.f;
	// Percentage how much the player is sprinting
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float SprintRatio = 0.f;
	// Rotation offset of the lower body relative to the upper body
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float BodyRotationOffset = 0.f;
	// Whether the character is moving faster than the standing threshold
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsMoving = false;
	// Whether the character is sprinting right now
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsSprinting = false;
	// Whether the character is crouched
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsCrouched = false;
	// Whether the body is rotating in place right now
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsBodyRotating = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsMovingOrRotating = false;
	// Whether the character is currently walking on the ground, used instead of the enum to enable fast blueprint path
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsWalking = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsFalling = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsSwimming = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FTransform RootTransform;
	// ~Movement
	// ~Variables

	// ~Constructor
	FCharacterAnimInstanceProxy()
	{
	}

	FCharacterAnimInstanceProxy(UAnimInstance* Instance);
	virtual ~FCharacterAnimInstanceProxy() override;

	// ~Super Interface
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void CacheBones() override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	virtual bool Evaluate(FPoseContext& Output) override;
	// ~Super Interface
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
};
