// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Character/Animation/CharacterAnimInstance.h"
#include "ZombieAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FZombieAnimInstanceProxy : public FCharacterAnimInstanceProxy
{
	GENERATED_BODY()
	;

public:
	// ~Constructor
	FZombieAnimInstanceProxy()
	{
	}

	FZombieAnimInstanceProxy(UAnimInstance* Instance);
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UZombieAnimInstance : public UCharacterAnimInstance
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(Transient, EditDefaultsOnly, BlueprintReadOnly, Category = Character)
	FZombieAnimInstanceProxy Proxy;
	// ~Properties

	// ~Constructor
	UZombieAnimInstance();

	// ~Super Interface
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;
	// ~Super Interface
};
