#pragma once

#include "GenericTeamAgentInterface.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrickPawnInterface.generated.h"

class AController;
class ABrickPlayerController;
class ABrickPlayerState;

UINTERFACE()
class UBrickPawnInterface : public UInterface
{
	GENERATED_BODY()
};

class IBrickPawnInterface
{
	GENERATED_IINTERFACE_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, ABrickPlayerState*);
	DECLARE_MULTICAST_DELEGATE(FOnTeamAffiliationChanged);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCanBeDamagedChanged, bool);

private:
	// ~Variables
	// Cached reference to the owning player state
	TWeakObjectPtr<ABrickPlayerState> OwningPlayerStatePrivate;
	// ~Variables

public:
	// ~Delegates
	FOnPlayerStateChanged OnOwningPlayerStateChangedDelegate;
	FOnTeamAffiliationChanged OnTeamAffiliationChangedDelegate;
	FOnCanBeDamagedChanged OnCanBeDamagedChangedDelegate;
	// ~Delegates

	// ~Pawn
	// Returns the pawn the player controller should possess when taking control over this pawn
	virtual APawn* GetPawnToPossess() const;
	// Get the controller owning this pawn
	virtual AController* GetOwningController() const;
	// Get the owning controller as a player controller
	ABrickPlayerController* GetOwningPlayerController() const;
	// Implement to return the current owning player state
	virtual ABrickPlayerState* GetOwningPlayerState() const;
	// Returns whether the owning controller is a local player controller
	bool IsLocallyPlayerControlled() const;

protected:
	// Should be called when the player state could have changed
	void UpdateOwningPlayerState();

private:
	// Called when the owning player state has changed
	virtual void OnOwningPlayerStateChanged(ABrickPlayerState* OldPlayerState)
	{
	}

	// ~Pawn

public:
	// ~Camera
	// Updates the current view rotation and applies it to the controller
	void OverrideViewRotation(const FRotator& NewRotation);
	// ~Camera

	// ~Team
	// Implement to get all teams represented by this pawn
	virtual void GetTeamAffiliation(TArray<FGenericTeamId>& OutTeams) const = 0;

protected:
	// To be called if the team affiliation could have changed
	void OnTeamAffiliationChanged();
	// ~Team

public:
	// ~Damage
	// Enables or disables god mode, server only
	void SetCanBeDamagedFlag(bool bNewCanBeDamaged);
	// Whether god mode is enabled or not
	bool GetCanBeDamagedFlag() const;

protected:
	// Should be called whenever bCanBeDamaged has changed
	virtual void OnCanBeDamagedChanged();
	// ~Damage

public:
	// ~Spawning
	// Get the pawn bounds in local space
	virtual void GetPawnBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const = 0;
	// This should return the bottom center of the pawns bounds
	virtual FTransform GetPawnRestartTransform() const = 0;
	// Allows the pawn to modify the free cam spawn point
	virtual void OverridePawnFreeCamPlacement(const FHitResult& Hit, FVector& OutLocation, FRotator& OutRotation) const
	{
	}

	// ~Spawning
};
