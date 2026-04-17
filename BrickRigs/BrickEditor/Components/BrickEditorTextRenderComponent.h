// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Components/TextRenderComponent.h"
#include "BrickEditorTextRenderComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorTextRenderComponent : public UTextRenderComponent
{
	GENERATED_BODY()
};

struct FBrickEditorTextRenderComponentParams : public FBrickEditorPrimitiveComponentParams
{
	UFont* Font;
	UMaterialInterface* TextMaterial;
	EHorizTextAligment HorizontalAlignment;
	EVerticalTextAligment VerticalAlignment;

	FBrickEditorTextRenderComponentParams(const UBrickEditorTextRenderComponent* Default = GetDefault<UBrickEditorTextRenderComponent>())
		: FBrickEditorPrimitiveComponentParams(Default),
		  Font(Default->Font),
		  TextMaterial(Default->TextMaterial),
		  HorizontalAlignment(Default->HorizontalAlignment),
		  VerticalAlignment(Default->VerticalAlignment)
	{
	}

	void InitializeComponent(UBrickEditorTextRenderComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		Comp->Font = Font;
		Comp->SetTextMaterial(TextMaterial);
		Comp->HorizontalAlignment = HorizontalAlignment;
		Comp->VerticalAlignment = VerticalAlignment;
	}
};
