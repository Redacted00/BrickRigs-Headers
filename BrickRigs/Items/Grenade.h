// Copyright Fluppisoft, 2021

#pragma once

#include "Animation/ItemAnimInstance.h"
#include "CoreMinimal.h"
#include "Items/ExplosiveItem.h"
#include "Grenade.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UGrenadeStaticInfo : public UExplosiveItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// After how many seconds the grenade explodes after being armed
	UPROPERTY(EditAnywhere, Category = Grenade)
	float FuseDelay;
	// ~Properties

	// ~Constructor
	UGrenadeStaticInfo();
};

USTRUCT()
struct FGrenadeAnimInstanceProxy : public FItemAnimInstanceProxy
{
	GENERATED_BODY()
	;

private:
	// ~Variables
	// Whether the grenade is currently armed
	bool bIsArmed;
	// Modifier for the pin bone (when it's pulled)
	FItemBoneModifier PinBoneModifier;
	// ~Variables
public:
	// ~Constructor
	FGrenadeAnimInstanceProxy()
	{
	}

	FGrenadeAnimInstanceProxy(UAnimInstance* Instance)
		: Super(Instance)
	{
	}

	// ~Super Interface
	virtual void Initialize(UAnimInstance* InAnimInstance) override;
	virtual void CacheBones() override;
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;
	virtual bool Evaluate(FPoseContext& Output) override;
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UGrenadeAnimInstance : public UItemAnimInstance
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override
	{
		return new FGrenadeAnimInstanceProxy(this);
	}

	// ~Super Interface
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AGrenade : public AExplosiveItem
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Timer handle used to delay the fuse
	FTimerHandle TimerHandle_Fuse;
	// ~Variables

public:
	// ~Constructor
	AGrenade();

	// ~Super Interface
	virtual void OnExplosiveStateChanged() override;
	// ~Super Interface

protected:
	// Callback for the fuse timer
	void OnFuseTimer();
};
