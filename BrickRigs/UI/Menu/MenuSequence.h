// Copyright Fluppisoft, 2017

#pragma once

#include "MenuSequenceProperties.h"
#include "GameFramework/Actor.h"
#include "MenuSequence.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API AMenuSequence : public AActor
{
	GENERATED_BODY()

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = PlayConditions)
	FMenuSequenceProperties MenuSequenceProperties;
	// ~Properties

public:
	// ~Constructor
	AMenuSequence();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
	// ~Super Interface
};
