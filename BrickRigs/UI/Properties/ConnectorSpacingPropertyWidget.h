// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/Misc/ScalableBrickConnectorSpacing.h"
#include "UI/Properties/PropertyWidget.h"
#include "ConnectorSpacingPropertyWidget.generated.h"

struct FScalableBrickConnectorSpacing;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UConnectorSpacingPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void UpdateValue(const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetConnectorSpacing(const FScalableBrickConnectorSpacing& NewSpacing);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateConnectorSpacing(const FScalableBrickConnectorSpacing& NewSpacing, const uint8 AxisFlags);
	// ~Blueprint Interface
};
