// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/OnlineReplStructs.h"
#include "UGC/UGCTypes.h"
#include "UI/Misc/BrickUIStyleTypes.h"
#include "BrickChatMessage.generated.h"

class ABrickPlayerController;
class ABrickPlayerState;
class ABrickVehicle;

UENUM(BlueprintType)
enum class EChatMessageType : uint8
{
	None,
	Message,
	Join,
	Leave,
	Kick,
	// Banned player tried to join
	JoinBanned,
	Unban,
	Death,
	MatchSettings,
	VehicleSpawnAttempt,
	VehicleSpawnSuccess,
	VehicleSpawnFailure
};

enum class EChatMessageImportance : uint8
{
	None,
	Notify,
	Critical
};

UENUM(BlueprintType)
enum class EChatContext : uint8
{
	None,
	Global,
	Team,
	Admin,
	Max
};

USTRUCT()
struct FChatMessagePlayerInfo
{
	GENERATED_BODY()

	// ~Variables
	UPROPERTY()
	FUniqueNetIdRepl PlayerId;
	UPROPERTY()
	FString PlayerName;
	// ~Variables

	// ~Constructor
	explicit FChatMessagePlayerInfo(const ABrickPlayerController* PC = nullptr);

	bool IsValid() const
	{
		return PlayerId.IsValid();
	}
};

USTRUCT()
struct FBrickChatMessage
{
	GENERATED_BODY()

	// ~Statics
	constexpr static int32 MaxPlayerMessageLength = 150;

	// Whether the given player message is in a valid format
	static bool IsValidPlayerMessage(const FText& InMessage)
	{
		return InMessage.ToString().Len() <= MaxPlayerMessageLength;
	}

	static FText GetChatContextDisplayName(EChatContext InContext);

	// Message factories
	static FBrickChatMessage MakePlayerMessage(const ABrickPlayerController* PC, const EChatContext Context, const FText& Message, const FGenericTeamId& TeamId);
	static FBrickChatMessage MakeKickMessage(const FUniqueNetIdRepl& PlayerId, const FString& PlayerName);
	static FBrickChatMessage MakeJoinBannedMessage(const FUniqueNetIdRepl& PlayerId, const FString& PlayerName);
	static FBrickChatMessage MakeUnbanMessage(const FUniqueNetIdRepl& PlayerId, const FString& PlayerName);
	static FBrickChatMessage MakeDeathMessage(const ABrickPlayerController* VictimPC, const ABrickPlayerController* KillerPC);
	static FBrickChatMessage MakeVehicleSpawnMessage(const ABrickPlayerController* PC, const FUGCFileInfo& FileInfo, const EChatMessageType Type);
	// ~Statics

private:
	// ~Variables
	UPROPERTY()
	EChatMessageType Type;
	UPROPERTY()
	FChatMessagePlayerInfo Player;
	UPROPERTY()
	FChatMessagePlayerInfo OtherPlayer;
	UPROPERTY()
	FText TextOption;
	UPROPERTY()
	int32 IntOption = 0;
	UPROPERTY()
	FGenericTeamId TeamOption;
	UPROPERTY()
	FFluUGCItemIdWrapper OnlineUGCItemIdOption;
	// ~Variables

public:
	// ~Constructor
	explicit FBrickChatMessage(const EChatMessageType Type = EChatMessageType::None, const ABrickPlayerController* PC = nullptr)
		: Type(Type), Player(PC)
	{
	}

	// Whether the given player should see this message
	bool CanPlayerReceiveMessage(const ABrickPlayerController* PC) const;

	// Whether this message should be logged
	bool ShouldLogMessage() const;

	// Returns the message as a display text
	FText GetMessageText() const;

	// Returns the importance of the message type
	EChatMessageImportance GetMessageImportance() const;

	// Returns the desired color style of the message
	EBrickUIColorStyle GetMessageColorStyle(const ABrickPlayerController* PC) const;

	// Returns the player associated to this message who could be kicked
	FChatMessagePlayerInfo GetPlayerToKick() const;

	// Returns the vehicle that can be viewed in the browser
	FFluUGCItemIdWrapper GetVehicleToView() const;

private:
	// Returns the chat context, applicable for player messages only
	EChatContext GetChatContext() const;

	FText GetPlayerMessageText() const;
	FText GetDeathMessageText() const;
	FText GetVehicleSpawnMessageText() const;
};
