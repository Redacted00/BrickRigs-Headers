#pragma once

#include "UI/Misc/PagedListEntry.h"
#include "GameFramework/OnlineReplStructs.h"
#include "KickedPlayer.generated.h"

UENUM(BlueprintType)
enum class EKickedPlayerSortMethod : uint8
{
	PlayerName,
	PlayerNameAsc,
	KickDate,
	KickDateAsc,
	TimeRemaining,
	TimeRemainingAsc
};

USTRUCT(BlueprintType)
struct FKickedPlayer : public FPagedListEntry
{
	GENERATED_BODY()

	constexpr static int32 MaxKickReasonLength = 100;

	UPROPERTY(BlueprintReadOnly)
	FUniqueNetIdRepl PlayerId;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;
	UPROPERTY(BlueprintReadOnly)
	FDateTime KickTime = FDateTime(0);
	UPROPERTY(BlueprintReadOnly)
	FTimespan KickDuration;
	UPROPERTY(BlueprintReadOnly)
	FString KickReason;

	// ~Constructor
	FKickedPlayer() = default;
	// ~Destructor
	virtual ~FKickedPlayer() override = default;

	// ~Constructor
	FKickedPlayer(const FUniqueNetIdRepl& InPlayerId, const FString& InPlayerName)
		: PlayerId(InPlayerId), PlayerName(InPlayerName)
	{
	}

	// ~Super Interface
	virtual UStruct* GetStruct() const override
	{
		return StaticStruct();
	}

	virtual FText GetDisplayName() const override
	{
		return FText::AsCultureInvariant(PlayerName);
	}

	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	static bool IsValidKickReason(const FString& InReason)
	{
		return InReason.Len() <= MaxKickReasonLength;
	}

	void UpdateKick(const FString& InReason, const FTimespan& InDuration)
	{
		KickReason = InReason;
		KickDuration = InDuration;
		KickTime = FDateTime::UtcNow();
	}

	bool IsInfinite() const
	{
		return KickDuration == FTimespan::MaxValue();
	}

	FTimespan GetKickTimeRemaining() const
	{
		return FMath::Max(KickDuration - (FDateTime::UtcNow() - KickTime), FTimespan::Zero());
	}

	// Get the remaining kick time as a text
	FText GetKickTimeRemainingText() const;

	// Return whether this kick/ban has expired by now
	bool HasExpired() const
	{
		return GetKickTimeRemaining() == FTimespan::Zero();
	}

	// Used for sorting the player list
	bool Sort(const FKickedPlayer& Other, EKickedPlayerSortMethod SortMethod) const
	{
		switch (SortMethod)
		{
		case EKickedPlayerSortMethod::PlayerName:
			return PlayerName < Other.PlayerName;
		case EKickedPlayerSortMethod::PlayerNameAsc:
			return PlayerName > Other.PlayerName;
		case EKickedPlayerSortMethod::KickDate:
			return KickTime < Other.KickTime;
		case EKickedPlayerSortMethod::KickDateAsc:
			return KickTime > Other.KickTime;
		case EKickedPlayerSortMethod::TimeRemaining:
			return GetKickTimeRemaining() < Other.GetKickTimeRemaining();
		case EKickedPlayerSortMethod::TimeRemainingAsc:
			return GetKickTimeRemaining() > Other.GetKickTimeRemaining();
		default:
			ensure(false);
			return false;
		}
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << PlayerId;
		Ar << PlayerName;
		Ar << KickTime;
		Ar << KickReason;
		Ar << KickDuration;

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FKickedPlayer> : TStructOpsTypeTraitsBase2<FKickedPlayer>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

// Used to replicate a list of kicked player infos
USTRUCT()
struct FKickedPlayerList
{
	GENERATED_BODY()

	FKickedPlayerList(const TArray<FKickedPlayer>& InPlayers = TArray<FKickedPlayer>(), int32 InTotalNumPlayers = 0)
		: Players(InPlayers), TotalNumPlayers(InTotalNumPlayers)
	{
	}

	const auto& GetPlayers() const
	{
		return Players;
	}

	int32 GetTotalNumPlayers() const
	{
		return TotalNumPlayers;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		auto NumPlayers = Players.Num();
		Ar << NumPlayers;

		if (Ar.IsLoading())
		{
			Players.SetNum(NumPlayers);
		}

		for (auto i = 0; i < NumPlayers; ++i)
		{
			Players[i].NetSerialize(Ar, Map, bOutSuccess);
		}

		return true;
	}

private:
	// The list of kicked players
	TArray<FKickedPlayer> Players;
	// Total number of kicked players
	int32 TotalNumPlayers;
};

template <>
struct TStructOpsTypeTraits<FKickedPlayerList> : TStructOpsTypeTraitsBase2<FKickedPlayerList>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

// Used to share kicked player list request params
USTRUCT()
struct FKickedPlayerListRequest
{
	GENERATED_BODY()

	constexpr static int32 MaxSearchTextLength = 20;
	constexpr static int32 MaxPlayersPerPage = 50;

	bool IsValidRequest() const
	{
		return SearchText.Len() < MaxSearchTextLength;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Page;
		Ar << SearchText;
		Ar << SortMethod;
		Ar << SearchPlayerIds;

		return true;
	}

	// The page to return
	uint16 Page;
	// Custom search text
	FString SearchText;
	// List of player IDs to query
	TArray<FUniqueNetIdRepl> SearchPlayerIds;
	// Sort method to use
	EKickedPlayerSortMethod SortMethod;
};

template <>
struct TStructOpsTypeTraits<FKickedPlayerListRequest> : TStructOpsTypeTraitsBase2<FKickedPlayerList>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
