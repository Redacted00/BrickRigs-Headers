// Copyright Fluppisoft, 2020

#pragma once

#include "Components/HUDIconComponent.h"
#include "CoreMinimal.h"
#include "World/StaticMeshProp.h"
#include "LoadoutContainer.generated.h"

/**
 * The loadout container can be used by players to buy items
 */
UCLASS(Abstract)
class BRICKRIGS_API ALoadoutContainer : public AStaticMeshProp
{
	GENERATED_BODY()

private:
	// ~Variables
	// Whether the inventory is currently accessed
	bool bIsAccessedByPlayer;
	// The current open/close anim position
	float AnimRatio;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UHUDIconComponent* HUDIconComponent;
	// ~Components

protected:
	// ~Properties
	// How long the lid open animation should take
	UPROPERTY(EditDefaultsOnly, Category = LoadoutContainer)
	float AnimationLength;
	UPROPERTY(EditDefaultsOnly, Category = LoadoutContainer)
	FHUDIconProperties IconProperties;
	// ~Properties

public:
	// ~Constructor
	ALoadoutContainer();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void Tick(float DeltaTime) override;
	// ~Super Interface

	// Called from the inventory component to update the accessed state
	void SetIsAccessedByPlayer(bool bNewAccessed);

protected:
	// Used to update the open animation
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAnimation(float InAnimRatio);

private:
	// Interaction component callbacks
	FVector GetInteractionLocation() const;
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	// Interaction callback
	UFUNCTION()
	void Interact_Inventory(ABrickPlayerController* PC);
	// Enables or disables ticking if needed
	void UpdateLoadoutContainerTickEnabled();
};
