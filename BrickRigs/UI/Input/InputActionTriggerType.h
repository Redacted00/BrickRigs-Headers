#pragma once

#include "CoreMinimal.h"
#include "InputActionTriggerType.generated.h"

UENUM(BlueprintType)
enum class EInputActionTriggerType : uint8
{
	Press,
	// Action is executed upon pressing the key
	Tab,
	// Key has to be tabbed quickly
	Hold,
	// Key has to be held down for a certain time
	Axis,
	// A full float input axis
	AxisPositive,
	// The positive direction of a float axis
	AxisNegative,
	// The negative direction of a float axis
};
