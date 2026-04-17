// Copyright Fluppisoft, 2016

#pragma once

#include "UGC/UGCTypes.h"
#include "Properties/BrickPropertyInterface.h"
#include "GameFramework/CheatManager.h"
#include "BrickCheatManager.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class BRICKRIGS_API UBrickCheatManager : public UCheatManager, public IBrickPropertyInterface
{
	GENERATED_BODY()

private:
	// ~Variables
	bool bReceivedCharacter = false;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(Config)
	float MaxFPS = 0.f;
	UPROPERTY(Config)
	bool bShowStatUnit = true;
	UPROPERTY(Config)
	bool bEnableGodMode = false;
	UPROPERTY(Config)
	FUGCFileInfo VehicleToSpawn;
	// ~Properties

	// ~Super Interface
	virtual AActor* GetTarget(APlayerController* PlayerController, struct FHitResult& OutHit) override;
	// ~Super Interface

	// Events called by the player controller
	void OnReceivedPlayer();
	void OnEndPlay();

	UFUNCTION(Exec)
	void AddScore(float Amount);
	UFUNCTION(Exec)
	void AddMoney(float Amount);
	UFUNCTION(Exec)
	void SetTargetOnFire();
	// Forcefully moves the local player to the spectating state
	UFUNCTION(Exec)
	void ForceSpectate();

private:
	// Updates the values of all console variables
	void UpdateConsoleVariables(bool bEndPlay = false);
	// Spawns the selected vehicle and lets the player enter it
	void SpawnVehicle();

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface
};
