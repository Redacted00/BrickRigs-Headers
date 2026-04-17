#pragma once

#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "PagedListEntry.generated.h"

class ABrickPlayerController;

// This is the base struct of anything that can be displayed on a paged list
USTRUCT()
struct FPagedListEntry : public FBrickStructPropertyInterface
{
	GENERATED_BODY()

	// ~Super Interface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Has to be implemented for type checking
	virtual UStruct* GetStruct() const
	{
		return StaticStruct();
	}

	// Implement to return the display name
	virtual FText GetDisplayName() const
	{
		return FText();
	}

	// Optionally implement to return the displayed dimensions
	virtual FVector GetDimensions() const
	{
		return FVector::ZeroVector;
	}

	// Optionally return the maximum dimensions
	virtual FVector GetMaxDimensions(const ABrickPlayerController* PC) const
	{
		return FVector::ZeroVector;
	}

	// Optionally implement to return the total mass
	virtual float GetMass() const
	{
		return -1.f;
	}

	// Optionally return the maximum mass
	virtual float GetMaxMass(const ABrickPlayerController* PC) const
	{
		return 0.f;
	}

	// Optionally implement to return the number of objects/bricks used
	virtual int32 GetNumObjects() const
	{
		return 0;
	}

	// Optionally implement to return the object limit
	virtual int32 GetMaxNumObjects(const ABrickPlayerController* PC) const
	{
		return 0;
	}

	// Optionally implement to return the entry price
	virtual float GetPrice(const ABrickPlayerController* PC) const
	{
		return 0.f;
	}
};
