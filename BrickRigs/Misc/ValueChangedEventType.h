#pragma once

#include "CoreMinimal.h"
#include "ValueChangedEventType.generated.h"

UENUM(BlueprintType)
enum class EValueChangedEventType : uint8
{
	Commit,
	Cancel,
	Pending
};
