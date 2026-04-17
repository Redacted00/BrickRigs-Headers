// Copyright Fluppisoft, 2018

#pragma once

#include "BrickEditor/Components/BrickEditorComponentInterface.h"
#include "InteractionOptions.h"
#include "CoreMinimal.h"
#include "Components/HUDIconComponent.h"
#include "InteractionComponent.generated.h"

class ABrickPlayerController;

// This component can be added to actors to achieve a 3D interactive point
UCLASS(ClassGroup = (BrickRigs), meta = (BlueprintSpawnableComponent))
class BRICKRIGS_API UInteractionComponent : public UHUDIconComponent
{
	GENERATED_BODY()

public:
	// ~Properties
	// The color style of the interaction icon
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	EBrickUIColorStyle ColorStyle;
	// ~Properties

	// ~Constructor
	UInteractionComponent();

	// ~Super Interface
	virtual bool ShouldIconBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const override;
	// ~Super Interface
};

struct FBrickEditorInteractionComponentParams : public FBrickEditorHUDIconComponentParams
{
	// ~Variables
	EBrickUIColorStyle ColorStyle;
	// ~Variables

	FBrickEditorInteractionComponentParams(const UInteractionComponent* Default = GetDefault<UInteractionComponent>())
		: FBrickEditorHUDIconComponentParams(Default),
		  ColorStyle(Default->ColorStyle)
	{
	}

	void InitializeComponent(UInteractionComponent* Comp)
	{
		FBrickEditorHUDIconComponentParams::InitializeComponent(Comp);

		Comp->ColorStyle = ColorStyle;
	}
};
