// Copyright Fluppisoft, 2015

#pragma once

#include "Train.h"
#include "WorldSetupActor.h"
#include "GameFramework/WorldSettings.h"
#include "BrickWorldSettings.generated.h"

class ABaseCharacter;
// Actors
class ACapturePoint;
class ABrickBuilding;
class ADefaultPhysicsVolume;
// Components
class UProjectileManagerComponent;
class UHUDIconComponent;
class USplineComponent;
// Assets
class UTexture2D;
class UMaterialInstanceDynamic;

USTRUCT(BlueprintType)
struct FTrainSpawnInfo
{
	GENERATED_BODY()

	// Index of the train track to spawn on
	UPROPERTY(EditAnywhere)
	int32 TrackIndex = 0;
	// The type of train to spawn
	UPROPERTY(EditAnywhere)
	TSubclassOf<ATrain> TrainClass;
};

USTRUCT(BlueprintType)
struct FTrainTrackSplineMeshInfo
{
	GENERATED_BODY()

	// The mesh to look out for
	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh = nullptr;
	// Offset of the rail relative to the mesh origin
	UPROPERTY(EditAnywhere)
	FVector RailOffset = FVector::ZeroVector;
};

/**
 *
 */
UCLASS(Blueprintable)
class BRICKRIGS_API ABrickWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FHUDIconAdded, UHUDIconComponent*);
	DECLARE_MULTICAST_DELEGATE(FOnSlomoChanged);

	// ~Variables
	// The world setup actor that has been created for this level
	UPROPERTY(Transient, VisibleAnywhere, Category = World)
	AWorldSetupActor* WorldSetupActor;
	// Default water physics volume instance
	UPROPERTY(Transient)
	APhysicsVolume* DefaultWaterPhysicsVolume;
	// Current time used for the wave simulation
	float WaveTime;

	// Realtime seconds when the world has begun play
	float BeginPlayTime;
	// The target slow motion speed level
	int8 SlomoSpeedLevel;
	// Whether slow motion is currently enabled
	uint8 bSlomoEnabled : 1;
	// Whether trains have been spawned yet
	uint8 bSpawnedTrains : 1;
	// List of trains that have been spawned
	UPROPERTY(Transient)
	TArray<ATrain*> SpawnedTrains;

	// List of registered HUD icon components
	TArray<TWeakObjectPtr<UHUDIconComponent>> HUDIconComponents;
	// List of characters in the level
	TArray<TWeakObjectPtr<ABaseCharacter>> Characters;
	// Actors that should be targeted by heat seeking missiles
	TArray<TWeakObjectPtr<AActor>> HeatSeekingTargets;

public:
	// List of capture points in the level
	UPROPERTY(Transient)
	TArray<ACapturePoint*> CapturePoints;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient)
	UProjectileManagerComponent* ProjectileManager;
	// ~Components

public:
	// ~Properties
	// Default water physics volume class to spawn
	UPROPERTY(EditAnywhere, NoClear, BlueprintReadOnly, Category = Physics, AdvancedDisplay)
	TSubclassOf<ADefaultPhysicsVolume> DefaultWaterPhysicsVolumeClass;

	// Bounding box of the ocean water, calculated from the world bounds actor
	UPROPERTY(EditDefaultsOnly, Category = World)
	FBox WaterBounds;
	// Used to determine the altitude
	UPROPERTY(EditDefaultsOnly, Category = World)
	float SeaLevel;
#if WITH_EDITORONLY_DATA
	// Whether the water bounding box should be determined by the world bounds actor settings
	UPROPERTY(EditDefaultsOnly, Category = World)
	bool bSetWaterBoundsFromWorldBounds;
	// Whether the sea level should be automatically determined by the world bounds actor
	UPROPERTY(EditDefaultsOnly, Category = World)
	bool bSetSeaLevelFromWorldBounds;
	// Whether MBP should be setup automatically from the world bounds actor dimensions
	UPROPERTY(EditDefaultsOnly, Category = World)
	bool bSetMBPFromWorldBounds;
	// Whether the kill Z height should be setup automatically from the world bounds actor
	UPROPERTY(EditDefaultsOnly, Category = World)
	bool bSetKillZFromWorldBounds;

	// Used to test different scenarios in the editor
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	FWorldSetupParams EditorWorldSetupParams;
	// Weather settings used when there is no weather preset specified in EditorWorldSetupParams
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	FWeatherConditionParams EditorWeatherOverride;
#endif

	UPROPERTY(VisibleAnywhere, Category = Trains)
	TArray<FTrainTrack> TrainTracks;

protected:
	UPROPERTY(EditAnywhere, Category = Trains)
	TArray<FTrainSpawnInfo> TrainSpawnInfos;
