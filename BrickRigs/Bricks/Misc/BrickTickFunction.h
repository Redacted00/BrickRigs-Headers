#pragma once

#include "CoreMinimal.h"
#include "BrickTickFunction.generated.h"

class UBrick;

USTRUCT(BlueprintType)
struct FBrickTickFunction
{
	GENERATED_BODY()

	FBrickTickFunction(UBrick* InTarget = nullptr)
		: TickInterval(0.f), bCanEverTick(false), bAllowTickOnDedicatedServer(false), bAllowTickInEditor(false), bIsEnabled(true), bIsRegistered(false), LastTickTime(0.f), Target(InTarget)
	{
	}

	void Tick(float DeltaTime, float CurrentTime);

	bool IsTickFunctionRegistered() const
	{
		return bIsRegistered;
	}

	void SetTickFunctionRegistered(bool bNewRegistered)
	{
		if (bNewRegistered != bIsRegistered)
		{
			bIsRegistered = bNewRegistered;
			LastTickTime = 0.f;
		}
	}

	bool IsTickFunctionEnabled() const
	{
		return bIsEnabled;
	}

	void SetTickFunctionEnable(bool bNewEnabled)
	{
		if (bNewEnabled != bIsEnabled)
		{
			bIsEnabled = bNewEnabled;
			LastTickTime = 0.f;
		}
	}

	auto* GetTarget() const
	{
		return Target;
	}

	UPROPERTY(EditAnywhere)
	float TickInterval;
	UPROPERTY(EditAnywhere)
	uint8 bCanEverTick : 1;
	UPROPERTY(EditAnywhere)
	uint8 bAllowTickOnDedicatedServer : 1;
	UPROPERTY(EditAnywhere)
	uint8 bAllowTickInEditor : 1;

private:
	uint8 bIsEnabled : 1;
	uint8 bIsRegistered : 1;
	float LastTickTime;
	UBrick* Target;
};
