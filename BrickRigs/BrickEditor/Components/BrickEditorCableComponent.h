// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "BrickEditorComponentInterface.h"
#include "CableComponent.h"
#include "BrickEditorCableComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorCableComponent : public UCableComponent
{
	GENERATED_BODY()
};

struct FBrickEditorCableComponentParams : public FBrickEditorPrimitiveComponentParams
{
	bool bAttachStart;
	bool bAttachEnd;
	FComponentReference AttachEndTo;
	FName AttachEndToSocketName;
	FVector EndLocation;
	float CableLength;
	int32 NumSegments;
	float SubstepTime;
	int32 SolverIterations;
	bool bEnableStiffness;
	bool bUseSubstepping;
	bool bSkipCableUpdateWhenNotVisible;
	bool bSkipCableUpdateWhenNotOwnerRecentlyRendered;
	bool bEnableCollision;
	float CollisionFriction;
	FVector CableForce;
	float CableGravityScale;
	float CableWidth;
	int32 NumSides;
	float TileMaterial;
	UMaterialInterface* Material;

	FBrickEditorCableComponentParams(const UBrickEditorCableComponent* Default = GetDefault<UBrickEditorCableComponent>())
		: FBrickEditorPrimitiveComponentParams(Default),
		  bAttachStart(Default->bAttachStart),
		  bAttachEnd(Default->bAttachEnd),
		  AttachEndTo(Default->AttachEndTo),
		  AttachEndToSocketName(Default->AttachEndToSocketName),
		  EndLocation(Default->EndLocation),
		  CableLength(Default->CableLength),
		  NumSegments(Default->NumSegments),
		  SubstepTime(Default->SubstepTime),
		  SolverIterations(Default->SolverIterations),
		  bEnableStiffness(Default->bEnableStiffness),
		  bUseSubstepping(Default->bUseSubstepping),
		  bSkipCableUpdateWhenNotVisible(Default->bSkipCableUpdateWhenNotVisible),
		  bSkipCableUpdateWhenNotOwnerRecentlyRendered(Default->bSkipCableUpdateWhenNotOwnerRecentlyRendered),
		  bEnableCollision(Default->bEnableCollision),
		  CollisionFriction(Default->CollisionFriction),
		  CableForce(Default->CableForce),
		  CableGravityScale(Default->CableGravityScale),
		  CableWidth(Default->CableWidth),
		  NumSides(Default->NumSides),
		  TileMaterial(Default->TileMaterial),
		  Material(nullptr)
	{
	}

	void InitializeComponent(UBrickEditorCableComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		Comp->bAttachStart = bAttachStart;
		Comp->bAttachEnd = bAttachEnd;
		Comp->AttachEndTo = AttachEndTo;
		Comp->AttachEndToSocketName = AttachEndToSocketName;
		Comp->EndLocation = EndLocation;
		Comp->CableLength = CableLength;
		Comp->NumSegments = NumSegments;
		Comp->SubstepTime = SubstepTime;
		Comp->SolverIterations = SolverIterations;
		Comp->bEnableStiffness = bEnableStiffness;
		Comp->bUseSubstepping = bUseSubstepping;
		Comp->bSkipCableUpdateWhenNotVisible = bSkipCableUpdateWhenNotVisible;
		Comp->bSkipCableUpdateWhenNotOwnerRecentlyRendered = bSkipCableUpdateWhenNotOwnerRecentlyRendered;
		Comp->bEnableCollision = bEnableCollision;
		Comp->CollisionFriction = CollisionFriction;
		Comp->CableForce = CableForce;
		Comp->CableGravityScale = CableGravityScale;
		Comp->CableWidth = CableWidth;
		Comp->NumSides = NumSides;
		Comp->TileMaterial = TileMaterial;
		Comp->SetMaterial(0, Material);
	}
};
