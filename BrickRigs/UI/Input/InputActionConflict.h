#pragma once

#include "InputActionAndAxisMapping.h"
#include "CoreMinimal.h"

struct FInputActionConflict
{
	FInputActionAndAxisMapping Mapping;
	bool bOtherNegativeAxis;

	FInputActionConflict(const FInputActionAndAxisMapping& InMapping, bool bInOtherNegativeAxis)
		: Mapping(InMapping), bOtherNegativeAxis(bInOtherNegativeAxis)
	{
	}

	// Needed so this can be used as a map key
	bool operator==(const FInputActionConflict& Other) const
	{
		return Mapping == Other.Mapping && bOtherNegativeAxis == Other.bOtherNegativeAxis;
	}
};

using FInputActionConflicts = TMap<FName, TArray<FInputActionConflict>>;
using FInputActionConflictsMap = TMap<FName, FInputActionConflicts>;
