// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "PropertyListInterface.h"
#include "PropertyWidget.h"
#include "StructPropertyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UStructPropertyWidget : public UPropertyWidget, public IPropertyListInterface
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual void UpdateProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo, const FBrickPropertyChangedEvent* ChangedEvent) override;
	// ~Super Interface
};
