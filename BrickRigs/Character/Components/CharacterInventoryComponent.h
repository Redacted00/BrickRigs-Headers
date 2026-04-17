// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Inventory/InventoryComponent.h"
#include "CharacterInventoryComponent.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UCharacterInventoryComponent : public UInventoryComponent
{
	GENERATED_BODY()

public:
	// ~Constructor
	UCharacterInventoryComponent();

	// ~Super Interface
	virtual bool ShouldReplicateItem(const AInventoryItem* InItem) const override;
	virtual bool GetItemAttachInfo(AInventoryItem* InItem, USceneComponent*& OutAttachParent, FName& OutAttachSocket, FVector& OutLocation, FRotator& OutRotation) const override;
	// ~Super Interface
};
