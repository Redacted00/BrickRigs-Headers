// Copyright Fluppisoft, 2016

#pragma once

#include "Components/SceneCaptureComponent2D.h"
#include "Settings/MatchSettings.h"
#include "GameFramework/Actor.h"
#include "Misc/BrickAssetManager.h"
#include "WorldSetupActor.generated.h"

class UPostProcessComponent;
class UDirectionalLightComponent;
class USkyLightComponent;
class UStaticMeshComponent;
class UAudioComponent;
class UExponentialHeightFogComponent;
class USceneCaptureComponent2D;
class UArrowComponent;

class UMaterialParameterCollection;
class UTextureRenderTarget2D;
class UTextureCube;
class USoundBase;
class USoundAttenuation;
class USoundMix;

struct FMapRenderParams
{
	FVector2D Position;
	float Rotation;
	FVector2D Size;
	FIntPoint Resolution;

	bool operator==(const FMapRenderParams& Other) const
	{
		return Position == Other.Position && Rotation == Other.Rotation && Size == Other.Size && Resolution == Other.Resolution;
	}

	bool operator!=(const FMapRenderParams& Other) const
	{
		return !(*this == Other);
	}
};

/**
 * This actor has to be placed in every level to take care of lighting, weather and other effects
 */
UCLASS(Abstract, HideCategories = ("ActorTick", "Rendering", "Replication", "Input", "Actor", "LOD", "Collision"))
class BRICKRIGS_API AWorldSetupActor : public AActor
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimeOfDayChanged, float);

	// ~Variables
	FSmartStreamableHandle StreamableHandle_AmbientSound;
	// Used to count down to the next lightning strike
	FTimerHandle TimerHandle_Lightning;
	// NOTE: These structs need to be UPROPERTY since they includes pointers
	// The params that should currently be in effect
	UPROPERTY(Transient)
	FWorldSetupParams CurrentWorldSetupParams;
	// The previous params that are being transitioned from
	UPROPERTY(Transient)
	FWorldSetupParams PrevWorldSetupParams;
	// Currently desired parameters
	UPROPERTY(Transient)
	FWorldSetupParams TargetWorldSetupParams;
	// The weather params currently in effect
	FWeatherConditionParams CurrentWeather;
	// Whether being transitioned away from
	FWeatherConditionParams PrevWeather;
	// Currently desired weather
	FWeatherConditionParams TargetWeather;
	// Server time when the transition was started
	float TransitionStartTime;
	// Current transition phases of different layers
	float TimeTransitionPhase;
	float WeatherTransitionPhase;
	float GroundCoverageTransitionPhase;
	// Whether the world setup params have been initialized yet
	uint8 bInitializedWorldSetupParams : 1;
	// Whether the local player is currently in the editor
	uint8 bIsInEditor : 1;
	// Whether the ambient sound should also transition
	uint8 bAmbientSoundTransitioning : 1;
	// Whether the camera is currently underwater
	uint8 bIsUnderwater : 1;
	// Whether bIsAboveClouds has been updated before
	uint8 bInitializedAboveClouds : 1;
	// Whether we were above the clouds last camera update
	uint8 bIsAboveClouds : 1;
	// Cached clouds bottom color
	FLinearColor CloudUnlitColor;
	// The current time of day
	float TimeOfDay;
	// The last time of day that has actually been applied
	float LastTODUpdateTime;
	// The last time of day where the reflection capture has been updated
	float LastReflectionCaptureUpdateTime;
	// At 1 the sun is at the zenith, at -1 pointing in the opposite direction
	float SunHeight;
	// The current direction and speed of the wind
	FVector WindVelocity;
	// The cached total distance the wind has moved
	FVector AccumulatedWindMovement;
	// Current coordinate indices of the clouds mesh
	FIntVector CloudCoordinates;
	// The max vertical distance at which the post process effect could be visible
	float CloudPostProcessMaxDistance;
	// The current intensity of the precipitation PSC
	float PrecipitationSystemIntensity;
	// Counter of the next lightning to be spawned
	int32 NextLightningIndex;
	// ~Variables

