// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MatchTimerWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UMatchTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimer(int32 NewTime);
	// ~Blueprint Interface

private:
	// Callbacks for the game state
	void OnMatchTimerChanged(int32 NewTime);
};
