// Copyright Fluppisoft, 2022

#pragma once

#include "BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BrickEditorSkeletalMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorSkeletalMeshComponent : public USkeletalMeshComponent, public IBrickEditorComponentInterface
{
	GENERATED_BODY()
};

struct FBrickEditorSkeletalMeshComponentParams : public FBrickEditorMeshComponentParams
{
	USkeletalMesh* SkeletalMesh;
	TSubclassOf<UAnimInstance> AnimClass;
	FSingleAnimationPlayData AnimationData;
	TEnumAsByte<EAnimationMode::Type> AnimationMode;

	FBrickEditorSkeletalMeshComponentParams(const UBrickEditorSkeletalMeshComponent* Default = GetDefault<UBrickEditorSkeletalMeshComponent>())
		: FBrickEditorMeshComponentParams(Default),
		  SkeletalMesh(Default->SkeletalMesh),
		  AnimClass(Default->AnimClass),
		  AnimationData(Default->AnimationData),
		  AnimationMode(Default->GetAnimationMode())
	{
	}

	void InitializeComponent(UBrickEditorSkeletalMeshComponent* Comp)
	{
		FBrickEditorMeshComponentParams::InitializeComponent(Comp);

		Comp->SetSkeletalMesh(SkeletalMesh);
		Comp->AnimClass = AnimClass;
		Comp->AnimationData = AnimationData;
		Comp->SetAnimationMode(AnimationMode);
	}
};
