#pragma once

#include "GenericTeamAgentInterface.h"
#include "CoreMinimal.h"
#include "BrickTeamId.generated.h"

struct FBrickTeamId
{
	static FGenericTeamId MinTeamId;
	static FGenericTeamId MaxTeamId;
	static FGenericTeamId ZombieTeamId;
	static FGenericTeamId DummyTeamId;
	static FGenericTeamId AllTeams;

	// Called from the game instance Init() function
	static void Init()
	{
		// Setup the custom attitude solver
		FGenericTeamId::SetAttitudeSolver(&FBrickTeamId::TeamAttitudeSolver);
	}

	// Used by the generic team id to determine the team attitude
	static ETeamAttitude::Type TeamAttitudeSolver(FGenericTeamId TeamA, FGenericTeamId TeamB)
	{
		if (TeamA == FGenericTeamId::NoTeam || TeamA == DummyTeamId || TeamB == FGenericTeamId::NoTeam || TeamB == DummyTeamId)
		{
			return ETeamAttitude::Neutral;
		}

		return TeamA == TeamB ? ETeamAttitude::Friendly : ETeamAttitude::Hostile;
	}

	static ETeamAttitude::Type MultipleTeamAttitudeSolver(const TSet<FGenericTeamId>& TeamASet, const FGenericTeamId& TeamB)
	{
		ETeamAttitude::Type OutTeamAttitude = ETeamAttitude::Neutral;
		for (const FGenericTeamId& TeamA : TeamASet)
		{
			const ETeamAttitude::Type OtherAttitude = TeamAttitudeSolver(TeamA, TeamB);
			if (OtherAttitude == ETeamAttitude::Hostile)
			{
				// If any is hostile, all are hostile
				OutTeamAttitude = OtherAttitude;
				break;
			}
			if (OtherAttitude == ETeamAttitude::Friendly)
			{
				OutTeamAttitude = OtherAttitude;
			}
		}

		return OutTeamAttitude;
	}

	static FGenericTeamId GetTeamIdFromIndex(int32 TeamIndex)
	{
		const FGenericTeamId OutTeamId(MinTeamId + TeamIndex);
		ensure(OutTeamId <= MaxTeamId);
		return OutTeamId;
	}

	static int32 GetIndexFromTeamId(const FGenericTeamId& TeamId)
	{
		const int32 OutIndex = TeamId.GetId() - MinTeamId;
		ensure(OutIndex >= 0);
		return OutIndex;
	}

	static void SerializeTeamId(FArchive& Ar, FGenericTeamId& TeamId)
	{
		uint8 Byte = TeamId;
		Ar << Byte;
		if (Ar.IsLoading())
		{
			TeamId = Byte;
		}
	}
};

// Used to conveniently replicate a set of team ids
USTRUCT()
struct FGenericTeamIdSet
{
	GENERATED_BODY()

private:
	// ~Variables
	TSet<FGenericTeamId> Teams;
	// ~Variables

public:
	const auto& Get() const
	{
		return Teams;
	}

	void Set(const TSet<FGenericTeamId>& InTeams)
	{
		Teams = InTeams;
	}

	bool operator==(const FGenericTeamIdSet& Other) const
	{
		return *this == Other.Teams;
	}

	bool operator==(const TSet<FGenericTeamId>& OtherTeams) const
	{
		return Teams.Num() == OtherTeams.Num() && Teams.Includes(OtherTeams);
	}

	bool operator!=(const TSet<FGenericTeamId>& OtherTeams) const
	{
		return !(*this == OtherTeams);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* PackageMap, bool& bSuccess)
	{
		auto Num = static_cast<uint8>(Teams.Num());
		Ar << Num;

		if (Ar.IsSaving())
		{
			for (auto& TeamId : Teams)
			{
				FBrickTeamId::SerializeTeamId(Ar, TeamId);
			}
		}
		else
		{
			Teams.Empty(Num);

			for (auto i = 0; i < Num; ++i)
			{
				auto NewTeamId = FGenericTeamId();
				FBrickTeamId::SerializeTeamId(Ar, NewTeamId);
				Teams.Add(NewTeamId);
			}
		}

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FGenericTeamIdSet> : public TStructOpsTypeTraitsBase2<FGenericTeamIdSet>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};
