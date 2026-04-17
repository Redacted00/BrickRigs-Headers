// Copyright Fluppisoft, 2016

#pragma once

#include "Interaction/InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

class UStaticMeshComponent;
class UAudioComponent;
class UInteractionComponent;

UENUM()
enum class EElevatorDirection : uint8
{
	Stop,
	Up,
	Down,
};

UCLASS(Abstract)
class BRICKRIGS_API AElevator : public AActor
{
	GENERATED_BODY()

private:
	// ~Variables
	// The current user entered direction the elevator is moving in
	EElevatorDirection ElevatorDirection;
	// The current height in meters of the elevator
	float ElevatorHeight;
	// The current target height, used for optimized replication
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ReplicatedTargetHeight)
	uint8 ReplicatedTargetHeight;
	UFUNCTION()
	void OnRep_ReplicatedTargetHeight();

protected:
	// The current speed of the elevator
	UPROPERTY(Transient, BlueprintReadOnly, Category = Elevator)
	float ElevatorSpeed;
	// ~Variables

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
	AElevator(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void Reset() override;
	// ~Super Interface

	// Changes the current movement direction, server only
	void SetElevatorDirection(EElevatorDirection Dir);

protected:
	// Implement to return the interactive locations
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetInteractionLocation(int32 Index) const;

private:
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
