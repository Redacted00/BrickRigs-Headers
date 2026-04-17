// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BrickEditor/Components/BrickEditorStaticMeshComponent.h"
#include "BrickStaticMeshComponent.generated.h"

/**
 * The static mesh component specialized for bricks
 */
UCLASS()
class BRICKRIGS_API UBrickStaticMeshComponent : public UBrickEditorStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Custom visual 3D scaling
	FVector RenderScale3D = FVector::OneVector;
	// ~Variables

	// ~Constructor
	UBrickStaticMeshComponent();

	// ~Super Interface
	virtual void PostInitProperties() override;
	virtual bool ShouldCreatePhysicsState() const override;
	virtual FMatrix GetRenderMatrix() const override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	virtual bool UpdateOverlapsImpl(const TOverlapArrayView* NewPendingOverlaps = nullptr, bool bDoNotifies = true, const TOverlapArrayView* OverlapsAtEndLocation = nullptr) override;
	// ~Super Interface

	// Changes the custom scaling
	void SetRenderScale3D(const FVector& NewScale);
	// Changes the scale of the body setup shapes
	void SetBodySetupScale3D(const FVector& NewScale);
};

struct FBrickStaticMeshComponentParams : FBrickEditorStaticMeshComponentParams
{
	FVector RenderScale3D;

	FBrickStaticMeshComponentParams(const UBrickStaticMeshComponent* Default = GetDefault<UBrickStaticMeshComponent>())
		: FBrickEditorStaticMeshComponentParams(Default),
		  RenderScale3D(Default->RenderScale3D)
	{
	}

	void InitializeComponent(UBrickStaticMeshComponent* Comp)
	{
		FBrickEditorStaticMeshComponentParams::InitializeComponent(Comp);

		Comp->RenderScale3D = RenderScale3D;
	}
};
