// Copyright Fluppisoft, 2021

#pragma once

#include "ProjectileManagerComponent.h"
#include "CoreMinimal.h"
#include "StaticMeshProp.h"
#include "Physics/ContactModifyInterface.h"
#include "FuelTank.generated.h"

class UStaticMeshComponent;
class UExplosiveMaterial;

UCLASS(Abstract)
class BRICKRIGS_API AFuelTank : public AStaticMeshProp, public IProjectileImpactInterface, public IContactModifyInterface
{
	GENERATED_BODY()

	// ~Variables
	// The total damage received so far
	float TotalDamage;
	// Whether the tank has exploded yet
	UPROPERTY(Transient, ReplicatedUsing = OnRep_bHasExploded)
	bool bHasExploded;
	UFUNCTION()
	void OnRep_bHasExploded();
	// ~Variables

protected:
	// ~Properties
	// Maximum damage at which the tank explodes
	UPROPERTY(EditAnywhere, Category = FuelTank)
	float MaxDamage;
	// Mesh to be used when the tank has exploded
	UPROPERTY(EditAnywhere, Category = FuelTank)
	UStaticMesh* ExplodedStaticMesh;
	// The type of fuel in the tank
	UPROPERTY(EditAnywhere, Category = FuelTank)
	TSubclassOf<UExplosiveMaterial> FuelType;
	// Volume of the fuel in liters
	UPROPERTY(EditAnywhere, Category = FuelTank)
	float FuelVolume;
	// Emitter to spawn on leaks
	UPROPERTY(EditAnywhere, Category = FuelTank)
	UParticleSystem* LeakEmitter;
	// ~Properties

public:
	// ~Constructor
	AFuelTank();

	// ~Super Interface
	virtual void Reset() override;
	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	bool ShouldSpawnLeakOnHit(const FHitResult& Hit);
	// ~Blueprint Interface

private:
	void SetHasExploded(bool bNewExploded);
	void OnHasExplodedChanged();
	void RemoveImpactEffects();

	// ~IProjectileImpactInterface
	virtual void OverrideImpactEffects(const FHitResult& Hit, FSurfaceTypeEffects& InOutEffects) override;
	// ~IProjectileImpactInterface

	// ~ICollisionDamageInterface
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx) const override;
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx) override;
	// ~ICollisionDamageInterface
};
