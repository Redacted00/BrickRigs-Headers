// Copyright Fluppisoft, 2020

#pragma once

#include "BrickRigs/Character/Animation/AnimNode_ViewRotation.h"
#include "CoreMinimal.h"
#include "AnimGraphNode_Base.h"
#include "AnimGraphNode_ViewRotation.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGSEDITOR_API UAnimGraphNode_ViewRotation : public UAnimGraphNode_Base
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Settings)
	FAnimNode_ViewRotation Node;

	// ~Super Interface
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual FText GetTooltipText() const override;
	// ~Super Interface
};
