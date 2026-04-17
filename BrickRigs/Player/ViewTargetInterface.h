#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ViewTargetInterface.generated.h"

class APlayerController;
class ABrickPlayerController;

USTRUCT()
struct FViewTargetSharedParams
{
	GENERATED_BODY()
	;

public:
	// Speed at which the camera is zoomed in through rate input (e.g. a key)
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float InputRate;
	// Step at which the camera is zoomed in (e.g. with the mouse wheel)
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float Step;
	// The exponent of the zoom in curve
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float Exponent;
	// Speed at which the camera zoom is interpolated
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float InterpSpeed;

	// ~Constructor
	FViewTargetSharedParams()
		: InputRate(1.f), Step(0.05f), Exponent(2.f), InterpSpeed(8.f)
	{
	}
};

// This struct can be used to store camera speed parameters on static info assets etc.
USTRUCT(BlueprintType)
struct FViewTargetCameraSpeedParams : public FViewTargetSharedParams
{
	GENERATED_BODY()
	;

public:
	// The min and max camera speed
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval SpeedRange;

	// ~Constructor
	FViewTargetCameraSpeedParams()
		: FViewTargetSharedParams(), SpeedRange(300.f, 600.f)
	{
		Exponent = 1.5f;
	}
};

// This struct can be used to store zoom parameters on static info assets etc.
USTRUCT(BlueprintType)
struct FViewTargetZoomParams : public FViewTargetSharedParams
{
	GENERATED_BODY()
	;

public:
	// The min and max zoom ratio
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FFloatInterval ZoomRange;

	// ~Constructor
	FViewTargetZoomParams()
		: FViewTargetSharedParams(), ZoomRange(0.1f, 1.f)
	{
		Exponent = 0.75f;
	}
};

struct FViewTargetZoomCache
{
	float InputRate;
	float TargetRatio;
	float CurrentRatio;

	// ~Constructor
	FViewTargetZoomCache(float InDefaultRatio = 0.f)
		: InputRate(0.f), TargetRatio(InDefaultRatio), CurrentRatio(InDefaultRatio)
	{
	}
};

using FViewTargetCameraSpeedCache = FViewTargetZoomCache;

UINTERFACE()
class UViewTargetInterface : public UInterface
{
	GENERATED_BODY()
};

class IViewTargetInterface
{
	GENERATED_IINTERFACE_BODY()

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FFloatMulticastDelegate, float);

	// ~Variables
	// The player controller using this view target
	TWeakObjectPtr<ABrickPlayerController> ViewingPlayerPrivate;
	// The current view (control) rotation
	FRotator ViewRotationPrivate;
	// ~Variables

public:
	// ~Delegates
	FFloatMulticastDelegate OnCameraSpeedRatioChangedDelegate;
	FFloatMulticastDelegate OnZoomRatioChangedDelegate;
	// ~Delegates

	// Can be implemented to return a subobject that is being used as the view target, for example a camera brick
	virtual UObject* GetViewTargetSubobject() const
	{
		return _getUObject();
	}

	// Whether the view target is being used
	bool IsViewTarget() const
	{
		return ViewingPlayerPrivate.IsValid();
	}

	// Get the player player using this view target
	ABrickPlayerController* GetViewingPlayer() const
	{
		return ViewingPlayerPrivate.Get();
	}

	// Whether spectators are allowed to change our view rotation
	virtual bool CanSpectatorControlView() const
	{
		return true;
	}

	// Called when the owner has become the view target subobject
	virtual void BecomeViewTargetSubobject(APlayerController* PC);

	// Called when the owner is no longer the view target subobject
	virtual void EndViewTargetSubobject(APlayerController* PC);

protected:
	// Sets the viewing player
	void SetViewingPlayer(ABrickPlayerController* PC);

private:
	// Called when the interface has become or ended being view target
	virtual void OnIsViewTargetChanged(bool bNewIsViewTarget)
	{
	}

public:
	// Sets the current view rotation of the pawn, limits the view rotation and possibly applies other modifications like recoil
	virtual void UpdateViewRotation(FRotator& InOutRotation, float DeltaTime);

	// Sets the current view rotation, without checking for limits etc
	void SetCachedViewRotation(const FRotator& NewRotation)
	{
		ViewRotationPrivate = NewRotation;
	}

