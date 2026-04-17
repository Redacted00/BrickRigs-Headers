// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Bricks/Misc/ScalableBrickConnectorSpacing.h"
#include "UI/Properties/PropertyWidget.h"
#include "Vehicle/BrickConnection.h"
#include "ConnectorSpacingPropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UConnectorSpacingPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

	// ~Variables
	FScalableBrickConnectorSpacing ConnectorSpacing;
	// ~Variables

public:
	// ~Super Interface
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual FName GetFocusedSubProperty(const FWidgetPath& FocusedWidgetPath) const override;
	// ~Super Interface

	// Returns the selected spacing for the given axis
	UFUNCTION(BlueprintPure)
	EConnectorSpacing GetAxisSpacing(const int32 Axis, const bool bPositiveAxis) const;
	// Cycles the value of a certain axis
	UFUNCTION(BlueprintCallable)
	void CycleAxisSpacing(const int32 Axis, const bool bPositiveAxis);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateAxisSpacing(const int32 Axis, const bool bPositiveAxis, const EConnectorSpacing Spacing);
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetAxisWidget(const int32 Axis, const bool bPositiveAxis) const;
	// ~Blueprint Interface

private:
	// Executes a custom function for each axis and direction
	static void ForEachAxis(const TFunction<void(EAxis::Type, bool, EConnectorDirection)>& Func);
};
