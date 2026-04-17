// Copyright Fluppisoft, 2015

#pragma once

#include "BrickEditor/Components/BrickEditorSkeletalMeshComponent.h"
#include "Misc/ActuatorTypes.h"
#include "Character/CharacterHealth.h"
#include "Vehicle/VehicleInput.h"
#include "Vehicle/VehicleInputChannel.h"
#include "Bricks/Brick.h"
#include "SeatBrick.generated.h"

#define HANDBRAKE_DEFAULT_VALUE true

class UCameraBrick;
class UGunBrick;
class UActuatorBrick;
class UAnimSequence;
class ABrickCharacter;

USTRUCT()
struct FSeatBrickEditorParams : public FBrickEditorParams
{
	GENERATED_BODY()

	TBrickEditorComponentPtr<UBrickEditorSkeletalMeshComponent> CharacterMeshComponent;
	bool bIsExitLocationFocused = false;
};

UCLASS(Abstract)
class BRICKRIGS_API USeatBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Min and max view pitch rotation in the seat
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FFloatInterval ViewPitchRange;
	// View pitch range when holding an item
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FFloatInterval ViewPitchRangeItem;
	// Min and max view yaw rotation
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FFloatInterval ViewYawRange;
	// View yaw range when holding an item
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FFloatInterval ViewYawRangeItem;
	// Determines how much of the damage received by the seat is propagated to the character
	UPROPERTY(EditDefaultsOnly, Category = Seat, Meta = (UIMin = 0.f))
	float CharacterDamageScale;
	// Minimum damage that will be sent to the character
	UPROPERTY(EditDefaultsOnly, Category = Seat, Meta = (UIMin = 0.f))
	float MinCharacterDamage;
	// The material to apply to the editor character mesh
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	UMaterialInterface* EditorCharacterMaterial;
	// The animation played for the character sitting in this seat
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	UAnimSequence* CharacterIdleSequence;
	// Capsule height for a character in this seat
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	float CharacterCapsuleHalfHeight;
	// The cached relative location of the camera pivot socket in the sit animation
	UPROPERTY(VisibleDefaultsOnly, Category = Seat)
	FVector CharacterIdleCameraSocketLocation;
	// Number of slots added to the inventory with every seat
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	int32 NumInventorySlots;
	// ~Properties

	// ~Constructor
	USeatBrickStaticInfo();

	// ~Super Interface
	// Has to be capped because seat index only goes to 255
	virtual int32 GetMaxInstances(UClass*& OutClass) const override
	{
		OutClass = StaticClass();
		return 200;
	};
#if WITH_EDITOR
	virtual void BuildCachedData() override;
	// ~Super Interface

private:
	// Updates the cached camera socket location
	void UpdateCharacterIdleSequence();
#endif
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API USeatBrick : public UBrick
{
	GENERATED_BODY()

	inline const static auto InvalidExitLocation = FVector(MAX_FLT);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnFireActionModeChanged, int32);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInputActionChanged, EVehicleInputAxis, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCurrentGearChanged, int32, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnActiveSirenIndexChanged, int32);

	enum class ESeatCameraMode : uint8
	{
		ThirdPerson,
		FirstPerson,
		CameraBrick
	};

	// ~Variables
	FTimerHandle TimerHandle_RevertReplicatedInput;
	// The legacy input channel property
	uint8 LegacySeatChannel;

	// The character currently in the seat
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Character)
	ABrickCharacter* Character;
	UFUNCTION()
	void OnRep_Character(ABrickCharacter* OldCharacter);

	// Current input state of the seat
	FVehicleInput VehicleInput;
	// Vehicle input as replicated from the server/from the client
	UPROPERTY(Transient, ReplicatedUsing = OnRep_VehicleInput)
	FVehicleInput RepVehicleInput;
	UFUNCTION()
	void OnRep_VehicleInput();
	// Per axis input channel delegates
	TMap<EVehicleInputAxis, FInputChannelValueChanged> InputChannelDelegates;

	// List of actuators that need to be updated on the server
	TArray<TWeakObjectPtr<UActuatorBrick>> ActuatorsToReplicate;

	// The currently selected action to fire
	uint8 FireActionMode;
	// Number of different sirens that can be cycled through
	uint8 NumSirenTypes;
	// Whether there is currently a conscious character in the seat
	uint8 bHasConsciousCharacter : 1;
	// Whether the character is currently firing
	uint8 bIsFiring : 1;
	// True in the PreRepairBrick function
	uint8 bIsRepairingSeat : 1;
	// Whether this seat has replaced a passenger seat
	uint8 bReplacedPassengerSeat : 1;
	// Camera currently being used
	ESeatCameraMode CameraMode;
	// ID of the current or last camera brick
	FBrickEditorObjectID CameraBrickID;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FString SeatName;
	// Relative exit location
	UPROPERTY(EditDefaultsOnly, Category = Seat)
	FVector ExitLocation;
	// ~Brick properties

