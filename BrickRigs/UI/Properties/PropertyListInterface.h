// Copyright Fluppisoft, 2023

#pragma once

#include "UObject/Interface.h"
#include "PropertyListInterface.generated.h"

UINTERFACE()
class BRICKRIGS_API UPropertyListInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BRICKRIGS_API IPropertyListInterface
{
	GENERATED_IINTERFACE_BODY()

private:
	// ~Variables
	// Number of properties that have been added so far
	int32 NumProperties = 0;
	// ~Variables

public:
	auto GetNumProperties() const
	{
		return NumProperties;
	}

	void SetNumProperties(int32 Num)
	{
		NumProperties = Num;
	}

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddPropertyContainerWidget(UPropertyContainerWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePropertyContainerWidgetSlot(UPropertyContainerWidget* Widget, int32 Index, int32 NumPerRow);
	// ~Blueprint Interface
};
