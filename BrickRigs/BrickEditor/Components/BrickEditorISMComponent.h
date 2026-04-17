// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "BrickEditorComponentInterface.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BrickEditorISMComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorISMComponent : public UInstancedStaticMeshComponent, public IBrickEditorComponentInterface
{
	GENERATED_BODY()
};

struct FBrickEditorISMComponentParams : public FBrickEditorMeshComponentParams
{
	UStaticMesh* StaticMesh;
	int32 NumCustomDataFloats;

	FBrickEditorISMComponentParams(const UBrickEditorISMComponent* Default = GetDefault<UBrickEditorISMComponent>())
		: FBrickEditorMeshComponentParams(Default),
		  StaticMesh(Default->GetStaticMesh()),
		  NumCustomDataFloats(Default->NumCustomDataFloats)
	{
	}

	void InitializeComponent(UBrickEditorISMComponent* Comp)
	{
		FBrickEditorMeshComponentParams::InitializeComponent(Comp);

		Comp->ClearInstances();
		Comp->SetStaticMesh(StaticMesh);
		Comp->NumCustomDataFloats = NumCustomDataFloats;
	}
};
