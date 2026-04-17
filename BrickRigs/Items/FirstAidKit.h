// Copyright Fluppisoft, 2021

#pragma once

#include "StaticInfo/FirstAidKitStaticInfo.h"
#include "CoreMinimal.h"
#include "Items/InventoryItem.h"
#include "FirstAidKit.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AFirstAidKit : public AInventoryItem
{
	GENERATED_BODY()

	// ~Variables
	int32 NumBandages;
	// ~Variables

public:
	// ~Super Interface
	virtual void PostInitializeComponents() override;
	virtual void SetNumItems_Implementation(int32 Num) override;
	virtual int32 GetNumItems_Implementation() const override;
	virtual int32 GetNumDefaultItems_Implementation() const override;
	// ~Super Interface
};
