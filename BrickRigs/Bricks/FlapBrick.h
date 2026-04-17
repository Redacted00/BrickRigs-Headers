// Copyright Fluppisoft, 2017

#pragma once

#include "Bricks/Brick.h"
#include "FlapBrick.generated.h"

struct FFlapBrickEditorParams : public FBrickEditorParams
{
	bool bFocusedMinAngle;
	bool bFocusedMaxAngle;
};

UCLASS(Abstract)
class BRICKRIGS_API UFlapBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The mesh to spawn for the movable flap
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	UStaticMesh* FlapMesh;
	// Rate at which the flap is moved
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	float FlapInterpSpeed;
	// Cached transform of the flap socket
	UPROPERTY(VisibleDefaultsOnly, Category = Flap)
	FTransform FlapSocketTransform;
	// ~Properties

	// ~Constructor
	UFlapBrickStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BuildCachedData() override;
	virtual UStaticMesh* GetFluidDynamicProxyMesh() const override;

	virtual void ProcessFluidDynamicProperties(const FVector& BoundsSize, const FVector& SurfaceAreas, const FVector& AverageLocation, const FVector* AverageNormals) override
	{
		// Double the surface size for legacy reasons
		Super::ProcessFluidDynamicProperties(BoundsSize, SurfaceAreas * 2.f, AverageLocation, AverageNormals);
	}
#endif
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UFlapBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// The mesh component used for the flap
	TBrickEditorComponentPtr<UBrickEditorStaticMeshComponent> FlapMeshComponent;
	// Current angle of the flap
	float FlapAngle;
	// Current input channel value
	float InputChannelValue;
	// Index of the flap element on the part root
	mutable int32 FluidDynamicElementIndex;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Thruster)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	float InputScale;
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	float MinAngle;
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	float MaxAngle;
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	bool bAccumulateInput;
	// ~Brick Properties

public:
	// ~Constructor
	UFlapBrick();

	// ~Super Interface
	virtual void RecycleBrickEditorObject() override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UpdateCustomDepth(bool bEnable, uint8 Stencil) override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void RepairBrick() override;
	virtual void UpdateBrickMaterial() override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual void GetFluidDynamicElements(FFluidDynamicElements& OutElements, const FTransform& Transform) const override;
	virtual void GetFluidDynamicSurface(FFluidDynamicSurface& OutElement, const FTransform& Transform) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EBrickEditorMirrorMode MirrorMode) const override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeUnique<FFlapBrickEditorParams>();
	}

	// ~Super Interface

private:
	// Sets the current angle of the flap
	void SetFlapAngle(float NewAngle);
	// Returns the current relative transform of the flap surface
	FTransform GetFlapRelativeTransform() const
	{
		auto FlapTransform = GetStaticInfo<UFlapBrickStaticInfo>()->FlapSocketTransform;
		FlapTransform.SetRotation(FlapTransform.GetRotation() * FRotator(FlapAngle, 0.f, 0.f).Quaternion());
		return FlapTransform;
	}
};
