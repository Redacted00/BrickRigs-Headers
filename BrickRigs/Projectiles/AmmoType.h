#pragma once

#include "GameplayTags.h"
#include "AmmoType.generated.h"

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Default,
	Incendiary,
	HighExplosive,
	TargetSeeking,
	Guided,
	Max
};

struct FAmmoType
{
	static bool IsExplosive(EAmmoType Type)
	{
		return Type == EAmmoType::HighExplosive || Type == EAmmoType::TargetSeeking || Type == EAmmoType::Guided || Type == EAmmoType::Incendiary;
	}
};

struct FAmmoInfo
{
	FGameplayTag Caliber;
	EAmmoType Type;

	FAmmoInfo(const FGameplayTag& InCaliber, EAmmoType InType)
		: Caliber(InCaliber), Type(InType)
	{
	}

	bool operator==(const FAmmoInfo& Other) const
	{
		return Caliber == Other.Caliber && Type == Other.Type;
	}
};
