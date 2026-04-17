// Copyright Fluppisoft, 2015

#pragma once

#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Bricks/Brick.h"
#include "WheelBrick.generated.h"

class USprocketWheelBrick;

UCLASS(Abstract)
class BRICKRIGS_API UWheelBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float WheelRadius = 30.f;
	// The min and max X extent of the wheel mesh, automatically generated
	UPROPERTY(VisibleDefaultsOnly, Category = Wheel)
	FFloatInterval WheelWidthRange;
	// Radius of the sphere collider, automatically generated
	UPROPERTY(VisibleDefaultsOnly, Category = Wheel)
	float ColliderRadius;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float MinWheelRadius = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float MaxWheelRadiusScale = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float MinWheelWidth = 10.f;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float MaxWheelWidthScale = 4.f;
#if WITH_EDITORONLY_DATA
	// Optional proxy mesh used to calculate mass properties
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	UStaticMesh* MassPropertiesProxyMesh;
#endif
	// ~Properties

	// ~Constructor
	UWheelBrickStaticInfo();

	// ~Super Interface
#if WITH_EDITOR
	virtual void BuildCachedData() override;
	virtual void CalcMassProperties() override;
	virtual UStaticMesh* GetMassPropertiesProxyMesh(FTransform& OutTransform) const override;
#endif
	// ~Super Interface
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UWheelBrick : public UBrick
{
	GENERATED_BODY()

protected:
	enum EWheelPrimitiveData
	{
		WidthOffset = Max,
		RadiusOffset = WidthOffset + 1,
		RimRadiusOffset = RadiusOffset + 1,
		Max = RimRadiusOffset + 1
	};

	// ~Variables
	// All currently active wheel connections
	UPROPERTY(Transient)
	TArray<UWheelConnection*> WheelConnections;
	// ~Variables

	// ~Brick Props
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	bool bInvertTankSteering;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	FBrickUnits WheelDiameter;
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	FBrickUnits WheelWidth;
	// ~Brick Props

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void SetupCreateStaticMeshComponentParams(FBrickStaticMeshComponentParams& Params) override;
	virtual FTransform GetConnectorRelativeTransform(const FConnectorField& ConnectorField) const override;
	virtual float GetMaxConnectorDist() const override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* InConnection) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EBrickEditorMirrorMode MirrorMode) const override;
	virtual void OnCalculateMassProperties(FBodyInstance* BodyInstance, PxMassProperties& OutMassProps, FTransform& OutMassTransform) override;
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const override;
	virtual FVector GetRenderScale3D() const override;
	virtual FVector GetBodySetupScale3D() const override;
	// ~Super Interface

	// Whether the tank steering direction is set to be inverted on this wheel
	bool IsTankSteeringInverted() const;
	// Whether the wheel has an active wheel connection
	bool HasWheelConnection() const
	{
		return WheelConnections.Num() > 0;
	}

	// Returns all active wheel connections
	const auto& GetWheelConnections() const
	{
		return WheelConnections;
	}

	// Get the positive and negative extent of the wheel collider
	FFloatInterval GetWheelColliderWidthRange() const;
	// Get the total width of the wheel
	float GetWheelWidth() const;
	// Returns the inflated radius of the wheel
	float GetWheelRadius() const;
	// Get the radius of the drive surface, for deflated bricks it would be the rim radius for example
	virtual float GetDriveRadius() const;
	// Can be implemented to return the connected sprocket wheel
	virtual USprocketWheelBrick* GetSprocketWheel() const
	{
		return nullptr;
	}

	// Returns true if the given hit was on the wheel drive surface
	bool IsDriveSurfaceHit(const FVector& HitNormal, float& OutDamageRatio) const;

protected:
	// Returns the desired radius of the wheel collider, can be overwritten for deflating etc.
	virtual float GetBodySetupTargetRadius() const;
	// Returns the X scale to apply to the wheel
	float GetWheelWidthScale() const;
	// Returns the distance the wheel width is extended or shrunk
	float GetWheelWidthOffset() const;
	// Returns the distance the wheel radius is extended or shrunk
	float GetWheelRadiusOffset() const;
	// Can be implemented for wheels with tires
	virtual float GetRimRadiusOffset() const;
};
