// Copyright Fluppisoft, 2018

#pragma once

#include "Vehicle/VehicleInputAxis.h"
#include "PlayerPawnInputComponent.h"
#include "VehicleInputComponent.generated.h"

class USeatBrick;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UVehicleInputComponent : public UPlayerPawnInputComponent
{
	GENERATED_BODY()

protected:
	// ~Variables
	FHoldKeyTimer HoldKeyTimer_CycleSeats;
	FHoldKeyTimer HoldKeyTimer_CycleCamera;
	FHoldKeyTimer HoldKeyTimer_HandBrake;
	FHoldKeyTimer HoldKeyTimer_OperationMode;
	FHoldKeyTimer HoldKeyTimer_ToggleSteering;
	FHoldKeyTimer HoldKeyTimer_CycleSiren;
	FHoldKeyTimer HoldKeyTimer_PinVehicle;
	FHoldKeyTimer HoldKeyTimer_CycleFireActionMode;
	// The seat the player is sitting in
	UPROPERTY(Transient)
	USeatBrick* VehicleSeat;
	// Whether the camera is being pivoted right now
	bool bPivotCamera;
	// Stores input axes values accumulated over one frame
	TMap<EVehicleInputAxis, float> InputAxesInput;
	// ~Variables

public:
	// ~Constructor
	UVehicleInputComponent();

	// ~Super Interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetOwningCharacter(ABaseCharacter* NewCharacter) override;
	virtual bool CanAddViewInput() const override;
	virtual void MouseMoveUp(float Val) override;
	virtual void MouseMoveRight(float Val) override;
	virtual void LookUp(float Val) override;
	virtual void LookRight(float Val) override;
	virtual bool OnPivotCamera(bool bPressed) override;
	// ~Super Interface

protected:
	// Sets the associated seat
	virtual void SetVehicleSeat(USeatBrick* NewSeat);

	// Sets an action to on or off
	void SetAction(EVehicleInputAxis Action, bool bEnable);
	// Toggles an action between on and off
	void ToggleAction(EVehicleInputAxis Action);

	// Adds input to an input axis
	void AddInputAxisInput(EVehicleInputAxis Axis, float Val, bool bAdd = true);
	// Get the currently accumulated input
	float GetInputAxisInput(EVehicleInputAxis Axis) const
	{
		auto FoundVal = InputAxesInput.Find(Axis);
		return FoundVal ? *FoundVal : 0.f;
	}

	// Return whether operation mode is currently enabled
	bool IsInOperationMode() const;
	// Whether mouse and gamepad input should be used for the view pitch and yaw axis currently
	bool UseViewInputForPitchAndYaw() const;
	// Get the factor that should be applied to mouse input
	float GetMouseAxisInputScale() const;
	// Callback for the vehicle
	void OnVehicleInputActionChanged(EVehicleInputAxis Action, bool bNewValue);

	// Vehicle
	UFUNCTION()
	void OnPressedCaptureVehicleThumbnail();
	UFUNCTION()
	void OnPressedCycleSeats();
	UFUNCTION()
	void OnReleasedCycleSeats();
	UFUNCTION()
	void OnTappedCycleSeats();
	UFUNCTION()
	void OnHeldCycleSeats();
	UFUNCTION()
	void OnPressedExitVehicle();

	// Pinning
	UFUNCTION()
	void OnPressedPinVehicle()
	{
		HoldKeyTimer_PinVehicle.OnPressed(this, &ThisClass::OnHeldPinVehicle);
	}

	UFUNCTION()
	void OnReleasedPinVehicle()
	{
		if (HoldKeyTimer_PinVehicle.OnReleased())
		{
			OnTappedPinVehicle();
		}
	}

	UFUNCTION()
	void OnTappedPinVehicle();
	UFUNCTION()
	void OnHeldPinVehicle();

	// Camera
	UFUNCTION()
	void OnPressedCycleCameraMode();
	UFUNCTION()
	FText GetCycleCameraModeValueText() const;
	UFUNCTION()
	void OnPressedCycleCamera();
	UFUNCTION()
	void OnReleasedCycleCamera();
	UFUNCTION()
	void OnTappedCycleCamera();
	UFUNCTION()
	void OnHeldCycleCamera();

	// Shifting
	UFUNCTION()
	void OnPressedShiftUp();
	UFUNCTION()
	void OnPressedShiftDown();

