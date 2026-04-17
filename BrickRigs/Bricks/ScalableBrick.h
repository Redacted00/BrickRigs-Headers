// Copyright Fluppisoft, 2018

#pragma once

#include "Misc/ScalableBrickConnectorSpacing.h"
#include "Bricks/Brick.h"
#include "ScalableBrick.generated.h"

UENUM(BlueprintType)
enum class EScalableBrickShape : uint8
{
	Box,
	Ramp,
	Cylinder,
	HalfCylinder,
	Cone,
	QuarterCone,
	HalfCone,
	Hemisphere,
	QuarterSphere,
	RampRounded,
	RampRoundedN,
	Corner,
	CornerN,
	CornerRounded,
	CornerRoundedN,
	Pyramid,
	PyramidCorner,
	PyramidCornerRounded,
	Wedge,
	WedgeCorner,
	Cylinder90R0,
	Cylinder90R1
};

USTRUCT(BlueprintType)
struct FScalableBrickReplacementMesh
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	FBrickSize MinSize;
};

struct FScalableBrickEditorParams : FBrickEditorParams
{
	// The scale axis currently focused
	EAxis::Type FocusedScaleAxis = EAxis::None;
	// The connector direction property that is currently focused
	EConnectorDirection FocusedConnectorDirection = EConnectorDirection::Max;
};

UCLASS(Abstract)
class BRICKRIGS_API UScalableBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Variables
	// The surface areas for the Y and Z axis, X axis is stored in the base class
	UPROPERTY()
	float ScalableLiftSurfaceRadiiYZ[2];
	// Used to store surface normals additionally to the base class
	UPROPERTY()
	FVector ScalableLiftSurfaceNormalsYZ[2];
	// ~Variables

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	EScalableBrickShape ScalableShape;
	// Allows changing the mesh as the brick gets bigger
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	TArray<FScalableBrickReplacementMesh> ReplacementMeshes;
	// ~Properties

	// ~Constructor
	UScalableBrickStaticInfo();

	// ~Super Interface
	virtual void GetBrickEditorFilterTags(FGameplayTagContainer& OutTags) const override;
#if WITH_EDITOR
	virtual void ProcessFluidDynamicProperties(const FVector& BoundsSize, const FVector& SurfaceAreas, const FVector& AverageLocation, const FVector* AverageNormals) override;
#endif
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UScalableBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// Generated array of connectors
	TArray<FConnectorField> ScalableConnectors;
	// Cached maximum distance of any connector
	float MaxScalableConnectorDist;
	// The lift surface direction
	uint8 LiftAxisIdx;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	FBrickSize BrickSize;
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	FScalableBrickConnectorSpacing ConnectorSpacing;
	// ~Brick Properties

public:
	// ~Constructor
	UScalableBrick();

	// ~Super Interface
	virtual void OnMirrorBrickEditorObject(EBrickEditorMirrorMode MirrorMode) override;
	virtual bool AreBrickPropertiesMirroredFrom(UBrickEditorObject* OtherObject, EBrickEditorMirrorMode MirrorMode) const override;
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void SetupCreateStaticMeshComponentParams(FBrickStaticMeshComponentParams& Params) override;
	virtual bool ShouldShowGenerateLiftProperty() const override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual void OnCalculateMassProperties(FBodyInstance* BodyInstance, PxMassProperties& OutMassProps, FTransform& OutMassTransform) override;
	virtual FBrickSize GetBrickEditorObjectSize() const override;
	virtual float GetBrickEditorVolumeScale() const override;
	virtual void GetFluidDynamicSurface(FFluidDynamicSurface& OutElement, const FTransform& Transform) const override;
	virtual const TArray<FConnectorField>& GetBrickConnectors() const override;
	virtual float GetMaxConnectorDist() const override;
	virtual TTuple<EAxis::Type, EConnectorDirection> GetFocusedConnectorAxis() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeUnique<FScalableBrickEditorParams>();
	}

	// ~Super Interface

	// Get the size factor to use
	FVector GetScalableBrickScale3D() const
	{
		return BrickSize.ToVector();
	}

private:
	// Returns the mesh that should be used
	UStaticMesh* GetDesiredStaticMesh() const;
	// Updates the connectors according to the current shape
	void GenerateConnectors();
	// Updates the lift surface properties according to the current scale
	void UpdateAerodynamicLiftSurface();
	// Returns the size to use when mirrored
	FBrickSize GetMirroredBrickSize(EBrickEditorMirrorMode MirrorMode) const
	{
		int32 MirrorAxisX, MirrorAxisY, MirrorAxisZ;
		GetMirrorAxes(MirrorAxisX, MirrorAxisY, MirrorAxisZ);

		// Flip and exchange the size axes according to the mirror mode
		auto GetSizeAxis = [&](int32 InAxis)
		{
			return BrickSize[FMath::Abs(InAxis) - 1];
		};
		return FBrickSize(GetSizeAxis(MirrorAxisX), GetSizeAxis(MirrorAxisY), GetSizeAxis(MirrorAxisZ));
	}

	// Mirrors the brick size property
	void MirrorBrickSize(EBrickEditorMirrorMode MirrorMode);
	// Mirrors the connector spacing property
	void MirrorConnectorSpacing(EBrickEditorMirrorMode MirrorMode);
};
