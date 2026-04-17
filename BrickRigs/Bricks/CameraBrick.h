// Copyright Fluppisoft, 2016

#pragma once

#include "SeatBrick.h"
#include "Player/ViewTargetInterface.h"
#include "Bricks/Brick.h"
#include "CameraBrick.generated.h"

class UCameraBrickWidget;

UCLASS(Abstract)
class BRICKRIGS_API UCameraBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Local space view location offset
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FVector LenseOffset;
	// Zoom params to use for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetZoomParams ZoomParams;

	// Whether the view should be world relative rather than attached to the camera
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	bool bViewRotationInWorldSpace;
	// Whether the view should only be rotated while the player is interacting
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	bool bRotateViewWhenInteracting;
	// Post process settings to use, the blendable in slot 0 is being used for the disconnected effect
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FPostProcessSettings PostProcessSettings;
	// Post process material added when the camera is detached from the vehicle
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	UMaterialInterface* NotConnectedPostProcessMaterial;
	// The widget to display as the camera HUD
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSoftClassPtr<UCameraBrickWidget> HUDWidgetClass;
	// The maximum range that can be measured/displayed/hit
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float MaxTargetRange;

	// Min and max pitch view rotation
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval ViewPitchRange;
	// Min and max yaw view rotation
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval ViewYawRange;
	// ~Properties

	// ~Constructor
	UCameraBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UCameraBrick : public UBrick, public IViewTargetInterface
{
	GENERATED_BODY()

	// ~Variables
	// Cached camera zoom values
	mutable FViewTargetZoomCache ZoomCache;
	// Used to determine the initial view rotation
	uint32 bFirstTimeAsViewTarget : 1;
	// ~Variables

public:
	// ~Brick Properties
	// Exclusive seat this camera can be accessed from
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	FBrickEditorObjectPtr OwningSeat;
	// ~Brick Properties

	// ~Constructor
	UCameraBrick()
	{
		bFirstTimeAsViewTarget = true;
	}

	// ~Super Interface
	virtual void RepairBrick() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Used to get the desired view info for the camera
	virtual void GetCameraBrickView(float DeltaTime, FMinimalViewInfo& DesiredView);
	// Traces against world geometry from the current POV
	bool CameraTrace(FHitResult& OutHit);
	// Called from the vehicle on the active camera when the player started or stopped interacting
	void OnIsInteractingChanged(bool bNewInteracting);

private:
	// Returns the view rotation to use upon first use of the camera
	virtual FRotator GetFirstTimeViewRotation() const
	{
		return FRotator::ZeroRotator;
	}

public:
	// ~IViewTargetInterface
	virtual const FViewTargetZoomParams* GetZoomParams() const override
	{
		return &GetStaticInfo<UCameraBrickStaticInfo>()->ZoomParams;
	}

	virtual FViewTargetZoomCache* GetZoomCache() const override
	{
		return &ZoomCache;
	}

	virtual FRotator GetInitialViewRotation() const override;
	virtual void GetViewRotationLimit(FRotator& OutMinRotation, FRotator& OutMaxRotation) const override;
	virtual void BecomeViewTargetSubobject(APlayerController* PC) override;
	virtual void EndViewTargetSubobject(APlayerController* PC) override;
	virtual void UpdateViewRotation(FRotator& InOutRotation, float DeltaTime) override;
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	virtual FVector GetViewTargetAirVelocity() const override
	{
		return GetPhysicsLinearVelocity();
	}

	// ~IViewTargetInterface

	bool CanBeUsedBySeat(const USeatBrick* InSeat) const;

private:
	// Property callback
	static bool IsSeatObjectAllowed(const FBrickPropertyContainer& Container, const UBrickEditorObject* Object);
};