public:
	// ~Delegates
	FOnFireActionModeChanged OnFireActionModeChangedDelegate;
	FOnInputActionChanged OnInputActionChangedDelegate;
	FOnCurrentGearChanged OnCurrentGearChangedDelegate;
	FOnActiveSirenIndexChanged OnActiveSirenIndexChangedDelegate;
	FSimpleMulticastDelegate OnViewTargetChangedDelegate;
	// ~Delegates

	// ~Constructor
	USeatBrick();

	// ~Super Interface
	virtual void RecycleBrickEditorObject() override;
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void PostConstructVehicle() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void SetupVehicleInventory(FInventoryProperties& OutProperties, FInventoryLoadout& OutLoadout) override;

	virtual bool ShouldReplicate() const override
	{
		return true;
	}

	virtual void PreRepairBrick(const FTransform& SpawnTransform) override;
	virtual void RepairBrick() override;
	virtual void ReceiveDamageInternal(int32 DamageDepth, float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const override;
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) override;
	virtual void OnIsOnFireChanged() override;
	virtual bool CanBePickedUp() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual bool ResolveRemovedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;
	virtual void UpdateEditorVisualization() override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeEditorParams<FSeatBrickEditorParams>();
	}

	// ~Super Interface

	// ~Seat
	// Returns the friendly display name of the seat
	FText GetSeatDisplayName() const;

	// Get the legacy seat channel index
	uint8 GetLegacySeatChannel() const
	{
		return LegacySeatChannel;
	}

	// Whether this seat has replaced the deprecated passenger seat brick
	bool HasReplacedPassengerSeat() const
	{
		return bReplacedPassengerSeat;
	}

	// Whether this seat is visible
	auto IsVisibleSeat() const
	{
		return GetRootComponent() != nullptr;
	}

	// Whether the custom exit location is enabled
	auto HasCustomExitLocation() const
	{
		return ExitLocation != InvalidExitLocation;
	}

	// Enables or disables the custom exit location
	void SetHasCustomExitLocation(bool bValue)
	{
		if (bValue != HasCustomExitLocation())
		{
			ExitLocation = bValue ? FVector::ZeroVector : InvalidExitLocation;
		}
	}

	// Returns the custom exit location, only set if it should be used
	auto GetCustomExitLocation() const
	{
		return HasCustomExitLocation() ? TOptional(GetBrickEditorRootSpawnTransform().TransformPositionNoScale(SpawnLocation + ExitLocation)) : TOptional<FVector>();
	}

	// Whether this seat is the driver seat instance
	bool IsDriverSeat() const;

	// Return whether the seat is currently occupied
	bool IsSeatOccupied() const
	{
		return Character != nullptr;
	}

	// Sets the character in this seat
	void SetCharacter(ABrickCharacter* NewCharacter);

	// Get the character in this seat
	ABrickCharacter* GetCharacter() const
	{
		return Character;
	}

	// Whether there is a character in the seat and it is conscious
	bool HasConsciousCharacter() const
	{
		return bHasConsciousCharacter;
	}

	// Whether a possessed character is in the seat
	bool HasPossessedCharacter() const;

	// Whether there is a locally controlled character in the seat
	bool HasLocallyControlledCharacter() const;

private:
	// Called whenever the character has been changed
	void OnCharacterChanged(ABrickCharacter* OldCharacter);

	// Updates the conscious character flag
	void UpdateHasConsciousCharacter();

	// Callback for the character
	void OnCharacterStateOfHealthChanged(ECharacterStateOfHealth NewStateOfHealth);
	// ~Seat

public:
	// ~Input
	// Get the replicated input struct
	const FVehicleInput& GetVehicleInput() const
	{
		return VehicleInput;
	}

	// Bind the delegate to the axis
	void AddInputChannelDelegate(const EVehicleInputAxis Axis, const FInputChannelDelegate& Delegate);

	// Returns the current value of an input channel
	float GetInputChannelValue(const EVehicleInputAxis Axis) const;

	// Get the current value of an input axis
	float GetInputAxis(EVehicleInputAxis Axis) const
	{
		return VehicleInput.GetAxis(Axis);
	}

	// Update a float input axis from player input
	void SetInputAxis(EVehicleInputAxis Axis, float Val);

	// Get the current value of an input action
	bool GetInputAction(EVehicleInputAxis Action) const
	{
		return VehicleInput.GetAction(Action);
	}

	// Update a bool input value from player input
	void SetInputAction(EVehicleInputAxis Action, bool bEnable);

	// Toggle an input action between on and off
	void ToggleInputAction(EVehicleInputAxis Action);

	// Called from the character RPC function
	void ServerUpdateVehicleInput(const FVehicleInput& NewInput, const TArray<FRepActuatorState>& NewActuatorStates, const FRestartTransformVersion& RestartVersion);

