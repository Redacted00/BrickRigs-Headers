// Copyright Fluppisoft, 2017

#pragma once

#include "Game/BrickGameMode.h"
#include "ZombieGameMode.generated.h"

class APlayerStart;

/**
 *
 */
UCLASS()
class BRICKRIGS_API AZombieGameMode : public ABrickGameMode
{
	GENERATED_BODY()

private:
	// ~Variables
	// Used to delay spawning of zombies
	FTimerHandle TimerHandle_SpawnZombie;
	// Number of zombies that are yet to be spawned in this wave
	int32 ZombieSpawnsRemaining;
	// List of all zombies alive
	UPROPERTY(Transient)
	TArray<AZombie*> Zombies;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = GameMode)
	int32 NumStartZombies;
	UPROPERTY(EditAnywhere, Category = GameMode)
	int32 AddZombiesPerWave;
	// The min and max random amount of time after which a zombie is automatically killed
	UPROPERTY(EditAnywhere, Category = GameMode)
	FFloatInterval ZombieLifeSpanRange;
	UPROPERTY(EditAnywhere, Category = GameMode)
	TSubclassOf<APawn> ZombieClass;
	UPROPERTY(EditAnywhere, Category = GameMode)
	TSubclassOf<APawn> BossZombieClass;
	// ~Properties

public:
	// ~Constructor
	AZombieGameMode();

	// ~Super Interface
	virtual void OnCharacterBeginPlayOrRevived(ABaseCharacter* Character) override;
	virtual void OnCharacterEndPlayOrDied(ABaseCharacter* Character) override;
	virtual void HandleRoundWaitingToStart() override;
	virtual void HandleRoundHasStarted() override;
	// ~Super Interface

	// Get the current number of zombies to spawn
	int32 GetNumZombieSpawns() const;

private:
	// Spawns a single zombie
	void SpawnZombie();
	// Chooses a player start for a zombie to spawn at
	ABrickPlayerStart* ChooseZombiePlayerStart(UClass* PawnClass);
	// Updates the replicated zombie count
	void UpdateZombieCount();
};
