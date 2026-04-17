#pragma once

#include "Serialization/FluSerializationStatics.h"
#include "CharacterDamageInfo.generated.h"

// This struct is used to replicate damage events to clients
USTRUCT()
struct FCharacterDamageInfo
{
	GENERATED_BODY()

public:
	// Direction the damage impulse came from in 0-360�
	float HitDirection;
	// Whether a specific direction is set, otherwise assumed to be non directional
	bool bHitDirectionSet;
	// The damage that was dealt
	float Damage;

	FCharacterDamageInfo(float InDamage = 0.f)
		: HitDirection(0.f), bHitDirectionSet(false), Damage(FMath::Clamp(InDamage, 0.f, 1.f))
	{
	}

	void SetHitDirection(float Direction)
	{
		HitDirection = FRotator::ClampAxis(Direction);
		bHitDirectionSet = true;
	}

	void SetHitDirectionFromVector(const FVector& Direction)
	{
		SetHitDirection(Direction.Rotation().Yaw);
	}

	float GetRelativeHitDirection(const FRotator& RefRotation) const
	{
		if (bHitDirectionSet)
		{
			return HitDirection - RefRotation.Yaw;
		}

		return 0.f;
	}

	float GetWorldHitDirection(const FRotator& Fallback) const
	{
		if (bHitDirectionSet)
		{
			return HitDirection;
		}

		return Fallback.Yaw;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		FFluSerializationStatics::SerializeFloatCompressed<uint8>(Ar, Damage, 0.f, 1.f);

		Ar << bHitDirectionSet;
		if (bHitDirectionSet)
		{
			FFluSerializationStatics::SerializeFloatCompressed<uint8>(Ar, HitDirection, 0.f, 360.f);
		}

		return !Ar.IsError();
	}

	bool operator==(const FCharacterDamageInfo& Other) const
	{
		return HitDirection == Other.HitDirection && Damage == Other.Damage && bHitDirectionSet == Other.bHitDirectionSet;
	}
};

template <>
struct TStructOpsTypeTraits<FCharacterDamageInfo> : public TStructOpsTypeTraitsBase2<FCharacterDamageInfo>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};
