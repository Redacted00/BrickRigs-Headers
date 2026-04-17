// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/ItemAction.h"
#include "ExtinguishAction.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UExtinguishAction : public UItemAction
{
	GENERATED_BODY()
	
public:
	// ~Super Interface
	virtual bool CanStartAction() const override;
	virtual void StartItemAction() override;
	virtual void OnActionFinished(bool bSuccess) override;
	virtual bool CanCharacterFire() const override { return true; }
	virtual bool IsItemSupported(AInventoryItem* InItem) const override;
	// ~Super Interface

private:
	void OnStartExtinguish();
	
};
