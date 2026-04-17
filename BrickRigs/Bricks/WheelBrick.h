// Copyright Fluppisoft, 2015

#pragma once

#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Bricks/Brick.h"
#include "WheelBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UWheelBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float WheelRadius;
	// The min and max X extent of the wheel mesh, automatically generated
	UPROPERTY(VisibleDefaultsOnly, Category = Wheel)
	FFloatInterval WheelColliderWidthRange;
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

private:
	// ~Variables
	// Number of wheel connections currently active
	uint16 NumActiveWheelConnections;
	// ~Variables

protected:
	// ~Brick Props
	UPROPERTY(EditDefaultsOnly, Category = Steering)
	bool bInvertTankSteering;
	// ~Brick Props

public:
	// ~Super Interface
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* InConnection) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const override;
	// ~Super Interface

	// Whether the tank steering direction is set to be inverted on this wheel
	bool IsTankSteeringInverted() const;
	// Whether the wheel has an active wheel connection
	bool HasWheelConnection() const
	{
		return NumActiveWheelConnections > 0;
	}

	// Get the positive and negative extent of the wheel collider
	const FFloatInterval& GetWheelColliderWidthRange() const
	{
		return GetStaticInfo<UWheelBrickStaticInfo>()->WheelColliderWidthRange;
	}

	// Get the radius of the drive surface, for deflated bricks it would be the rim radius for example
	virtual float GetDriveRadius() const;
	// Returns true if the given hit was on the wheel drive surface
	bool IsDriveSurfaceHit(const FVector& HitNormal, float& OutDamageRatio) const;
};
