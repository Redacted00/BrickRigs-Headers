// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "Player/BasePlayerController.h"
#include "PendingLevelPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API APendingLevelPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void PostInitializeComponents() override;
	// ~Super Interface
};
