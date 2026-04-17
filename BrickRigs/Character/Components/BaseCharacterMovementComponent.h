// Copyright Fluppisoft, 2018

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseCharacterMovementComponent.generated.h"

class ABaseCharacter;

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBaseCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Target yaw rotation of the body if it is currently turning
	float TargetBodyYaw;
	// Value counting up to 1 in order to play the next footstep sound
	float FootstepCycle;

public:
	// Whether the character wants to sprint
	uint8 bWantsToSprint : 1;
	// Whether the character wants to aim
	uint8 bWantsToAim : 1;
	// Whether the character wants to fire, used to cancel sprinting
	uint8 bWantsToFire : 1;
	// ~Variables

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float MaxSprintSpeed;
	// ~Properties

	// ~Constructor
	UBaseCharacterMovementComponent();

	// ~Super Interface
	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PhysicsRotation(float DeltaTime) override;
	virtual bool ForcePositionUpdate(float DeltaTime) override;
	virtual void SetDefaultMovementMode() override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxAcceleration() const override;
	virtual float GetMinAnalogSpeed() const override;
	virtual float ImmersionDepth() const override;
	virtual void UpdateCharacterStateAfterMovement(float DeltaSeconds) override;
	virtual bool ClientUpdatePositionAfterServerUpdate() override;
	virtual bool VerifyClientTimeStamp(float TimeStamp, FNetworkPredictionData_Server_Character& ServerData) override;
	// Overridden for our custom net prediction data class
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	// Overridden for our custom flags
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void CapsuleTouched(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	// ~Super Interface

	// Get the owning character
	ABaseCharacter* GetCharacter() const;
	ABaseCharacter* GetCharacterChecked() const;

	// Public accessor to the footstep cycle
	float GetFootstepCycle() const;
	// Return whether the character wants to and can aim
	bool ShouldAim() const;
	// Whether the player currently wants to sprint
	bool WantsToSprint() const;
	// Return whether the character wants to and can sprint
	bool ShouldSprint() const;
	// Return the maximum rotation rate for the body, can be overridden for different stances for example
	float GetYawRotationRate() const;

private:
	// Called every frame to update the rotation
	void UpdateCharacterRotation(float DeltaTime);
	// Get the current footstep interval, i.e. the time between footsteps and the volume
	bool GetFootstepParams(float& OutInterval, float& OutVolume) const;
	// Returns how fast the character can sprint in the current direction
	float GetMaxSprintSpeed() const;
	// Note: We need this helper function because the one from the subclass is protected and not mutable in a const function
	void SetPredictionData(class FNetworkPredictionData_Client_BaseCharacter* PredictionData);
};

class FSavedMove_BaseCharacter : public FSavedMove_Character
{
public:
	uint8 bWantsToSprint : 1;
	uint8 bWantsToAim : 1;
	uint8 bWantsToFire : 1;

	FSavedMove_BaseCharacter();

	// ~Super Interface
	virtual void SetMoveFor(ACharacter* C, float InDeltaTime, const FVector& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
	virtual void PostUpdate(ACharacter* C, EPostUpdateMode PostUpdateMode) override;
	virtual bool IsImportantMove(const FSavedMovePtr& LastAckedMove) const override;
	virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InPawn, float MaxDelta) const override;
	virtual uint8 GetCompressedFlags() const override;
	// ~Super Interface

	friend class UBaseCharacterMovementComponent;
};

class FNetworkPredictionData_Client_BaseCharacter : public FNetworkPredictionData_Client_Character
{
public:
	FNetworkPredictionData_Client_BaseCharacter(const UCharacterMovementComponent& ClientMovement);

	// ~Super Interface
	virtual FSavedMovePtr AllocateNewMove() override;
	// ~Super Interface
};
