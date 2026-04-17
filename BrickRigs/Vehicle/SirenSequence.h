// Copyright Fluppisoft, 2016

#pragma once

#include "UObject/Object.h"
#include "SirenSequence.generated.h"

USTRUCT()
struct FSirenSequenceChannel
{
	GENERATED_BODY()

	// Whether the light should be turned on when the sequence starts
	UPROPERTY(EditAnywhere)
	bool bStartActivated;

	UPROPERTY(EditAnywhere)
	TArray<float> Timings;

	FSirenSequenceChannel()
		: bStartActivated(true)
	{
	}
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API USirenSequence : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	FText DisplayNameAppendix;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	FText DisplayCategory;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	TArray<FSirenSequenceChannel> Channels;

	USirenSequence();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~Super Interface

	// Evaluate the flash sequence and return whether the light should be on at the given time
	bool EvalSequence(float Time) const;
	// Returns the name to be displayed
	FText GetDisplayName() const
	{
		return FText::Format(FText::AsCultureInvariant("{0} {1}"), DisplayName, DisplayNameAppendix);
	}
};
