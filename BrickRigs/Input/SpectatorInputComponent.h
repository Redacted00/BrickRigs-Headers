// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "SpectatorInputComponent.generated.h"

class ABrickSpectatorPawn;
class ABrickVehicle;

/**
 *
 */
UCLASS(Within = BrickSpectatorPawn)
class BRICKRIGS_API USpectatorInputComponent : public UPawnInputComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached spectator pawn reference
	UPROPERTY(Transient)
	ABrickSpectatorPawn* SpectatorPawn;
	// ~Variables

public:
	// ~Constructor
	USpectatorInputComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface

protected:
	// Vehicle
	UFUNCTION()
	void OnPressedCaptureVehicleThumbnail();

	// Movement
	UFUNCTION()
	void MoveForward(float Val);
	UFUNCTION()
	void MoveRight(float Val);
	UFUNCTION()
	void MoveUp(float Val);
	UFUNCTION()
	void OnPressedShiftSpeed();
	UFUNCTION()
	void OnReleasedShiftSpeed();

	// Placement
	UFUNCTION()
	void RotatePawnCW(float Val);
	UFUNCTION()
	void OnPressedPlacePawn();
	UFUNCTION()
	void OnReleasedPlacePawn();
	UFUNCTION()
	void OnPressedSpawnDummy();
	UFUNCTION()
	void OnReleasedSpawnDummy();

private:
	// Callback for the player controller
	void OnPlayerVehicleChanged(ABrickVehicle* NewVehicle);
};