protected:
	// ~Components
	// ~Lighting
	UPROPERTY(EditDefaultsOnly, Category = Components)
	UDirectionalLightComponent* SunLightComponent;
	UPROPERTY(EditDefaultsOnly, Category = Components)
	USkyLightComponent* SkyLightComponent;
	UPROPERTY(Transient)
	UStaticMeshComponent* SkyMeshComponent;
	// ~Lighting

	// ~Weather
	UPROPERTY(Transient)
	UStaticMeshComponent* CloudMeshComponent;
	UPROPERTY(Transient)
	UParticleSystemComponent* PrecipitationPSC;
	UPROPERTY(Transient)
	TArray<UAudioComponent*> LightningACs;
	UPROPERTY(Transient)
	UExponentialHeightFogComponent* ExponentialHeightFogComponent;
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UArrowComponent* WindArrowComponent;
#endif
	// ~Weather

	// ~Misc
	UPROPERTY(EditDefaultsOnly, Category = Components)
	UPostProcessComponent* PostProcessComponent;
	UPROPERTY(Transient)
	UAudioComponent* AmbientAudioComponent;
	UPROPERTY(Transient)
	USceneCaptureComponent2D* MapCaptureComponent;
	// ~Misc
	// ~Components

	// ~Properties
	// World setup
	UPROPERTY(EditDefaultsOnly, Category = WorldSetup)
	UStaticMesh* SkyMesh;
	UPROPERTY(EditDefaultsOnly, Category = WorldSetup)
	float SkyMeshScale;
	UPROPERTY(EditDefaultsOnly, Category = WorldSetup)
	UMaterialParameterCollection* EnvironmentMPC;
	// The sky material to use, default mesh material is used if this is null
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	UMaterialInterface* SkyMaterial;
	// Sky material to use in the brick editor
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	UMaterialInterface* EditorSkyMaterial;
	// Seconds it takes to transition for every our in time difference during a transition
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	float TimeTransitionRate;
	// How long it takes for weather to transition
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	float WeatherTransitionTime;
	// How long it for ground coverage (snow, wetness) to fully transition
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	float GroundCoverageTransitionTime;
	// Above the maximum height there will always be snow, below the minimum never
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	FFloatInterval SnowHeightRange;
	// The default height below which everything is wet
	UPROPERTY(EditAnywhere, Category = WorldSetup)
	float WaterHeight;

	// Time of day
	// Amount of hours between a time of day update
	UPROPERTY(EditDefaultsOnly, Category = TimeOfDay)
	float TimeOfDayUpdateInterval;
	// Amount of hours between a reflection capture update
	UPROPERTY(EditDefaultsOnly, Category = TimeOfDay)
	float ReflectionCaptureUpdateInterval;
	UPROPERTY(EditAnywhere, Category = TimeOfDay)
	UCurveLinearColor* SunColorCurve;
	UPROPERTY(EditAnywhere, Category = TimeOfDay)
	UCurveLinearColor* SunColorCurveNoAtmosphere;
	UPROPERTY(EditAnywhere, Category = TimeOfDay)
	UCurveLinearColor* SkyColorCurve;
	UPROPERTY(EditAnywhere, Category = TimeOfDay)
	UCurveLinearColor* HorizonColorCurve;
	// R channel determines the sun intensity, A how much to lerp sun color with cloud color
	UPROPERTY(EditAnywhere, Category = TimeOfDay)
	UCurveLinearColor* CloudDensitySunIntensityCurve;

	// Clouds
	UPROPERTY(EditAnywhere, Category = Clouds)
	UStaticMesh* CloudMesh;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float MaxCloudThickness;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudCoordinateSnappingGrid;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudScaleSnappingGrid;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudScaleFactor;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudLitColorSunIntensity = 1.f;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudLitColorSkyIntensity = 1.f;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudUnlitColorDesaturation = 0.9f;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float CloudUnlitColorBrightness = 0.1f;
	// Min cloud density at which the opauqe material is used
	UPROPERTY(EditAnywhere, Category = Clouds)
	float MinCloudOpaqueMaterialDensity;
	// Material used when clouds are fully opaque
	UPROPERTY(EditAnywhere, Category = Clouds)
	UMaterialInterface* CloudOpaqueMaterial;
	UPROPERTY(EditAnywhere, Category = Clouds)
	UMaterialInterface* CloudPostProcessMaterial;

	// Fog
	UPROPERTY(EditAnywhere, Category = Clouds)
	float FogDensity;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float FogHeightFalloff;
	UPROPERTY(EditAnywhere, Category = Clouds)
	float FogStartDistance;
	// Vertical offset applied to the fog location
	UPROPERTY(EditAnywhere, Category = Clouds)
	float FogHeightOffset;
	// Brightness factor applied to the fog color
	UPROPERTY(EditAnywhere, Category = Clouds)
	float FogBrightness;

	// Lightning
	UPROPERTY(EditAnywhere, Category = Lightning)
	TArray<USoundBase*> LightningSounds;
	UPROPERTY(EditAnywhere, Category = Lightning)
	float MaxLightningLength;

	// Precipitation
	UPROPERTY(EditAnywhere, Category = Precipitation)
	UParticleSystem* PrecipitationEmitters[static_cast<int32>(EPrecipitationType::Max)];

	// Water
	UPROPERTY(EditDefaultsOnly, Category = Water)
	USoundMix* UnderwaterSoundMix;
	UPROPERTY(EditDefaultsOnly, Category = Water)
	UMaterialInterface* UnderwaterProcessMaterial;

	// Map Render
	UPROPERTY(EditDefaultsOnly, Category = Map)
	UTextureRenderTarget2D* MapRenderTarget;
	UPROPERTY(EditDefaultsOnly, Category = Map)
	TEnumAsByte<ESceneCaptureSource> MapCaptureSource;
	UPROPERTY(EditDefaultsOnly, Category = Map)
	TArray<FEngineShowFlagsSetting> MapCaptureShowFlags;
	UPROPERTY(EditDefaultsOnly, Category = Map)
	FPostProcessSettings MapCapturePostProcessSettings;

