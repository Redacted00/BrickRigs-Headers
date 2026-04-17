// Copyright Fluppisoft, 2017

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "UObject/NoExportTypes.h"
#include "SirenType.generated.h"

/**
 *
 */
UCLASS(Blueprintable)
class BRICKRIGS_API USirenType : public UObject, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	FText DisplayName;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	USoundBase* HornSound;
	UPROPERTY(EditDefaultsOnly, Category = Siren)
	TArray<USoundBase*> SirenSounds;
	// ~Properties

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~Super Interface

	// Returns the user friendly name
	FText GetDisplayName() const
	{
		return DisplayName;
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

	// ~IObjectPropertyItemInterface
};
