// Copyright Fluppisoft, 2016

#pragma once

#include "BrickGameState.h"
#include "SandboxGameState.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API ASandboxGameState : public ABrickGameState
{
	GENERATED_BODY()

public:
	// ~Constructor
	ASandboxGameState();

	// ~Super Interface
	virtual int32 GetMatchTimerRate_Implementation() const override;
	// ~Super Interface
};