	// Actions
	UFUNCTION()
	void OnPressedOperationMode();
	UFUNCTION()
	void OnReleasedOperationMode();
	UFUNCTION()
	void OnToggleOperationMode();
	UFUNCTION()
	void OnPressedToggleSteering();
	UFUNCTION()
	void OnReleasedToggleSteering();
	UFUNCTION()
	void OnTappedToggleSteering();
	UFUNCTION()
	void OnHeldToggleSteering();
	UFUNCTION()
	void OnPressedHorn();
	UFUNCTION()
	void OnReleasedHorn();
	UFUNCTION()
	void OnPressedHandBrake();
	UFUNCTION()
	void OnReleasedHandBrake();
	UFUNCTION()
	void OnPressedHeadlight();
	UFUNCTION()
	void OnPressedBeacon();
	UFUNCTION()
	void OnPressedWarningLight();

	// Vehicle axes
	UFUNCTION()
	void Steering(float Val);
	UFUNCTION()
	void Throttle(float Val);
	UFUNCTION()
	void Brake(float Val);
	UFUNCTION()
	void ViewPitch(float Val);
	UFUNCTION()
	void ViewYaw(float Val);
	UFUNCTION()
	void Pitch(float Val);

	// Sirens
	UFUNCTION()
	void OnPressedCycleSiren();
	UFUNCTION()
	void OnReleasedCycleSiren();
	UFUNCTION()
	void OnTappedCycleSiren();
	UFUNCTION()
	void OnHeldCycleSiren();

	// Fire Actions
	UFUNCTION()
	void OnPressedCycleFireActionMode()
	{
		HoldKeyTimer_CycleFireActionMode.OnPressed(this, &ThisClass::OnHeldCycleFireActionMode);
	}

	UFUNCTION()
	void OnReleasedCycleFireActionMode()
	{
		if (HoldKeyTimer_CycleFireActionMode.OnReleased())
		{
			OnTappedCycleFireActionMode();
		}
	}

	UFUNCTION()
	void OnTappedCycleFireActionMode()
	{
		CycleFireActionMode(1);
	}

	UFUNCTION()
	void OnHeldCycleFireActionMode()
	{
		CycleFireActionMode(-1);
	}

	// Cycles to the next or previous fire action mode
	void CycleFireActionMode(int32 Dir);

	UFUNCTION()
	void OnPressedAction1()
	{
		SetAction(EVehicleInputAxis::Action1, true);
	}

	UFUNCTION()
	void OnReleasedAction1()
	{
		SetAction(EVehicleInputAxis::Action1, false);
	}

	UFUNCTION()
	void OnPressedAction2()
	{
		SetAction(EVehicleInputAxis::Action2, true);
	}

	UFUNCTION()
	void OnReleasedAction2()
	{
		SetAction(EVehicleInputAxis::Action2, false);
	}

	UFUNCTION()
	void OnPressedAction3()
	{
		SetAction(EVehicleInputAxis::Action3, true);
	}

	UFUNCTION()
	void OnReleasedAction3()
	{
		SetAction(EVehicleInputAxis::Action3, false);
	}

	UFUNCTION()
	void OnPressedAction4()
	{
		SetAction(EVehicleInputAxis::Action4, true);
	}

	UFUNCTION()
	void OnReleasedAction4()
	{
		SetAction(EVehicleInputAxis::Action4, false);
	}

	UFUNCTION()
	void OnPressedAction5()
	{
		SetAction(EVehicleInputAxis::Action5, true);
	}

	UFUNCTION()
	void OnReleasedAction5()
	{
		SetAction(EVehicleInputAxis::Action5, false);
	}

	UFUNCTION()
	void OnPressedAction6()
	{
		SetAction(EVehicleInputAxis::Action6, true);
	}

	UFUNCTION()
	void OnReleasedAction6()
	{
		SetAction(EVehicleInputAxis::Action6, false);
	}

	UFUNCTION()
	void OnPressedAction7()
	{
		SetAction(EVehicleInputAxis::Action7, true);
	}

	UFUNCTION()
	void OnReleasedAction7()
	{
		SetAction(EVehicleInputAxis::Action7, false);
	}

	UFUNCTION()
	void OnPressedAction8()
	{
		SetAction(EVehicleInputAxis::Action8, true);
	}

	UFUNCTION()
	void OnReleasedAction8()
	{
		SetAction(EVehicleInputAxis::Action8, false);
	}
};
