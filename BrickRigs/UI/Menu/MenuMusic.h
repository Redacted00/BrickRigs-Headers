// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MenuMusic.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UMenuMusic : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundWave> Sound;
	// ~Properties
};
