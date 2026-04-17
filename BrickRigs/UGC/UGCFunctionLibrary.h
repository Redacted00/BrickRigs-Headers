// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "UGCTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UGCFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UUGCFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Creates an online item id
	UFUNCTION(BlueprintPure)
	static FFluUGCItemIdWrapper MakeOnlineUGCItemId(FString ItemId);
};
