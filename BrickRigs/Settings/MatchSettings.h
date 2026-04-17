// Copyright Fluppisoft, 2017

#pragma once

#include "Online/BitfieldSerializer.h"
#include "Misc/LevelInfo.h"
#include "Misc/GameModeInfo.h"
#include "World/WeatherCondition.h"
#include "UGC/UGCTypes.h"
#include "Properties/BrickPropertyInterface.h"
#include "MatchSettings.generated.h"

// These properties define how a world setup actor should behave
USTRUCT(BlueprintType)
struct FWorldSetupParams : public FBrickStructPropertyInterface
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.f, ClampMax = 90.f))
	float SunAngle = 30.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = -180.f, ClampMax = 180.f))
	float SunYaw = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.f, ClampMax = 24.f))
	float TimeOfDay = 9.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan DayLength = FTimespan::MaxValue();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0.f, ClampMax = 1.f))
	float AtmosphereDensity = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWeatherCondition* Weather = nullptr;
	// ~Properties

	// ~Destructor
	virtual ~FWorldSetupParams() override = default;

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	bool operator==(const FWorldSetupParams& Other) const
	{
		return
			SunAngle == Other.SunAngle
			&& SunYaw == Other.SunYaw
			&& TimeOfDay == Other.TimeOfDay
			&& DayLength == Other.DayLength
			&& AtmosphereDensity == Other.AtmosphereDensity
			&& Weather == Other.Weather;
	}

	bool operator!=(const FWorldSetupParams& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		FSerializationHelper::SerializeFloatCompressed(Ar, SunAngle, 0.f, 90.f, 8);
		FSerializationHelper::SerializeFloatCompressed(Ar, SunYaw, -180.f, 180.f, 8);
		FSerializationHelper::SerializeFloatCompressed(Ar, TimeOfDay, 0.f, 24.f, 16);
		FSerializationHelper::SerializeFloatCompressed(Ar, AtmosphereDensity, 0.f, 1.f, 8);
		DayLength.NetSerialize(Ar, Map, bOutSuccess);
		Ar << Weather;
		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FWorldSetupParams> : public TStructOpsTypeTraitsBase2<FWorldSetupParams>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true
		// NOTE: Can't use WithNetSharedSerialization because of the object pointers
	};
};

/**
* Match settings, shared for single and multiplayer
*/
USTRUCT(BlueprintType)
struct FMatchSettings : public FBrickStructPropertyInterface
{
	GENERATED_BODY()

	// NOTE: These are limited to k_cubChatMetadataMax = 8192 for Steam
	constexpr static int32 MaxServerNameLength = 64;
	constexpr static int32 MaxServerDescriptionLength = 2048;
	// NOTE: This has to be <= kNumUGCResultsPerPage for now, otherwise UGC queries break
	constexpr static int32 MaxNumWhitelistedVehicles = 50;

	// Functions used to enforce character and line limits
	static FString ValidateServerName(const FString& Name);
	static FString ValidateServerDescription(const FString& Description);

	// ~Multiplayer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ServerDescription;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 NumPlayerSlots = 2;
	// ~Multiplayer

	// ~Gameplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelInfo* Level = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameModeInfo* GameMode = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWorldSetupParams WorldSetupParams;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan RoundDuration = FTimespan::MaxValue();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoBalanceTeams = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableAITrains = true;
	// Whether the new match settings should fade in smoothly or instantly
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFadeIn = true;
	// ~Gameplay

	// ~Sandbox
	// Minimum time between restarting the player and/or vehicle
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan RestartDelay = FTimespan::Zero();

	// NOTE: These can't be bitflags because that won't work with reflection
	// Whether admins are exempted from all sandbox rules
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bExemptAdminsFromRestrictions = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowInvincibility = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowSpawnAnywhere = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowWeapons = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowExplosions = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowBuildingDamage = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowEnterVehicleWithoutSeat = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowRemoveVehicleFromOtherPlayer = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowFreeCam = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowInfiniteAmmo = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowInfiniteFuel = true;

protected:
	// Maximum number of dummies allowed to exist
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxDummies = 0;
	// ~Sandbox

public:
	// ~Vehicles
	// Exclusive list of vehicles allowed to be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FUGCFileInfo> VehicleWhitelist;
	// Whether the white list should be used
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bEnableVehicleWhitelist = false;
	// Maximum brick count allowed per vehicle
	UPROPERTY()
	uint16 MaxBricksPerVehicle = 1000;
	// Maximum mass vehicles are allowed to have to be spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxVehicleMass = 0.f;
	// Maximum size in any direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxVehicleSize = 0.f;
	// Maximum number of vehicles a player is allowed to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 MaxVehiclesPerPlayer = 4;
	// Multiplies vehicle prices
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VehiclePriceFactor = 0.1f;
	// Time until empty vehicles are removed from the map
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTimespan VehicleRemovalDelay = FTimespan::FromMinutes(1);
	// ~Vehicles

