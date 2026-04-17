// Copyright Fluppisoft, 2020

#pragma once

#include "Items/StaticInfo/ItemStaticInfo.h"
#include "Character/CharacterStaticInfo.h"
#include "CoreMinimal.h"
#include "CharacterAnimInstance.h"
#include "BrickCharacterAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FBrickCharacterAnimInstanceProxy : public FCharacterAnimInstanceProxy
{
	GENERATED_BODY()

	// ~Variables
	// Cached path of the current item, used to keep track of changes
	// NOTE: Don't use a pointer, since if the item is destroyed the pointer would be cleared and we wouldn't know it changed
	FSoftObjectPath CurrentItemPath;

	// Cached transform of the right hand in the idle pose
	FTransform ItemIdleTransform;
	// Cached walk sway animation parameters
	FWalkSwayParams WalkSwayParams;
	// Cached sprint ratio of last frame
	float LastItemSprintRatio = 0.f;
	// Current footstep state
	float FootstepCycle = 0.f;
	// ~Variables

	// ~Properties
	// ~Misc
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float AimRatio = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float CameraDepthOffset = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float ArmsViewRotationAlpha = 0.f;
	// ~Misc

	// ~Item
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float ItemBlendRatio = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FItemAimParams ItemAimParams;
	// Sway of the item due to the inability to hold it perfectly still
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator ItemSwayRotation = FRotator::ZeroRotator;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector WalkSwayOffset = FVector::ZeroVector;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator WalkSwayRotation = FRotator::ZeroRotator;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float WeaponSupportRatio = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FTransform ItemIdlePoseOffset;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float ItemSprintRatio = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsWeaponBlocked = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsCarryingBrick = false;
	// ~Item

	// ~Vehicle
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	bool bIsInVehicle = false;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	UAnimSequence* VehicleIdleSequence = nullptr;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	UAnimInstance* VehicleAnimInstance = nullptr;
	// ~Vehicle

	// ~Recoil
	// Current recoil location and rotation offset
	FVector ItemRecoilOffset = FVector::ZeroVector;
	FRotator ItemRecoilRotation = FRotator::ZeroRotator;
	// ~Recoil

	// ~Hands IK
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	float HandsIKAlpha = 0.f;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector LeftHandIKOffset = FVector::ZeroVector;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FVector RightHandIKOffset = FVector::ZeroVector;
	UPROPERTY(Transient, EditAnywhere, BlueprintReadWrite, Category = Character)
	FRotator HandsRotationOffset = FRotator::ZeroRotator;
	// ~Hands IK

	// ~Animations
	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimSequenceBase* ItemIdleSequence = nullptr;
	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = Animations)
	UAnimSequenceBase* ItemSprintSequence = nullptr;
	// ~Animations
	// ~Properties

	// ~Constructor
	FBrickCharacterAnimInstanceProxy()
	{
	}

	FBrickCharacterAnimInstanceProxy(UAnimInstance* Instance);

	// ~Super Interface
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual void Update(float DeltaSeconds) override;
	// ~Super Interface

private:
	// Update the walk sway animation
	void UpdateWalkSway(float DeltaSeconds);
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickCharacterAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	FBrickCharacterAnimInstanceProxy Proxy;
	// ~Properties

	// ~Constructor
	UBrickCharacterAnimInstance();

	// ~Super Interface
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;
	// ~Super Interface
};
