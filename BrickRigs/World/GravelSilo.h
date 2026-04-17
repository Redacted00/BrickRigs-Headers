// Copyright Fluppisoft, 2016

#pragma once

#include "Interaction/InteractionComponent.h"
#include "StaticMeshProp.h"
#include "GravelSilo.generated.h"

class UStaticMeshComponent;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API AGravelSilo : public AStaticMeshProp
{
	GENERATED_BODY()

private:
	// ~Variables
	// Used to delay spawning the next particle
	FTimerHandle TimerHandle_SpawnParticle;
	// Whether the silo is currently open
	UPROPERTY(ReplicatedUsing = OnRep_bIsSiloOpen)
	bool bIsSiloOpen;
	UFUNCTION()
	void OnRep_bIsSiloOpen();
	// ~Variables

	// ~Components
	// Used for the interactive opening point
	UPROPERTY(Transient)
	UInteractionComponent* InteractionComponent;
	// Particle emitter used for the silo opening
	UPROPERTY(Transient)
	UParticleSystemComponent* OutletPSC;
	// List of particles that have been created so far
	UPROPERTY(Transient)
	TArray<UStaticMeshComponent*> ParticleComponents;
	// ~Components

protected:
	// ~Properties
	// Particle system to use for the outlet effect
	UPROPERTY(EditAnywhere, Category = Silo)
	UParticleSystem* OutletEmitter;
	// Mesh to use for the particles
	UPROPERTY(EditAnywhere, Category = Silo)
	UStaticMesh* ParticleMesh;
	// Number of particles to spawn per second
	UPROPERTY(EditAnywhere, Category = Silo)
	float ParticlesPerSecond;
	// Maximum spawn location deviation
	UPROPERTY(EditAnywhere, Category = Silo)
	float MaxOffset;
	// Minimum initial velocity
	UPROPERTY(EditAnywhere, Category = Silo)
	FVector MinVelocity;
	// Maximum initial velocity
	UPROPERTY(EditAnywhere, Category = Silo)
	FVector MaxVelocity;
	// Min and max random scale of particles
	UPROPERTY(EditAnywhere, Category = Silo)
	FFloatInterval ParticleScale;
	// ~Properties

public:
	// ~Constructor
	AGravelSilo();

	// ~Super Interface
	virtual void BeginPlay() override;
	// ~Super Interface

	// Opens or closes the silo
	void SetIsSiloOpen(bool bNewOpen);

private:
	// Called when the silo has been opened or closed
	void OnIsSiloOpenChanged();
	// Spawns a single particle and sets up the timer for the next one
	void SpawnParticle();

	// Interaction component callbacks
	FVector GetInteractionLocation() const;
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	UFUNCTION()
	void Interact_OpenSilo(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_CloseSilo(ABrickPlayerController* PC);
};
