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

class UMenuWidget;
class ABrickSpectatorPawn;
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
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlay();
	// Called when the world is being unloaded
	UFUNCTION(BlueprintImplementableEvent)
	void EndPlay();

	// Allows you to override the game mode after loading a map
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<AGameModeBase> OverrideGameModeClass(TSubclassOf<AGameModeBase> GameModeClass, const FString& MapName, const FString& Options, const FString& Portal);
	// Allows you to override the game state class
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<AGameStateBase> OverrideGameStateClass(TSubclassOf<AGameStateBase> GameStateClass);
	// Allows you to override the player controller class
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<APlayerController> OverridePlayerControllerClass(TSubclassOf<APlayerController> PlayerControllerClass);
	// Allows you to override the player controller class used for demo replays
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<APlayerController> OverrideReplayPlayerControllerClass(TSubclassOf<APlayerController> PlayerControllerClass);
	// Allows you to use a custom player camera manager
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<APlayerCameraManager> OverridePlayerCameraManagerClass(TSubclassOf<APlayerCameraManager> PlayerCameraManagerClass, ABasePlayerController* PC);
	// Allows you to override the pawn to spawn for any player
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<APawn> OverridePawnClass(TSubclassOf<APawn> PawnClass, AController* Controller);
	// Allows you to override the spectator (aka free cam) class, this will only be called once at startup
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<ASpectatorPawn> OverrideSpectatorClass(TSubclassOf<ASpectatorPawn> SpectatorClass);
	// Allows you to override the vehicle class to spawn
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<ABrickVehicle> OverrideVehicleClass(TSubclassOf<ABrickVehicle> VehicleClass, ABrickPlayerController* PC, const FPlayerSpawnRequest& SpawnRequest);
	// Allows you to load additional sub levels or prevent game mode specific levels from being loaded
	UFUNCTION(BlueprintNativeEvent)
	bool OverrideLoadSubLevel(bool bLoadLevel, const FString& LevelName, UGameModeInfo* GameMode);

	// Called from the player controller BeginPlay event
	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerControllerBeginPlay(ABasePlayerController* PC);
	// Called from the character BeginPlay event
	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterBeginPlay(ABaseCharacter* Character);
	// Called from the spectator BeginPlay event
	UFUNCTION(BlueprintImplementableEvent)
	void OnSpectatorBeginPlay(ABrickSpectatorPawn* Pawn);
	// Called from the vehicle BeginPlay event
	UFUNCTION(BlueprintImplementableEvent)
	void OnVehicleBeginPlay(ABrickVehicle* Vehicle);

	// Called on the server after a new player controller has been spawned
	UFUNCTION(BlueprintImplementableEvent)
	void PostSpawnPlayerController(APlayerController* PC);
	// Called on the server after a new pawn has been spawned
	UFUNCTION(BlueprintImplementableEvent)
	void PostSpawnPawn(APawn* Pawn);
	// Called on the client after a new spectator pawn has been spawned
	UFUNCTION(BlueprintImplementableEvent)
	void PostSpawnSpectatorPawn(ASpectatorPawn* Pawn);
	// Called on the server after a new vehicle has been spawned
	UFUNCTION(BlueprintImplementableEvent)
	void PostSpawnVehicle(ABrickVehicle* Vehicle);

	// Allows you to modify the loadout to spawn for a character
	UFUNCTION(BlueprintNativeEvent)
	FInventoryLoadout OverrideCharacterLoadout(const FInventoryLoadout& Loadout, ABaseCharacter* Character);
	// Allows you to reorder or remove items accessible through loadout containers
	UFUNCTION(BlueprintNativeEvent)
	TArray<TSubclassOf<AInventoryItem>> OverrideLoadoutContainerItems(const TArray<TSubclassOf<AInventoryItem>>& ItemClasses);

	// Allows you to override the menu sequence to spawn
	UFUNCTION(BlueprintNativeEvent)
	TSoftClassPtr<AMenuSequence> OverrideMenuSequence(const TSoftClassPtr<AMenuSequence>& SequenceClass);
	// Allows you to override world setup params in the menu
	UFUNCTION(BlueprintNativeEvent)
	FWorldSetupParams OverrideMenuWorldSetupParams(const FWorldSetupParams& Params);
	// Allows you to customize the train to spawn
	UFUNCTION(BlueprintNativeEvent)
	TSubclassOf<ATrain> OverrideTrainClass(TSubclassOf<ATrain> TrainClass);
	// Allows you to override the texture to display on any billboard
	UFUNCTION(BlueprintNativeEvent)
	TSoftObjectPtr<UTexture2D> OverrideBillboardTexture(const TSoftObjectPtr<UTexture2D>& Texture, ABillboard* Billboard);

	// Allows you to modify or add menu pages, return true if the original menu page should not be shown
	UFUNCTION(BlueprintNativeEvent)
	bool OverrideMenu(UMenuWidget* Widget, const FName& Context);
};
