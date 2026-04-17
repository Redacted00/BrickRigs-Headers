// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "Engine/LocalPlayer.h"
#include "BrickLocalPlayer.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickLocalPlayer : public ULocalPlayer
{
	GENERATED_BODY()

public:
	// ~Constructor
	UBrickLocalPlayer();

	// ~Super Interface
	virtual void BeginDestroy() override;
	virtual void PlayerRemoved() override;
	// ~Super Interface

private:
	// Resets the slate operations reply
	void ResetSlateOperations()
	{
		SlateOperations = FReply::Unhandled();
	}
};
