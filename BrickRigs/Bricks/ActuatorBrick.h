// Copyright Fluppisoft, 2016

#pragma once

#include "FluMoveSyncPacer.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "Misc/ActuatorTypes.h"
#include "Vehicle/BrickVehicle.h"
#include "Bricks/Brick.h"
#include "ActuatorBrick.generated.h"

class ABrickPlayerController;
class UAudioComponent;
class UBrickEditorArrowComponent;
class USoundBase;
class USeatBrick;
class UBrickConnection;

USTRUCT()
struct FActuatorBrickEditorParams : public FBrickEditorParams
{
	GENERATED_BODY()

	// Arrow components used to visualize the actuation limits in the editor
	TArray<TBrickEditorComponentPtr<UBrickEditorArrowComponent>> ArrowComponents;
};

UCLASS(Abstract)
class BRICKRIGS_API UActuatorBrickBaseStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// The axis around which the actuator rotates/moves along
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	TEnumAsByte<EAxis::Type> ActuationAxis;
	// Offset in the actuation direction where the travel starts
	UPROPERTY(VisibleDefaultsOnly, Category = Actuator)
	float LinearActuationOffset;
	// Maximum travel of the actuator
	UPROPERTY(VisibleDefaultsOnly, Category = Actuator)
	float LinearActuationRange;
	// ~Properties

	// ~Constructor
	UActuatorBrickBaseStaticInfo();

#if WITH_EDITOR
	virtual void BuildCachedData() override;
#endif
};

UCLASS(Abstract)
class BRICKRIGS_API UActuatorBrickStaticInfo : public UActuatorBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// True if the actuator moves angular, false if it is a linear actuator
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	bool bIsAngularActuator;
	// Default actuator speed in degrees/second or cm/second
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	float ActuationSpeed;
	// Used for backwards compatibility of linear actuators
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	bool bIsLegacyLinearSlider;
	// The sound to play when moving the actuator
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	USoundBase* ActuatorSound;
	UPROPERTY(EditDefaultsOnly, Category = Replication)
	float ReplicationErrorSmoothTime = 0.2f;
	// ~Properties

	// ~Constructor
	UActuatorBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UActuatorBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// The current state of the actuator
	FActuatorState LocalActuatorState;
	// Last replicated state of the actuator
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ActuatorState)
	FActuatorState RepActuatorState;
	UFUNCTION()
	void OnRep_ActuatorState();
	// Used to smooth actuation replication
	TFluMoveSyncPacer<FActuatorTargetState> ActuationPacer;
	// Actuation value currently used
	float CurrentActuation;
	// Delta between the replicated and local actuation
	float RepActuationError;
	// Time remaining over which to smooth out the actuation error
	float ActuationErrorSmoothTime;
	// Current speed ratio of the actuator, used for sound
	float ActuationSpeedRatio;
	// All created actuator connections
	TArray<TWeakObjectPtr<UActuatorConnection>> ActuatorConnections;
	// Audio component for the movement sound
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> ActuatorAC;
	// Optional interaction component used to actuate via interaction
	TBrickEditorComponentPtr<UInteractionComponent> InteractionComponent;
	// The player currently applying interaction input
	UPROPERTY(Transient)
	ABrickPlayerController* InteractingPC;
	// Current input channel value
	float InputChannelValue;
	// Accumulated interaction input
	float InteractionInput;
	// Last time the player has interacted
	float LastInteractionTime;
	// Min and max actuation
	mutable FFloatInterval ActuationRange;
	// Cached flag indicating whether the actuator should be replicated
	uint8 bReplicateActuator : 1;
	// Whether the local player has authority over the actuator
	uint8 bHasActuatorAuthority : 1;
	// Whether the actuator state has been replicated before
	uint8 bActuatorStateReplicated : 1;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	EActuatorMode ActuatorMode;
	// Optional input axis to be used for this actuator 
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	float SpeedFactor;
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	float MaxLimit;
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	float MinLimit;
	// ~Brick Properties

