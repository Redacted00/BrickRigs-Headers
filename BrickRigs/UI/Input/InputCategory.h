#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCategory.generated.h"

// Used to categorize input actions in the input and context menu
UCLASS(BlueprintType, HideCategories=Object)
class BRICKRIGS_API UInputCategory : public UDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly)
	FDisplayInfo DisplayInfo;
	// ~Properties

public:
	// Returns the user friendly name
	FText GetDisplayName() const
	{
		return DisplayInfo.Name;
	}

	// Used to sort input categories for display purposes
	bool SortInputCategory(const UInputCategory* Other) const
	{
		return GetName() < Other->GetName();
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return DisplayInfo;
	}

	// ~IObjectPropertyItemInterface
};
