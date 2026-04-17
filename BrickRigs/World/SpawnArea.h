// Copyright Fluppisoft, 2017

#pragma once

#include "Components/HUDIconComponent.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "Game/SpawnPoint.h"
#include "GameFramework/Actor.h"
#include "SpawnArea.generated.h"

class ABrickPlayerStart;

UCLASS(Abstract, Hidecategories = (Collision, Tags, Cooking, Actor, Mobile, Input, LOD, Replication, Rendering, "Actor Tick"))
class BRICKRIGS_API ASpawnArea : public AActor, public ISpawnPointInterface
{
	GENERATED_BODY()

protected:
	// ~Components
	// Used to display the spawn point on the UI
	UPROPERTY(Transient)
	UHUDIconComponent* HUDIconComponent;
	// ~Components

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = SpawnArea)
	FHUDIconProperties HUDIconProperties;
	// ~Properties

public:
	// ~Constructor
	ASpawnArea();

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// ~Super Interface

protected:
	// HUD icon component callbacks
	virtual bool ShouldIconBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const;
	virtual FVector GetIconLocation() const;

public:
	// ~ISpawnPointInterface
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const override;
	virtual bool GetSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const override;
	// ~ISpawnPointInterface
};
