#pragma once

#include "Player/PlayerSpawnRequest.h"
#include "UI/Misc/DisplayInfo.h"
#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "SpawnPoint.generated.h"

class ABrickPlayerController;
class ABaseCharacter;

UINTERFACE()
class BRICKRIGS_API USpawnPointInterface : public UInterface
{
	GENERATED_BODY()
};

class ISpawnPointInterface
{
	GENERATED_BODY()

protected:
	const static int32 TeamBaseSortPriority = 1000;
	const static int32 CapturePointSortPriority = 0;

public:
	// Whether players should be spawned here automatically
	virtual bool IsAutoSpawnPoint() const { return true; }
	// Get the desired sort priority, higher values are displayed on top
	virtual int32 GetSpawnSortPriority() const { return 0; }
	// Return true if the player can enter the spawn point (for example a vehicle)
	virtual bool CanPlayerEnterSpawnPoint(ABrickPlayerController* PC) const { return false; }
	// Lets the player enter the spawn point
	virtual void EnterSpawnPoint(ABaseCharacter* InCharacter)
	{
	}

	// Return whether the given player is allowed to spawn here
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const { return false; }
	// Return the exact spawn point for a given player
	virtual bool GetSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const = 0;
	// Whether the player is allowed to spawn a vehicle here
	virtual bool CanPlayerSpawnVehicle(ABrickPlayerController* PC) const { return false; }
	// Get the spawn position for vehicles
	virtual bool GetVehicleSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const { return false; }
	// Return the box extend of the vehicle spawn area (if there is any)
	virtual TTuple<FVector, FVector> GetVehicleSpawnBounds() const { return MakeTuple(FVector::ZeroVector, FVector::ZeroVector); };
};
