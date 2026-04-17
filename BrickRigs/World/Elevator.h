// Copyright Fluppisoft, 2016

#pragma once

#include "FluMoveSyncInterpolator.h"
#include "FluMoveSyncPacer.h"
#include "Interaction/InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

class UStaticMeshComponent;
class UAudioComponent;
class UInteractionComponent;

UENUM(BlueprintType)
enum class EElevatorDirection : uint8
{
	Stop,
	Up,
	Down,
};

USTRUCT(BlueprintType)
struct FElevatorState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	EElevatorDirection Direction = EElevatorDirection::Stop;
	UPROPERTY(BlueprintReadWrite)
	float Height = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float Speed = 0.f;

	friend bool operator==(const FElevatorState& LHS, const FElevatorState& RHS)
	{
		return LHS.Direction == RHS.Direction
			&& LHS.Height == RHS.Height
			&& LHS.Speed == RHS.Speed;
	}

	friend bool operator!=(const FElevatorState& LHS, const FElevatorState& RHS)
	{
		return !(LHS == RHS);
	}
};

UCLASS(Abstract)
class BRICKRIGS_API AElevator : public AActor
{
	GENERATED_BODY()

	// ~Variables
	// Local state of the elevator
	FElevatorState LocalElevatorState;
	// Replicated state of the elevator
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepElevatorState)
	FElevatorState RepElevatorState;
	UFUNCTION()
	void OnRep_RepElevatorState();
	TFluMoveSyncPacer<FElevatorState> MovementPacer;
	TFluMoveSyncLocationInterp<float> HeightInterp;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* ElevatorMesh;
	UPROPERTY(EditAnywhere, Category = Components)
	UAudioComponent* AudioComponent;
	UPROPERTY(Transient)
	UInteractionComponent* BaseIC;
	UPROPERTY(Transient)
	UInteractionComponent* TopIC;
	UPROPERTY(Transient)
	UInteractionComponent* PlatformIC;
	// ~Components

	// ~Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Elevator)
	float MaxHeight;
	UPROPERTY(EditAnywhere, Category = Elevator)
	float MaxSpeed;
	UPROPERTY(EditAnywhere, Category = Elevator)
	float Acceleration;
	// ~Properties

public:
	// ~Constructor
	AElevator();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;
	// ~Super Interface

	// Changes the current movement direction, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetElevatorDirection(const EElevatorDirection Dir);

protected:
	// ~Blueprint Interface
	// Implement to return the interactive locations
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetInteractionLocation(int32 Index) const;
	UFUNCTION(BlueprintImplementableEvent)
	void OnElevatorStateChanged(const FElevatorState NewState);
	// ~Blueprint Interface

	// Updates the elevator state, should only be called on the server
	UFUNCTION(BlueprintCallable)
	void SetElevatorState(const FElevatorState& NewState);

private:
	// Returns the effective height
	float GetEffectiveHeight() const;
	// Updates the current height
	void UpdateHeight();

	// ~Interaction
	// Callbacks for the interaction component
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	UFUNCTION()
	void Interact_StopElevator(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_ElevatorUp(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_ElevatorDown(ABrickPlayerController* PC);
	void Interact_SetElevatorDirection(ABrickPlayerController* PC, EElevatorDirection Dir);
	// ~Interaction
};
