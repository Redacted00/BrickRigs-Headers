// Copyright Fluppisoft, 2021

#pragma once

#include "World/ExplosiveMaterial.h"
#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "ExplosiveItem.generated.h"

UENUM()
enum class EExplosiveItemState : uint8
{
	Safe,
	Primed,
	Exploded
};

UCLASS(Abstract)
class BRICKRIGS_API UExplosiveItemStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The type of explosive substance used
	UPROPERTY(EditAnywhere, Category = Explosive)
	TSubclassOf<UExplosiveMaterial> ExplosiveMaterial;
	// How much explosive material there is in the item
	UPROPERTY(EditAnywhere, Category = Explosive)
	float ExplosiveVolume;
	// Min damage required to blow up the explosive
	UPROPERTY(EditDefaultsOnly, Category = Explosive)
	float MinExplosionDamage;
	// Whether the explosive can be defused again after placing it
	UPROPERTY(EditAnywhere, Category = Explosive)
	bool bCanBeDefused;

	// ~Animations
	UPROPERTY(EditDefaultsOnly, Category = Animations)
	FThrowAnimation ThrowAnimation;
	// ~Animations
	// ~Properties

	// ~Constructor
	UExplosiveItemStaticInfo();
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AExplosiveItem : public AInventoryItem
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Indicates whether this explosive is primed, has exploded yet etc.
	UPROPERTY(ReplicatedUsing = OnRep_ExplosiveState)
	EExplosiveItemState ExplosiveState;
	UFUNCTION()
	void OnRep_ExplosiveState();
	// Number of items stacked
	int32 NumExplosives;
	// ~Variables

public:
	// ~Super Interface
	virtual FText GetDisplayName() const override;
	virtual bool CanBePickedUp_Implementation() const override;
	virtual void SetIsFiring_Implementation(bool bNewFiring) override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool ShouldReplicateInInventory() const override;
	virtual bool CanBeStacked() const override;
	virtual void OnPickedUp_Implementation() override;

	virtual void SetNumItems_Implementation(int32 Num) override
	{
		NumExplosives = Num;
	}

	virtual int32 GetNumItems_Implementation() const override
	{
		return NumExplosives;
	}

	// ~Super Interface

	// ~Constructor
	AExplosiveItem()
		: NumExplosives(1)
	{
	}

	// Prime this explosive and make it ready for triggering
	UFUNCTION(BlueprintCallable)
	void PrimeExplosive();
	// Return whether the explosive is already primed
	UFUNCTION(BlueprintPure)
	bool IsPrimed() const;
	// Blow up this explosive, server only
	UFUNCTION(BlueprintCallable)
	void Explode(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	// Version that uses GetInstigatorController()
	void Explode()
	{
		Explode(-1.f, FDamageEvent(), GetInstigatorController(), nullptr);
	}

protected:
	// Set the current state of the explosive, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetExplosiveState(EExplosiveItemState NewState);
	// Called whenever the current state of the explosive has changed
	virtual void OnExplosiveStateChanged();
};
