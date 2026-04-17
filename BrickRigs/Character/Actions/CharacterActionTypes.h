#pragma once

#include "CoreMinimal.h"
#include "CharacterActionTypes.generated.h"

UENUM()
enum class ECharacterActionLayer : uint8
{
	UpperBody,
	// Action takes place on the upper body (default for tool animations for example)
	LowerBody,
	// Actions that are for example movement related but don't require the arms to be used
	FullBody // Body affects the entire body, nothing else can happen during such action
};

UENUM()
enum class ECharacterActionAuth : uint8
{
	Local,
	// Everybody can start and stop these types of actions, not replicated to anyone
	ClientAuth,
	// Cient has full authority
	ServerAuth,
	// Server has sole authority
};
