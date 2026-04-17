// Copyright Fluppisoft, 2022

#pragma once

#include "Components/FireExtinguisherComponent.h"
#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "FireExtinguisher.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UFireExtinguisherStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = FireExtinguisher)
	FFireExtinguisherProperties ExtinguisherProperties;
	// Distance between the camera and the start of the extinguish shape
	UPROPERTY(EditAnywhere, Category = FireExtinguisher)
	float ExtinguishStartDistance = 50.f;

	// ~Animations
	UPROPERTY(EditDefaultsOnly, Category = Animations)
		FItemAnimation ExtinguishAnimation;
	// ~Animations
	// ~Properties

	// ~Constructor
	UFireExtinguisherStaticInfo();
};


/**
 * 
 */
UCLASS()
class BRICKRIGS_API AFireExtinguisher : public AInventoryItem
{
	GENERATED_BODY()

	// ~Components
	TWeakObjectPtr<UFireExtinguisherComponent> ExtinguisherComponent;
	// ~Components

public:
	// ~Super Interface
	virtual void OnUnequipped() override;
	virtual void SetIsFiring(bool bNewFiring) override;
	// ~Super Interface

	// Called by the extinguish action
	void OnStartExtinguish();
	void OnStopExtinguish();

};