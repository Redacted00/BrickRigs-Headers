// Copyright Fluppisoft, 2019

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "BrickRichTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickRichTextBlock : public URichTextBlock
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void UpdateStyleData() override;
	// ~Super Interface

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
};
