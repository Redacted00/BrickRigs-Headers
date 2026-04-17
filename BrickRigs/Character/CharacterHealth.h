#pragma once

#include "Online/BitfieldSerializer.h"
#include "Serialization/SerializationHelper.h"
#include "CoreMinimal.h"
#include "CharacterHealth.generated.h"

// Indicates the 'aliveness' of characters
UENUM(BlueprintType)
enum class ECharacterStateOfHealth : uint8
{
	Conscious,
	Unconscious,
	Dead
};

UENUM(BlueprintType)
enum class ECharacterHealingState : uint8
{
	NotHealing,
	Healing,
	HealingFirstAidKit
};

USTRUCT()
struct FCharacterHealth
{
	GENERATED_BODY()
	;

private:
	// ~Variables
	// Health percentage, from 0-1
	float Health;
	// Indicates if the character is dead, unconscious etc, independently from the health
	// Since the health is compressed we can't rely on it being accurate
	ECharacterStateOfHealth StateOfHealth;
	// Indicates if and how the character is healing
	ECharacterHealingState HealingState;
	// Last bone that was hit before death
	FName LastHitBone;
	// Damage impulse vector applied to the bone that was last hit
	FVector_NetQuantize100 LastDamageImpulse;
	// ~Variables

public:
	// ~Constructor
	FCharacterHealth()
		: Health(1.f), StateOfHealth(ECharacterStateOfHealth::Conscious), HealingState(ECharacterHealingState::NotHealing), LastHitBone(NAME_None), LastDamageImpulse(FVector::ZeroVector)
	{
	}

	bool operator==(const FCharacterHealth& Other) const
	{
		return Health == Other.Health
			&& StateOfHealth == Other.StateOfHealth
			&& HealingState == Other.HealingState
			&& LastDamageImpulse == Other.LastDamageImpulse
			&& LastHitBone == Other.LastHitBone;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		// Compress into a bitfield
		auto Bitfield = TBitfieldSerializer<uint16>(Ar);

		// Compress the health into 8 bits
		// NOTE: Make sure the values of 0 and 255 are reserved for full or no health
		auto CompressedHealth = static_cast<uint8>(Health <= 0.f ? 0 : Health >= 1.f ? 255 : FMath::RoundToInt(FMath::GetMappedRangeValueUnclamped({0.f, 1.f}, {1.f, 254.f}, Health)));
		Bitfield.Add(CompressedHealth, 8);
		Bitfield.Add(StateOfHealth, 2);
		Bitfield.Add(HealingState, 2);
		Bitfield.Finish();

		if (Ar.IsLoading())
		{
			// Decrompress the health
			Health = FMath::GetMappedRangeValueUnclamped({0.f, 255.f}, {0.f, 1.f}, CompressedHealth);
		}

		// Serialize the last damage info, but only if the character isn't fully alive
		// IMPORTANT: Do this after serializing the flags
		if (StateOfHealth != ECharacterStateOfHealth::Conscious)
		{
			LastDamageImpulse.NetSerialize(Ar, Map, bOutSuccess);
			Ar << LastHitBone;
		}

		return true;
	}

	float GetHealth() const
	{
		return Health;
	}

	void SetHealth(float NewHealth)
	{
		check(NewHealth >= 0.f && NewHealth <= 1.f);
		Health = NewHealth;
	}

	auto GetStateOfHealth() const
	{
		return StateOfHealth;
	}

	void SetStateOfHealth(ECharacterStateOfHealth NewState)
	{
		StateOfHealth = NewState;
	}

	auto GetHealingState() const
	{
		return HealingState;
	}

	void SetHealingState(ECharacterHealingState NewState)
	{
		HealingState = NewState;
	}

	FName GetLastHitBone() const
	{
		return LastHitBone;
	}

	FVector GetLastDamageImpulse() const
	{
		return LastDamageImpulse;
	}

	void SetLastHitInfo(const FName& InBoneName, const FVector& InImpulse)
	{
		LastHitBone = InBoneName;
		LastDamageImpulse = InImpulse;
	}
};

template <>
struct TStructOpsTypeTraits<FCharacterHealth> : public TStructOpsTypeTraitsBase2<FCharacterHealth>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};