public:
	// ~Constructor
	UActuatorBrick();

	// ~Super Interface
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void PostConstructVehicle() override;
	virtual void TickBrick(float DeltaTime) override;
	virtual bool ShouldBrickTick() const override;
	virtual bool ShouldReplicate() const override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return IsControllableActuator();
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual UBrickConnection* CreateCustomBrickConnection(UBrick* OtherBrick, const FName& ConnectionName, FBrickConnectionParams& Params) const override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* InConnection) override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual bool ResolveRemovedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EAxis::Type MirrorAxis) const override;
	virtual void UpdateEditorVisualization() override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeEditorParams<FActuatorBrickEditorParams>();
	}

	// ~Super Interface

	// Returns whether the actuator is of the angular variety
	bool IsAngularActuator() const
	{
		return GetStaticInfo<UActuatorBrickStaticInfo>()->bIsAngularActuator;
	}

	// Get the actuation direction/rotation axis
	EAxis::Type GetActuationAxis() const
	{
		return GetStaticInfo<UActuatorBrickStaticInfo>()->ActuationAxis;
	}

	// Returns the maximum distance a linear actuator can travel in either direction
	float GetLinearActuationRange() const
	{
		return GetStaticInfo<UActuatorBrickStaticInfo>()->LinearActuationRange;
	}

	// Get the vector around/along which actuation happens
	auto GetActuationVector() const
	{
		return GetWorldTransform().GetUnitAxis(GetActuationAxis());
	}

	// Get the mode of this actuator
	EActuatorMode GetActuatorMode() const
	{
		return ActuatorMode;
	}

	// Get the user specified speed factor
	float GetActuationSpeedFactor() const
	{
		return SpeedFactor;
	}

	// Outputs the min and max actuation
	const auto& GetActuationRange() const
	{
		return ActuationRange;
	}

	// Whether the min and max limit properties should be used
	bool ShouldUseMinMaxAngularLimit() const
	{
		return MinLimit + KINDA_SMALL_NUMBER < MaxLimit;
	}

	// Whether the actuation range is limited
	bool IsActuationRangeLimited() const
	{
		return !IsAngularActuator() || ShouldUseMinMaxAngularLimit() || ActuatorMode == EActuatorMode::Seeking;
	}

	// Helper to indicate if this actuator can be controlled, i.e. is not physics driven or static
	bool IsControllableActuator() const
	{
		return ActuatorMode == EActuatorMode::Accumulated || ActuatorMode == EActuatorMode::Seeking || ActuatorMode == EActuatorMode::Cycle;
	}

	// Whether there is at least one in tact actuator connection
	bool HasActiveActuatorConnection() const
	{
		for (const auto& Connection : ActuatorConnections)
		{
			if (Connection.IsValid() && !Connection->IsBroken())
			{
				return true;
			}
		}

		return false;
	}

	// Whether the actuator can be moved through interaction
	bool CanActuateViaInteraction() const
	{
		return GetSeatControllingActuator() == nullptr && InputChannel.InputAxis != EVehicleInputAxis::Custom && IsControllableActuator() && HasActiveActuatorConnection();
	}

	// Public accessor to the actuator state
	const auto& GetActuatorState() const
	{
		return LocalActuatorState;
	}

	// Returns the current actuation value
	float GetActuation() const
	{
		return GetActuation(LocalActuatorState);
	}

	// Version that takes any actuator state
	float GetActuation(const FActuatorState& InState) const
	{
		const auto Actuation = InState.GetActuation();
		return FMath::Clamp(Actuation * FMath::Max(FMath::Abs(ActuationRange.Min), FMath::Abs(ActuationRange.Max)), ActuationRange.Min, ActuationRange.Max);
	}

	// Get the seat responsible for the actuator
	USeatBrick* GetSeatControllingActuator() const;

	// Called from the character or player controller server RPC
	void ServerUpdateActuatorState(const FActuatorState& NewState, ABrickPlayerController* PC = nullptr);

private:
	// To be called whenever the actuator connections have changed
	void OnActuatorConnectionAddedOrRemoved();
	// Updates the authority state
	void UpdateActuatorAuthority();
	// Whether the local client should currently have authority
	bool ShouldHaveActuatorAuthority() const;
	// Returns the player controller and state with authority over the actuator
	TTuple<AController*, ABrickPlayerState*> GetAuthoritativePlayer() const;
	// Updates the actuator state, server and all clients
	void SetActuatorState(const FActuatorState& NewState, const bool bReset = false);
	// Updates the replicated actuator state
	void SetRepActuatorState(const FActuatorState& NewState);
	// Updates the current actuation
	void SetCurrentActuation(const float NewActuation);
	// Assigns the interacting PC
	void SetInteractingPC(ABrickPlayerController* PC);
	// Creates or destroys the interaction component as needed
	void UpdateInteractionComponent();
	// Interaction callbacks
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_Actuate(ABrickPlayerController* OtherPC, float Val);
	// Fades out the actuation sound
	void StopActuatorSound() const;

	// Property callbacks
	static void GetActuatorModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
