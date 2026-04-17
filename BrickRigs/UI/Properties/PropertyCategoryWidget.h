// Copyright Fluppisoft, 2019

#pragma once

#include "PropertyListInterface.h"
#include "Blueprint/UserWidget.h"
#include "PropertyCategoryWidget.generated.h"

class UPropertyContainerWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPropertyCategoryWidget : public UUserWidget, public IPropertyListInterface
{
	GENERATED_BODY()

	// ~Variables
	int32 CategoryIndex;
	// ~Variables

public:
	void SetCategoryIndex(int32 Index)
	{
		CategoryIndex = Index;
	}

	auto GetCategoryIndex() const
	{
		return CategoryIndex;
	}

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCategory(const FText& DisplayName);
	// ~Blueprint Interface
};
