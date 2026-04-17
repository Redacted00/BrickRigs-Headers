// Copyright Fluppisoft, 2018

#pragma once

#include "ScalableBrickBase.h"
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
	ConeFlat,
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
	Cylinder90R1,
	Flag,
	TankCylinder,
	Button,
	Flap
};

USTRUCT(BlueprintType)
struct FScalableBrickReplacementMesh
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere)
	FVector MinSize = FVector::ZeroVector;
};

USTRUCT()
struct FScalableBrickEditorParams : public FScalableBrickBaseEditorParams
{
	GENERATED_BODY()

	// Scale axis currently focused
	EAxis::Type FocusedScaleAxis = EAxis::None;
};

UCLASS(Abstract)
class BRICKRIGS_API UScalableBrickStaticInfo : public UScalableBrickBaseStaticInfo
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
class BRICKRIGS_API UScalableBrick : public UScalableBrickBase
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The lift surface direction
	uint8 LiftAxisIdx;
	// ~Variables

	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = ScalableBrick)
	FVector BrickSize;
	// ~Brick Properties

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void OnMirrorBrickEditorObject(EAxis::Type MirrorAxis) override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EAxis::Type MirrorAxis) const override;
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual bool ShouldShowGenerateLiftProperty() const override;
	virtual FVector GetBrickEditorObjectSize() const override;
	virtual void GetFluidDynamicSurface(FFluidDynamicSurface& OutElement, const FTransform& Transform) const override;
	virtual UStaticMesh* GetStaticMesh() const override;
	virtual uint8 GetFocusedConnectorAxisFlags() const override;
	virtual FVector GetRenderScale3D() const override;
	virtual FVector GetBodySetupScale3D() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual bool ResolveRemovedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeEditorParams<FScalableBrickEditorParams>();
	}

	virtual void OnGenerateConnectors() override;
	// ~Super Interface

	// Get the size factor to use
	FVector GetScalableBrickScale3D() const;

protected:
	// Callbacks for the size property
	virtual FVector GetMinBrickSize() const;
	virtual FVector GetMaxBrickSize() const;
	virtual EFluAxisLock GetBrickSizeAxisLock() const;
	// Called after the user changes the size to enforce limits
	virtual void ConstraintBrickSize();
	// Updates the lift surface properties according to the current scale
	void UpdateAerodynamicLiftSurface();
	// Returns the size to use when mirrored
	FVector GetMirroredBrickSize() const;
	// Returns the connector spacing to use when mirrored
	FScalableBrickConnectorSpacing GetMirroredConnectorSpacing() const;
};