#if WITH_EDITORONLY_DATA
	// Set to true to rebuild the train track data
	UPROPERTY(EditAnywhere, Category = Trains)
	bool bBuildTrainTracks;
	// Meshes that should be considered when building train tracks
	UPROPERTY(EditDefaultsOnly, Category = Trains)
	TArray<FTrainTrackSplineMeshInfo> TrainTrackSplineMeshes;
#endif
	// ~Properties

public:
	// ~Delegates
	// Called when the desired slomo setup has changed
	FOnSlomoChanged OnSlomoChangedDelegate;
	// Called when a HUD icon was added or removed
	FHUDIconAdded OnHUDIconAddedDelegate;
	FHUDIconAdded OnHUDIconRemovedDelegate;
	// ~Delegates

	// Helper function for getting the casted world settings
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get World Settings"))
	static ABrickWorldSettings* Get(const UObject* WorldContextObject);

	// ~Constructor
	ABrickWorldSettings();

	// ~Super Interface
#if WITH_EDITOR
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	virtual void PostRegisterAllComponents() override;
	virtual void PostUnregisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyBeginPlay() override;
	virtual float SetTimeDilation(float NewTimeDilation) override;
	virtual float GetEffectiveTimeDilation() const override;
	virtual void SetPauserPlayerState(APlayerState* PlayerState) override;
	// ~Super Interface

#if WITH_EDITOR
	// Builds all cached data, such as the train tracks
	void BuildCachedData();
	// Builds the list of train tracks
	void BuildTrainTracks();
#endif

	// Sets the world setup for the level
	void SetWorldSetupActor(AWorldSetupActor* InWorldSetup, bool bReinit = false);
	// Returns the world setup actor instance of the level
	auto* GetWorldSetupActor() const
	{
		return WorldSetupActor;
	}

	// Returns the immersion depth at the given location
	TTuple<APhysicsVolume*, float> GetWaterImmersionDepth(const FVector& Location, bool bAddWaves = true) const;

	// Called by HUD icon components upon activate and deactivation
	void AddHUDIconComponent(UHUDIconComponent* Comp);
	void RemoveHUDIconComponent(UHUDIconComponent* Comp);

	int32 GetNumHUDIconComponents() const
	{
		return HUDIconComponents.Num();
	}

	// Allows safely iterating all registered HUD icon components
	void ForEachHUDIconComponent(const TFunction<void(UHUDIconComponent*)>& Func);

	// Registers a new character in the level
	void AddCharacter(ABaseCharacter* Character);
	// Removes a character from the level
	void RemoveCharacter(ABaseCharacter* Character);
	// Returns all registered characters
	const auto& GetCharacters() const
	{
		return Characters;
	}

	// Adds a new heat seeking target
	UFUNCTION(BlueprintCallable)
	void AddHeatSeekingTarget(AActor* Target);
	// Removes a heat seeking target
	UFUNCTION(BlueprintCallable)
	void RemoveHeatSeekingTarget(AActor* Target);
	// Returns currently registered head seeking targets
	const auto& GetHeatSeekingTargets() const
	{
		return HeatSeekingTargets;
	}

	// Public accessor to the projectile manager
	UProjectileManagerComponent* GetProjectileManager() const;
	// Whether the world has just been initialized
	bool HasWorldJustBegunPlay() const;
	// Return whether the building should be able to collapse
	bool CanBuildingCollapse(ABrickBuilding* Building) const;
	// Returns the closest point on the train track if close enough
	bool SnapToTrainTrack(const FVector& Location, const FVector& ReferenceDirection, FVector& OutLocation, FRotator& OutRotation) const;

	// Switch slow motion on or off
	void SetSlomoEnabled(bool bEnable);
	// Return whether slomo is currently active
	bool IsSlomoEnabled() const;
	// Change the speed of slow motion
	void SetSlomoSpeedLevel(int32 NewLevel);
	// Get the current slomo speed level
	int32 GetSlomoSpeedLevel() const;
	// Get the maximum and minimum possible speed level
	void GetSlomoSpeedLevelRange(int32& OutMin, int32& OutMax) const;
	// Get the selected slow motion speed factor
	float GetSlomoSpeed(bool& bOutInverted) const;
	// Whether the game is currently paused
	bool IsPaused() const;

private:
	// Get the currently desired time dilation
	float GetTargetTimeDilation() const;
	// Called whenever the current time dilation has changed
	void OnTimeDilationChanged();
	// Called whenever the slomo is activated/deactivated or the speed changes
	void OnSlomoParamsChanged();
	// Callback for the game state
	void OnMatchSettingsChanged(const FMatchSettings& MatchSettings);
};
