// Copyright Fluppisoft 2017

#pragma once

#include "CoreMinimal.h"
#include "ItemAction.h"
#include "CockAction.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UCockAction : public UItemAction
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual bool CanStartAction() const override;
	virtual void StartItemAction() override;
	virtual bool CanCharacterAim() const override;
	virtual bool IsItemSupported(AInventoryItem* InItem) const override;
	// ~Super Interface

private:
	// Called by the timer when the bolt has been pulled back
	void OnPulledBolt();
	// Called when the bolt has been released
	void OnReleasedBolt();
};