	// ~Destructor
	virtual ~FMatchSettings() override = default;

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~Super Interface

	bool operator==(const FMatchSettings& Other) const;
	bool operator!=(const FMatchSettings& Other) const;

	// Makes sure strings are within limits, selects a valid level, game mode etc.
	void ValidateSettings();

private:
	// Makes sure a valid level and game mode is selected
	void ValidateLevelAndGameMode();

public:
	// Whether the settings can be used
	bool AreSettingsValid() const;
	// Whether the game could be played with the selected settings
	bool CanStartGame() const;
	// Returns the concrete level name and game mode to load
	bool ResolveLevelAndGameMode(FString& OutLevel, FString& OutGameMode) const;
	// Get the selected round duration in seconds
	float GetRoundDuration() const
	{
		return RoundDuration == FTimespan::MaxValue() ? 0.f : RoundDuration.GetTotalSeconds();
	}

	// Returns the maximum amount of dummies to spawn, -1 means unlimited
	auto GetMaxDummies() const
	{
		return MaxDummies == MAX_uint8 ? -1 : static_cast<int32>(MaxDummies);
	}

	//  Sets the maximum amount of dummies to spawn, <0 means infinite
	void SetMaxDummies(int32 Max)
	{
		MaxDummies = Max < 0 ? MAX_uint8 : FMath::Min(Max, MAX_uint8 - 1);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << ServerName;
		Ar << ServerDescription;
		Ar << NumPlayerSlots;
		Ar << Level;
		Ar << GameMode;
		Ar << MaxBricksPerVehicle;
		Ar << MaxVehicleMass;
		Ar << MaxVehicleSize;
		Ar << MaxVehiclesPerPlayer;
		Ar << VehiclePriceFactor;
		Ar << MaxDummies;
		WorldSetupParams.NetSerialize(Ar, Map, bOutSuccess);
		RoundDuration.NetSerialize(Ar, Map, bOutSuccess);
		VehicleRemovalDelay.NetSerialize(Ar, Map, bOutSuccess);
		RestartDelay.NetSerialize(Ar, Map, bOutSuccess);

		// Serialize the vehicle whitelist
		auto NumVehicles = static_cast<uint16>(VehicleWhitelist.Num());
		Ar << NumVehicles;
		if (Ar.IsLoading())
		{
			VehicleWhitelist.SetNum(NumVehicles);
		}
		for (auto i = 0u; i < NumVehicles; ++i)
		{
			VehicleWhitelist[i].NetSerialize(Ar, Map, bOutSuccess);
		}

		TBitfieldSerializer<uint16> Bitfield(Ar);
		Bitfield.Add(bAutoBalanceTeams);
		Bitfield.Add(bEnableAITrains);
		Bitfield.Add(bFadeIn);
		Bitfield.Add(bAllowInvincibility);
		Bitfield.Add(bAllowSpawnAnywhere);
		Bitfield.Add(bAllowWeapons);
		Bitfield.Add(bAllowExplosions);
		Bitfield.Add(bAllowBuildingDamage);
		Bitfield.Add(bExemptAdminsFromRestrictions);
		Bitfield.Add(bAllowEnterVehicleWithoutSeat);
		Bitfield.Add(bAllowRemoveVehicleFromOtherPlayer);
		Bitfield.Add(bAllowFreeCam);
		Bitfield.Add(bAllowInfiniteAmmo);
		Bitfield.Add(bAllowInfiniteFuel);
		Bitfield.Add(bEnableVehicleWhitelist);
		Bitfield.Finish();

		return true;
	}

private:
	// Property callbacks
	static bool StaticIsGameModeSupported(const FBrickPropertyContainer& Container, UObject* Object);
};

template <>
struct TStructOpsTypeTraits<FMatchSettings> : TStructOpsTypeTraitsBase2<FMatchSettings>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true
		// NOTE: Can't use WithNetSharedSerialization because of the object pointers
	};
};
