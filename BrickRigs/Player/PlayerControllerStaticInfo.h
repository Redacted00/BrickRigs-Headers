// Copyright Fluppisoft, 2020

#pragma once

#include "BrickCameraManager.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerControllerStaticInfo.generated.h"

// Actors
class AVehicleEditor;
// UI
class UPopupWidget;
// Assets
class USoundBase;
class USoundMix;

/**
 * Stores shared properties for all player controllers
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UPlayerControllerStaticInfo : public UObject
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Fading)
	FCameraFadeParams LevelTransitionFade;
	UPROPERTY(EditDefaultsOnly, Category = Fading)
	FCameraFadeParams StateTransitionFade;
	UPROPERTY(EditDefaultsOnly, Category = Fading)
	FCameraFadeParams MatchEndFade;

	// Maximum number of chat message to cache
	UPROPERTY(EditDefaultsOnly, Category = UI)
	int32 MaxNumChatMessages;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> JoinSessionPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> MapPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> ScoreboardPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> InventoryPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> VehicleBrowserPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> UnsavedChangesPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> CheatMenuPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSoftClassPtr<UPopupWidget> KickPlayerPopupClass;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> PlayerHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> SpawnHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> DeathHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> SpectatorHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> EditorHUDWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSoftClassPtr<UUserWidget> MatchEndHUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = Interaction, meta = (ClampMin = 0.f, ClampMax = 90.f))
	float MaxInteractionAngle;
	UPROPERTY(EditAnywhere, Category = Interaction)
	float MaxCharacterInteractionDistance;
	UPROPERTY(EditAnywhere, Category = Interaction)
	float VehicleHUDIconDrawDistanceScale;
	UPROPERTY(EditAnywhere, Category = Interaction)
	float SpectatorHUDIconDrawDistanceScale;

	// Sound mix used when the player is dead
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundMix* DeathSoundMix;
	// Sound mix used when the player is in a level without an atmosphere
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundMix* NoAtmosphereSoundMix;
	// Sound played when the character was hit
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundBase* HurtSound;

	// Class to instantiate for the vehicle editor
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSubclassOf<AVehicleEditor> VehicleEditorClass;
	// The inflation applied to the garage spawn area when checking if the player is inside of it
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	float EditorEntryGarageInflation;
	// ~Properties

	// ~Constructor
	UPlayerControllerStaticInfo();
};
