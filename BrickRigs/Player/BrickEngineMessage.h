// Copyright Fluppisoft, 2017

#pragma once

#include "GameFramework/EngineMessage.h"
#include "BrickEngineMessage.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEngineMessage : public UEngineMessage
{
	GENERATED_BODY()

public:
	// Super Interface
	virtual void ClientReceive(const FClientReceiveData& ClientData) const override;
	// Super Interface
};