public:
	// The minimum area to display on the map screen
	UPROPERTY(EditAnywhere, Category = Map)
	FBox MapWorldBounds;
	// ~Properties

	// ~Delegates
	// Broadcasted when the time of day has changed
	FOnTimeOfDayChanged OnTimeOfDayChangedDelegate;
	// ~Delegates

	// ~Constructor
	AWorldSetupActor();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void PostRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;

	virtual bool ShouldTickIfViewportsOnly() const override
	{
		return true;
	}
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif
	// ~Super Interface

	// ~World Setup
	// Get the world setup actor instance
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get World Setup"))
	static AWorldSetupActor* Get(const UObject* WorldContextObject);

	// Sets whether the local player is currently in the editor
	void SetIsLocalPlayerInEditor(bool bNewInEditor);
	// Updates the world setup params that should currently be used
	void UpdateWorldSetupParams(bool bFadeIn = false);

private:
	// These are used to update the separate transitions
	void UpdateWorldSetupTransition();
	// Get the sycned server time seconds
	float GetServerTimeSeconds() const;
	// Get the random seed from the game state
	uint8 GetMatchRandomSeed() const;
	// Marks the reflection capture as needing to be updated
	void MarkReflectionCaptureDirty(bool bImmediate);
	// Updates the current sky material
	void UpdateSkyMaterial();
	// Callback for the world
	void OnBeginPostProcessSettings(FVector ViewLocation, FSceneView* SceneView);

public:
	// Get the currently used atmosphere density
	float GetAtmosphereDensity() const
	{
		return CurrentWorldSetupParams.AtmosphereDensity;
	}

	// ~World Setup

	// ~Map
	// Get the rotation of the compass around the Z axis
	float GetCompassRotation() const;
	// Renders the map texture, expensive operation!
	void RenderMap(const FMapRenderParams& Params);
	// ~Map

	// ~Time of Day
	// Public accessor to the time of day
	auto GetTimeOfDay() const
	{
		return TimeOfDay;
	}

private:
	// Updates the current time of day if needed
	void UpdateTimeOfDay(bool bForceUpdate = false);
	// Updates all time of day parameters, like the sun position
	void OnTimeOfDayChanged();
	// Returns the desired sun disk color
	FLinearColor GetSunColor() const;
	// Updates the sun color and shadow intensity
	void UpdateSunColorAndIntensity();
	// ~Time of Day

public:
	// ~Weather
	// Get the current velocity of the wind
	const FVector& GetWindVelocity() const
	{
		return WindVelocity;
	}

private:
	// Updates the current weather condition
	void UpdateWeather();
	// Get the cloud height in world space
	float GetWorldCloudHeight() const;
	// Get the current thickness of the clouds
	float GetCloudThickness() const;
	// Moves the clouds mesh to the current coordinates
	void UpdateCloudCoordinates();
	// Adjusts the rotation of the clouds mesh
	void UpdateCloudRotation();
	// Sets up the timer for the next lightning
	void SetupLightningTimer();
	// Spawns a lightning
	void OnLightningTimer();
	// Updates the current color of the fog
	void UpdateFogColor();
	// ~Weather

public:
	// ~Camera
	// Get the position of the default camera
	void GetDefaultCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const;
	// ~Camera

private:
	// ~Water
	// Updates the underwater state
	void SetIsUnderwater(bool bNewUnderwater);
	// ~Water
};
