// Copyright Fluppisoft, 2017

#pragma once

#include "UI/BrickUIStyle.h"
#include "Components/ScrollBox.h"
#include "BrickScrollBox.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickScrollBox : public UScrollBox
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUISpacingStyle SlotSpacingStyle;
	// ~Properties

public:
	// ~Constructor
	UBrickScrollBox();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetSlotSpacingStyle(EBrickUISpacingStyle NewStyle);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Synchronizes the slot spacing property with the slate widget
	void SynchronizeSlotSpacing() const;
};
