// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "BrickEditorStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorStaticMeshComponent : public UStaticMeshComponent, public IBrickEditorComponentInterface
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void BeginDestroy() override;
	// ~Super Interface
};

struct FBrickEditorStaticMeshComponentParams : FBrickEditorMeshComponentParams
{
	UStaticMesh* StaticMesh;
	int32 ForcedLodModel;
	bool bEvaluateWorldPositionOffset;

	FBrickEditorStaticMeshComponentParams(const UBrickEditorStaticMeshComponent* Default = GetDefault<UBrickEditorStaticMeshComponent>())
		: FBrickEditorMeshComponentParams(Default),
		  StaticMesh(Default->GetStaticMesh()),
		  ForcedLodModel(Default->ForcedLodModel),
		  bEvaluateWorldPositionOffset(Default->bEvaluateWorldPositionOffset)
	{
	}

	void InitializeComponent(UBrickEditorStaticMeshComponent* Comp)
	{
		FBrickEditorMeshComponentParams::InitializeComponent(Comp);

		Comp->SetStaticMesh(StaticMesh);
		Comp->ForcedLodModel = ForcedLodModel;
		Comp->bEvaluateWorldPositionOffset = bEvaluateWorldPositionOffset;
	}
};
