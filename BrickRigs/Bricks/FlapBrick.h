// Copyright Fluppisoft, 2017

#pragma once

#include "ScalableBrick.h"
#include "FlapBrick.generated.h"

USTRUCT()
struct FFlapBrickEditorParams : public FScalableBrickEditorParams
{
	GENERATED_BODY()

	bool bFocusedMinAngle;
	bool bFocusedMaxAngle;
};

UCLASS(Abstract)
class BRICKRIGS_API UFlapBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Rate at which the flap is moved
	UPROPERTY(EditDefaultsOnly, Category = Flap)
	float FlapInterpSpeed;
	// ~Properties

	// ~Constructor
	UFlapBrickStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
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
class BRICKRIGS_API UFlapBrick : public UScalableBrick
{
	GENERATED_BODY()

	enum class EFlapPrimitiveData : uint8
	{
		FlapAngle = Max,
		Max = FlapAngle + 1
	};

	// ~Variables
	// Current angle of the flap
	float FlapAngle = 0.f;
	// Current input channel value
	float InputChannelValue = 0.f;
	// Index of the flap element on the part root
	mutable int32 FluidDynamicElementIndex = INDEX_NONE;
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
	bool bAccumulateInput = false;
	// ~Brick Properties

public:
	// ~Constructor
	UFlapBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void RecycleBrickEditorObject() override;
	virtual void PostConstructVehicle() override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void RepairBrick() override;
	virtual void GetFluidDynamicElements(FFluidDynamicElements& OutElements, const FTransform& Transform) const override;
	virtual void GetFluidDynamicSurface(FFluidDynamicSurface& OutElement, const FTransform& Transform) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EAxis::Type MirrorAxis) const override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeEditorParams<FFlapBrickEditorParams>();
	}

	virtual FVector GetMaxBrickSize() const override;
	virtual FBoxSphereBounds CalcStaticMeshBounds() const override;
	// ~Super Interface

private:
	// Sets the current angle of the flap
	void SetFlapAngle(float NewAngle);

	// Converts the flap angle for the material parameter
	auto GetFlapAngleParam() const
	{
		return -FlapAngle / 360.f;
	}
};
