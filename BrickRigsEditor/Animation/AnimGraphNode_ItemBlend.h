// Copyright Fluppisoft, 2020

#pragma once

#include "BrickRigs/Character/Animation/AnimNode_ItemBlend.h"
#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "AnimGraphNode_ItemBlend.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGSEDITOR_API UAnimGraphNode_ItemBlend : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	// ~Properties
	// The own node
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ItemBlend Node;
	// ~Properties

	// ~Super Interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// ~Super Interface
};