private:
	// Version that doesn't check for the driver consciousness
	void SetInputAxisInternal(EVehicleInputAxis Axis, float Val);

	// Version that doesn't check for the driver consciousness
	void SetInputActionInternal(EVehicleInputAxis Action, bool bEnable);

	// This is the version that should be called by OnVehicleInputAxisChanged and OnVehicleInputActionChanged
	void OnInputAxisOrActionChanged(EVehicleInputAxis Axis);

	// Called from player input and after replication when an axis changed
	void OnInputAxisChanged(EVehicleInputAxis Axis);

	// Called from player input and after replication when an action changed
	void OnInputActionChanged(EVehicleInputAxis Action);

	// Has to be called whenever the vehicle input struct has been altered
	void OnVehicleInputChanged();

	// Updates the value of a certain axis or  action
	void UpdateInputChannelValue(EVehicleInputAxis Axis);

	// Starts the lopping timer for reverting input
	void SetupRevertReplicatedInputTimer();

	// Used to revert to the replicated input after a timer
	void RevertReplicatedInput();
	// ~Input

public:
	// ~Motor
	// Get the currently selected gear
	UFUNCTION(BlueprintPure)
	int32 GetCurrentGear() const
	{
		return VehicleInput.GetCurrentGear();
	}

	// Sets the current gear
	void SetCurrentGear(int32 NewGear, bool bManualShift);

	// Shifts the motor up or down
	void ShiftGear(bool bShiftUp);

	// Whether the motor throttle should currently be flipped
	UFUNCTION(BlueprintPure)
	bool GetFlipMotorThrottle() const
	{
		return VehicleInput.GetFlipMotorThrottle();
	}

	// Updates the throttle flipping
	void SetFlipMotorThrottle(bool bNewFlip);

private:
	// Called whenever the throttle flip flag has changed
	void OnFlipMotorThrottleChanged();

	// Called when the current gear has changed
	void OnCurrentGearChanged(bool bManualShift);
	// ~Motor

public:
	// ~Sirens
	// Safely sets the active siren index, only possible while the character is conscious
	void SetActiveSiren(int32 Index);

	// Cycles between all available sirens
	void CycleSiren();

	// Toggles the siren on and off
	void ToggleSiren();

	// Get the currently active siren index
	int32 GetActiveSirenIndex() const
	{
		return VehicleInput.GetSirenIndex();
	}

private:
	// Forcefully sets the active siren index
	void SetActiveSirenIndex(int32 Index);

	// To be called when the active siren index has been changed
	void OnActiveSirenIndexChanged();
	// ~Sirens

public:
	// ~Actuators
	// Marks an actuator as needing to be updated on the server
	void ReplicateActuatorToServer(UActuatorBrick* Actuator);

	// Returns the list of actuators pending replication and clears it
	const auto& GetActuatorsToReplicate()
	{
		return ActuatorsToReplicate;
	}

	// Clears the list of actuators pending to be replicated
	void ClearActuatorsToReplicate()
	{
		ActuatorsToReplicate.Reset();
	}

	// ~Actuators

	// ~Firing
	// Sets whether the character is currently firing
	void SetIsFiring(bool bNewFiring);
	// Changes the current action mode
	void SetFireActionMode(int32 NewMode);
	// Get the current fire action mode
	int32 GetFireActionMode() const
	{
		return FireActionMode;
	}

	// Get the input channel used for the current fire action
	FVehicleInputChannel GetFireActionInputChannel() const
	{
		return FVehicleInputChannel(FVehicleInput::FireActionFromIndex(FireActionMode));
	}

	// Cycles to the next action mode
	void CycleFireActionMode(int32 Dir);
	// Get the view rotation limits for players in this seat
	void GetViewRotationLimit(bool bHasItem, FRotator& OutMinRotation, FRotator& OutMaxRotation) const;

private:
	// Sets the value of the currently selected action mode
	void SetCurrentFireActionValue(bool bNewValue);
	// ~Firing

public:
	// ~View Target
	// Changes the current camera mode and brick
	void SetCamera(ESeatCameraMode NewMode, const FBrickEditorObjectID& NewCameraID = nullptr);

	// Cycles between available camera
	void CycleCamera(int32 Dir);

	// Toggle between third person and the last camera used
	void ToggleThirdPersonCamera();

	bool IsInThirdPersonCamera() const
	{
		return CameraMode == ESeatCameraMode::ThirdPerson;
	}

	bool IsInFirstPersonCamera() const
	{
		return CameraMode == ESeatCameraMode::FirstPerson;
	}

	bool CanUseFirstPersonCamera() const
	{
		// Don't allow for hidden seats
		return IsVisibleSeat();
	}

	UCameraBrick* GetViewTargetCamera() const;

	// Returns the index of the camera within the supported cameras
	int32 GetCameraIndex(const FBrickEditorObjectID& CameraID) const;

private:
	// Called whenever the view target was modified
	void OnViewTargetChanged() const;
	// ~View Target

	// Returns cameras that can be used by this seat
	void GetSupportedCameras(TArray<FBrickEditorObjectID>& OutCameras) const;

	// Interaction callbacks
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_EnterSeat(ABrickPlayerController* PC);
};
