#pragma once

#include "Online/BitfieldSerializer.h"
#include "Serialization/FluSerializationStatics.h"
#include "CoreMinimal.h"
#include "ClientDamageInfo.generated.h"

// Contains the damage info reported from the server to clients
USTRUCT(BlueprintType)
struct FClientDamageInfo
{
	GENERATED_BODY()
	;

public:
	// ~Variables
	// The team attitude towards the hit player
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ETeamAttitude::Type> TeamAttitude;
	// The normalized damage amount
	UPROPERTY(BlueprintReadOnly)
	float Damage;
	// Whether the shot was a headshot
	UPROPERTY(BlueprintReadOnly)
	bool bIsHeadshot;
	// Whether this shot has killed the enemy
	UPROPERTY(BlueprintReadOnly)
	bool bIsKillshot;
	// ~Variables

	// ~Constructor
	FClientDamageInfo(ETeamAttitude::Type InTeamAttitude = ETeamAttitude::Neutral, float InDamage = 0.f, bool bInHeadshot = false, bool bInKillshot = false)
		: TeamAttitude(InTeamAttitude), Damage(FMath::Clamp(InDamage, 0.f, 1.f)), bIsHeadshot(bInHeadshot), bIsKillshot(bInKillshot)
	{
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		// Compress the damage
		FFluSerializationStatics::SerializeFloatCompressed<uint8>(Ar, Damage, 0.f, 1.f);
		// Compress the attitude and flags into one byte
		TBitfieldSerializer<uint8> Bitfield(Ar);
		Bitfield.Add(TeamAttitude, 2);
		Bitfield.Add(bIsHeadshot);
		Bitfield.Add(bIsKillshot);
		Bitfield.Finish();

		return !Ar.IsError();
	}

	FORCEINLINE bool operator==(const FClientDamageInfo& Other) const
	{
		return Damage == Other.Damage && bIsKillshot == Other.bIsKillshot;
	}
};

template <>
struct TStructOpsTypeTraits<FClientDamageInfo> : public TStructOpsTypeTraitsBase2<FClientDamageInfo>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
