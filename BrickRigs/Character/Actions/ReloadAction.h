// Copyright Fluppisoft 2017

#pragma once

#include "CoreMinimal.h"
#include "ItemAction.h"
#include "ReloadAction.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UReloadAction : public UItemAction
{
	GENERATED_BODY()

private:
	// ~Variables
	// Timer for when the character grapped a new mag
	int32 GrabMagTimerIndex;
	// Timer index of the mag insert action
	int32 InsertMagTimerIndex;
	// ~Variables

public:
	// ~Constructor
	UReloadAction();

	// ~Super Interface
	virtual bool CanStartAction() const override;
	virtual bool CanStartOtherAction(const UCharacterAction* OtherAction) const override;
	virtual void StartItemAction() override;
	virtual void StartAnimation() override;
	virtual void OnActionFinished(bool bSuccess) override;
	virtual bool IsItemSupported(AInventoryItem* InItem) const override;
	// ~Super Interface

	// Whether the action already allows cocking
	bool CanCockAlready() const;

private:
	// Called when the soldier grabbed a new magazine
	void OnGrabbedNewMag();
	// Called when the mag has been inserted
	void OnInsertedMag();
};
