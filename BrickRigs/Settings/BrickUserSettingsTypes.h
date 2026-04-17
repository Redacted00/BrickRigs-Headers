#pragma once

#include "CoreMinimal.h"
#include "BrickUserSettingsTypes.generated.h"

UENUM(BlueprintType)
enum class EServerSortMethod : uint8
{
	Name,
	Map,
	GameMode,
	Players,
	PlayersAsc,
	Ping
};

UENUM(BlueprintType)
enum class ESearchSessionType : uint8
{
	Server,
	Lobby
};

UENUM(BlueprintType)
enum class EHostServerType : uint8
{
	None,
	Private,
	Public
};

UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	Local,
	Global,
	Orbital,
	Velocity
};

UENUM(BlueprintType)
enum class ETransmissionMode : uint8
{
	Manual,
	Automatic
};

UENUM(BlueprintType)
enum class EMeasurementSystem : uint8
{
	Metric,
	Imperial,
	Max
};

UENUM(BlueprintType)
enum class EHUDVisibility : uint8
{
	Full,
	// Full HUD, everything should be shown
	Minimal,
	// Minimal HUD, only gameplay critical info should be shown
	Off,
	// Screen is completely free of HUD elements, except inventory, chat etc. which only open when the player wants to
	Max
};

UENUM(BlueprintType)
enum class EBrickUnitsDisplayMode : uint8
{
	BrickUnits,
	DistanceUnits
};

UENUM(BlueprintType)
enum class EColorDisplayMode : uint8
{
	HSV,
	RGB
};
