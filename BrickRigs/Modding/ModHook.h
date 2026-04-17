// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "Inventory/InventoryLoadout.h"
#include "UI/Menu/MenuSequence.h"
#include "UObject/Object.h"
#include "World/Billboard.h"
#include "World/Train.h"
#include "ModHook.generated.h"

/**
 * A mod hook allows mods to execute custom logic and alter game logic
 */
UCLASS(Blueprintable)
class BRICKRIGS_API UModHook : public UObject
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual UWorld* GetWorld() const override;
	// ~Super Interface

	// Called when the world has begun play
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void BeginPlay();
	// Called when the world is being unloaded
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void EndPlay();

	// Allows you to override the game mode after loading a map
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<AGameModeBase> OverrideGameModeClass(TSubclassOf<AGameModeBase> GameModeClass, const FString& MapName, const FString& Options, const FString& Portal);
	// Allows you to override the game state class
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<AGameStateBase> OverrideGameStateClass(TSubclassOf<AGameStateBase> GameStateClass);
	// Allows you to override the player controller class
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<APlayerController> OverridePlayerControllerClass(TSubclassOf<APlayerController> PlayerControllerClass);
	// Allows you to use a custom player camera manager
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<APlayerCameraManager> OverridePlayerCameraManagerClass(TSubclassOf<APlayerCameraManager> PlayerCameraManagerClass, ABasePlayerController* PC);
	// Allows you to override the pawn to spawn for any player
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<APawn> OverridePawnClass(TSubclassOf<APawn> PawnClass, AController* Controller);
	// Allows you to override the spectator (aka free cam) class, this will only be called once at startup
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<ASpectatorPawn> OverrideSpectatorClass(TSubclassOf<ASpectatorPawn> SpectatorClass);
	// Allows you to override the vehicle class to spawn
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<ABrickVehicle> OverrideVehicleClass(TSubclassOf<ABrickVehicle> VehicleClass, ABrickPlayerController* PC, const FPlayerSpawnRequest& SpawnRequest);
	// Allows you to load additional sub levels or prevent game mode specific levels from being loaded
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool OverrideLoadSubLevel(bool bLoadLevel, const FString& LevelName, UGameModeInfo* GameMode);

	// Allows you to modify the loadout to spawn for a character
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FInventoryLoadout OverrideCharacterLoadout(const FInventoryLoadout& Loadout, ABaseCharacter* Character);
	// Allows you to reorder or remove items accessible through loadout containers
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TArray<TSubclassOf<AInventoryItem>> OverrideLoadoutContainerItems(const TArray<TSubclassOf<AInventoryItem>>& ItemClasses);

	// Allows you to override the menu sequence to spawn
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSoftClassPtr<AMenuSequence> OverrideMenuSequence(const TSoftClassPtr<AMenuSequence>& SequenceClass);
	// Allows you to override world setup params in the menu
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FWorldSetupParams OverrideMenuWorldSetupParams(const FWorldSetupParams& Params);
	// Allows you to customize the train to spawn
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<ATrain> OverrideTrainClass(TSubclassOf<ATrain> TrainClass);
	// Allows you to override the texture to display on any billboard
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSoftObjectPtr<UTexture2D> OverrideBillboardTexture(const TSoftObjectPtr<UTexture2D>& Texture, ABillboard* Billboard);
};
