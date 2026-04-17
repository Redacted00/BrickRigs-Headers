// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Components/ExpandableArea.h"
#include "UI/Misc/BrickUIStyleTypes.h"
#include "BrickExpandableArea.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickExpandableArea : public UExpandableArea
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

public:
	// ~Constructor
	UBrickExpandableArea();

	// ~Super Interface
	virtual void SynchronizeProperties() override;
	// ~Super Interface

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
};
