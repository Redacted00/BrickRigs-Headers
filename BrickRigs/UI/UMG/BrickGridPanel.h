// Copyright Fluppisoft, 2022

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/GridPanel.h"
#include "BrickGridPanel.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickGridPanel : public UGridPanel
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUISpacingStyle SlotSpacingStyle;

	// Column fill value to use when there is no explicit rule for the colum
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fill Rules")
	float DefaultColumnFill = 0.f;

	// Row fill value to use when there is no explicit rule for the row
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fill Rules")
	float DefaultRowFill = 0.f;
	// ~Properties

public:
	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetSlotSpacingStyle(const EBrickUISpacingStyle NewStyle);

	UFUNCTION(BlueprintCallable)
	void SetDefaultColumnFill(const float Coefficient);

	UFUNCTION(BlueprintCallable)
	void SetDefaultRowFill(const float Coefficient);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Callback for the slate widget
	FVector2D GetSlotSpacing() const;
};
