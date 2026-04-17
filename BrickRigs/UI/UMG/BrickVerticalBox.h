// Copyright Fluppisoft, 2022

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/VerticalBox.h"
#include "BrickVerticalBox.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickVerticalBox : public UVerticalBox
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
	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetSlotSpacingStyle(EBrickUISpacingStyle NewStyle);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Callback for the slate widget
	float GetSlotSpacing() const;
};
