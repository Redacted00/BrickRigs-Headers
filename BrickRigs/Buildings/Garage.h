// Copyright Fluppisoft, 2015

#pragma once

#include "Interaction/InteractionComponent.h"
#include "Components/HUDIconComponent.h"
#include "Game/SpawnPoint.h"
#include "Buildings/BrickBuilding.h"
#include "Garage.generated.h"

class ABrickVehicle;
class UBoxComponent;
class USceneComponent;
class UHUDIconWidget;

UCLASS(Abstract)
class BRICKRIGS_API AGarage : public ABrickBuilding, public ISpawnPointInterface
{
	GENERATED_BODY()

	// ~Components
	UPROPERTY(Transient)
	UHUDIconComponent* HUDIconComponent;
	UPROPERTY(Transient)
	UInteractionComponent* InteractionComponent;
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UBoxComponent* VehicleSpawnBox;
#endif
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* PlayerSpawnPoint;
	// ~Components

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Garage)
	FHUDIconProperties HUDIconProperties;
	UPROPERTY(EditAnywhere, Category = Garage)
	FVector VehicleSpawnExtent;
	UPROPERTY(EditAnywhere, Category = Garage)
	FVector VehicleSpawnOffset;
	UPROPERTY(EditAnywhere, Category = Garage)
	FRotator VehicleSpawnRotation;
	// ~Properties

public:
	// ~Constructor
	AGarage();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~Super Interface

	// ~Spawn
	// Whether the given location is within the spawn area
	bool IsPlayerInsideSpawnArea(const FVector& PlayerLocation, float Inflation) const;
	// Get the size of the spawn area
	FVector GetSpawnBoxExtent() const
	{
		return VehicleSpawnExtent;
	}

private:
	// Get the transform of the spawn box relative to the root component
	FTransform GetVehicleSpawnBoxRelativeTransform() const;
	// ~Spawn

	// ~HUD Icon
	FVector GetHUDIconLocation() const;
	void GetHUDIconInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	// ~HUD Icon

	// ~Interaction
	FVector GetInteractionLocation() const;
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	// Used both for the HUD icon and interaction icon
	void GetEnterInteractionOption(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	UFUNCTION()
	void Interact_SpawnVehicle(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_EnterEditor(ABrickPlayerController* PC);
	// Interaction callback
	UFUNCTION()
	void Interact_Inventory(ABrickPlayerController* PC);
	// ~Interaction

public:
	// ~ISpawnPointInterface
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const override;
	virtual bool GetSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const override;
	virtual bool CanPlayerSpawnVehicle(ABrickPlayerController* PC) const override;
	virtual bool GetVehicleSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const override;
	virtual TTuple<FVector, FVector> GetVehicleSpawnBounds() const override;
	// ~ISpawnPointInterface
};
