#pragma once

#include "Misc/FluTeamIdStatics.h"
#include "CoreMinimal.h"
#include "MatchWinner.generated.h"

/**
* This struct is used to pass along and replicate the winner of a match more easily
*/
USTRUCT(BlueprintType)
struct FMatchWinner
{
	GENERATED_BODY()

protected:
	// ~Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGenericTeamId WinnerTeamId;
	// ~Variables

public:
	// ~Constructor
	FMatchWinner(const FGenericTeamId& InWinnerTeamId = FGenericTeamId())
		: WinnerTeamId(InWinnerTeamId)
	{
	}

	static FMatchWinner Team(const FGenericTeamId& InWinnerTeamId)
	{
		return FMatchWinner(InWinnerTeamId);
	}

	static FMatchWinner None()
	{
		return FMatchWinner();
	}

	const FGenericTeamId& GetWinnerTeamId() const
	{
		return WinnerTeamId;
	}

	bool operator==(const FMatchWinner& Other) const
	{
		return WinnerTeamId == Other.WinnerTeamId;
	}

	bool operator!=(const FMatchWinner& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		FFluTeamIdStatics::SerializeTeamId(Ar, WinnerTeamId);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FMatchWinner> : TStructOpsTypeTraitsBase2<FMatchWinner>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
