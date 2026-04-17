#pragma once

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
