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
	Flare,
	Max
};

struct FAmmoType
{
	static bool IsExplosive(EAmmoType Type)
	{
		return Type == EAmmoType::HighExplosive || Type == EAmmoType::TargetSeeking || Type == EAmmoType::Guided || Type == EAmmoType::Incendiary;
	}
};

USTRUCT(BlueprintType)
struct FAmmoInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag Caliber;
	UPROPERTY(BlueprintReadWrite)
	EAmmoType Type;

	// ~Constructor
	explicit FAmmoInfo(const FGameplayTag& InCaliber = FGameplayTag::EmptyTag, const EAmmoType InType = EAmmoType::Default)
		: Caliber(InCaliber), Type(InType)
	{
	}

	bool operator==(const FAmmoInfo& Other) const
	{
		return Caliber == Other.Caliber && Type == Other.Type;
	}
};
