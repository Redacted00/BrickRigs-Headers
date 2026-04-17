// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "Attachment.generated.h"

class AFirearm;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AAttachment : public AInventoryItem
{
	GENERATED_BODY()

public:
	// Get the owning firearm
	UFUNCTION(BlueprintPure)
	AFirearm* GetFirearm() const;
};