protected:
	// Get the currently stored view rotation
	FRotator GetCachedViewRotation() const
	{
		return ViewRotationPrivate;
	}

public:
	// Returns the view rotation that should be applied when becoming the view target
	virtual FRotator GetInitialViewRotation() const
	{
		return GetCachedViewRotation();
	}

	// Can be implemented for custom view rotation limits
	virtual void GetViewRotationLimit(FRotator& OutMinRotation, FRotator& OutMaxRotation) const;

	// Utility used to limit a single axis
	bool LimitViewRotationAxis(float& Value, float Min, float Max) const
	{
		// Move the value to the -180...180 range so it can be clampled more easily
		Value = FRotator::NormalizeAxis(Value);
		if (Value < Min)
		{
			Value = Min;
			return true;
		}
		if (Value > Max)
		{
			Value = Max;
			return true;
		}
		return false;
	}

	// Applies the view rotation limits to the given rotation
	void LimitViewRotation(FRotator& InOutRotation) const;

	// Implement to return the exact camera transform
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const = 0;

	// Get the list of actors to ignore when doing depth of field traces
	virtual void GetFocalDistanceIgnoredActors(TArray<const AActor*>& OutActorsToIgnore) const
	{
	}

	// Hook to override the focal distance before the rest of the dof setup
	virtual bool OverrideFocalDistance(float& InOutFocalDistance) { return false; }

	// Returns the transform that the free cam should be attached to
	virtual FTransform GetFreeCamFollowTransform() const;

	// ~Speed
	// Implement to return the velocity relative to air if air noise is desired
	virtual FVector GetViewTargetAirVelocity() const
	{
		return FVector::ZeroVector;
	}

	// Implement to return custom camera speed parameters
	virtual const FViewTargetCameraSpeedParams* GetCameraSpeedParams() const
	{
		return nullptr;
	}

	// Implement to return custom camera speed cached values
	virtual FViewTargetCameraSpeedCache* GetCameraSpeedCache() const
	{
		return nullptr;
	}

	// Returns the current speed ratio to use
	virtual float GeCameraSpeedRatio() const
	{
		const auto* SpeedCache = GetCameraSpeedCache();
		return SpeedCache ? SpeedCache->CurrentRatio : 0.f;
	}

	// Returns the current maximum camera speed
	float GetMaxCameraSpeed() const;

	// Increases or decreases the speed by the given amount of positive or negative steps
	void SpeedUpStep(int32 Steps);

	// Adds acceleration input
	void SpeedUpRate(float Val);

protected:
	// To be called from CalcCamera, updates the camera speed
	void UpdateCameraSpeed(float DeltaTime);

	// Sets the camera speed ratio target
	void SetTargetCameraSpeedRatio(float NewTarget, bool bImmediate = false);

	// Sets the current camera speed ratio
	void SetCameraSpeedRatio(float NewRatio, bool bNotify = true);

	// Called whenever the speed ratio has changed
	virtual void OnCameraSpeedRatioChanged()
	{
	}

	// ~Speed

public:
	// ~Zoom
	// Whether this view target supports zooming
	bool HasZoomFeature() const
	{
		return GetZoomParams() != nullptr;
	}

	// Implement to get the zoom parameters
	virtual const FViewTargetZoomParams* GetZoomParams() const
	{
		return nullptr;
	}

	// Implement to return the runtime zoom parameters
	virtual FViewTargetZoomCache* GetZoomCache() const
	{
		return nullptr;
	}

	// Returns the current zoom value
	float GetZoom() const;

	// Version that takes a custom range
	float GetZoom(const FFloatInterval& InZoomRange) const;

	// Zooms the view in a certain amount of steps
	void ZoomInStep(int32 Steps);

	// Adds zoom rate input
	void ZoomInRate(float Val);

protected:
	// To be called from CalcCamera, updates the zoom ratio
	void UpdateZoom(float DeltaTime);

	// Sets the zoom ratio target
	void SetTargetZoomRatio(float NewTarget, bool bImmediate = false);

	// Sets the current zoom ratio
	void SetZoomRatio(float NewRatio, bool bNotify = true);

	// Called whenever the zoom ratio has changed
	virtual void OnZoomRatioChanged()
	{
	}

	// ~Zoom
};
