// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/ItemAction.h"
#include "ThrowExplosiveAction.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UThrowExplosiveAction : public UItemAction
{
	GENERATED_BODY()

private:
	// ~Variables
	int32 ThrowTimerIndex;
	// ~Variables

public:
	// ~Super Interface
	virtual bool CanStartAction() const override;
	virtual void StartItemAction() override;
	virtual bool ShouldCancelOnItemChange() const override;
	virtual void OnActionFinished(bool bSuccess) override;

	virtual FName GetAnimationName() const override
	{
		return "ThrowAnimation";
	}

	virtual bool IsItemSupported(AInventoryItem* InItem) const override;
	// ~Super Interface

private:
	// Called by the throw timer
	void OnThrowExplosive();
};
