// Copyright Fluppisoft, 2016

#pragma once

#include "GameFramework/Actor.h"
#include "Explosion.generated.h"

class UParticleSystemComponent;
class UAudioComponent;
class UCameraShakeBase;
class UExplosiveMaterial;

UCLASS(Abstract)
class BRICKRIGS_API AExplosion : public AActor
{
	GENERATED_BODY()

	// ~Variables
	FTimerHandle TimerHandle_ApplyDamage;
	// The type of explosive material used
	UPROPERTY(Transient, Replicated)
	TSubclassOf<UExplosiveMaterial> ExplosiveMaterial;
	// The amount of explosive material used
	UPROPERTY(Transient, Replicated)
	float ExplosiveVolume;
	// Components to ignore when tracing for obstacles
	UPROPERTY(Transient, Replicated)
	TArray<UPrimitiveComponent*> ComponentsToIgnore;
	// The actor that spawned this explosion
	UPROPERTY(Transient)
	AActor* DamageCauser;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(Transient)
	UAudioComponent* AudioComponent;
	// ~Components

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Explosion)
	TSubclassOf<UCameraShakeBase> CameraShakeEffect;
	// ~Properties

public:
	// ~Constructor
	AExplosion();

	// ~Super Interface
	virtual void BeginPlay() override;
	// ~Super Interface

	// Initializes the explosion parameters
	void InitializeExplosion(const TSubclassOf<UExplosiveMaterial>& InMaterial, float InVolume, AActor* InDamageCauser, const TArray<UPrimitiveComponent*>& InComponentsToIgnore);

private:
	// Applies the explosion damage, server and clients
	void ApplyDamage();
};
