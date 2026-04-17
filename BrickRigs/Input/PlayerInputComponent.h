// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Input/HoldKeyTimer.h"
#include "CoreMinimal.h"
#include "BaseInputComponent.h"
#include "PlayerInputComponent.generated.h"

class ABrickPlayerController;
class UInteractionComponent;

/**
 * Input component used on the player controller
 */
UCLASS(Within = BrickPlayerController)
class BRICKRIGS_API UPlayerInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

	// ~Variables
	FHoldKeyTimer HoldKeyTimer_MoveCamera;
	FTraceHandle TraceHandle_FindFocusedBrick;

	// The HUD icon component currently focused by the player
	TWeakObjectPtr<UHUDIconComponent> FocusedHUDIconComponent;
	// Current interaction options of the focused component
	FInteractionOptions FocusedInteractionOptions;
	// The brick the player is currently looking at
	TBrickEditorObjectPtr<UBrick> FocusedBrick;
	// Bitfield for interaction options that have been pressed but not yet released
	uint16 InteractionOptionsPendingRelease;
	// ~Variables

	// ~Components
	// Interaction component used for the focused brick
	UPROPERTY(Transient)
	UInteractionComponent* FocusedBrickInteractionComponent;
	// ~Components

public:
	// ~Constructor
	UPlayerInputComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetPlayerController(ABasePlayerController* PC) override;
	// ~Super Interface

	// Returns the currently focused component
	auto* GetFocusedHUDIconComponent() const
	{
		return FocusedHUDIconComponent.Get();
	}

protected:
	// Updates the focused brick
	void SetFocusedBrick(UBrick* InBrick);
	// Callback for the focused brick async sweep
	void OnFindFocusedBrickSweep(const FTraceHandle& InHandle, FTraceDatum& InDatum);

	// User Interface
	UFUNCTION()
	void OnPressedChat();
	UFUNCTION()
	void OnPressedScoreboard();

	UFUNCTION()
	void OnPressedCycleMeasurementSystem();
	UFUNCTION()
	FText GetCycleMeasurementSystemValueText() const;

	// Camera
	UFUNCTION()
	void OnPressedMoveCamera();
	UFUNCTION()
	void OnReleasedMoveCamera();
	void OnMoveCamera(bool bPressed);

	// Interaction
	UFUNCTION()
	void OnPressedInteractPri()
	{
		ExecuteInteraction(EInteractionOptionSlot::Pri, true);
	}

	UFUNCTION()
	void OnReleasedInteractPri()
	{
		ExecuteInteraction(EInteractionOptionSlot::Pri, false);
	}

	UFUNCTION()
	void OnPressedInteractSec()
	{
		ExecuteInteraction(EInteractionOptionSlot::Sec, true);
	}

	UFUNCTION()
	void OnReleasedInteractSec()
	{
		ExecuteInteraction(EInteractionOptionSlot::Sec, false);
	}

	UFUNCTION()
	void OnPressedInteractTer()
	{
		ExecuteInteraction(EInteractionOptionSlot::Ter, true);
	}

	UFUNCTION()
	void OnReleasedInteractTer()
	{
		ExecuteInteraction(EInteractionOptionSlot::Ter, false);
	}

	UFUNCTION()
	void InteractAxis(float Val)
	{
		ExecuteInteraction(EInteractionOptionSlot::Pri, true, Val);
	}

public:
	// Used to execute all interaction options
	bool ExecuteInteraction(EInteractionOptionSlot OptionSlot, bool bPressed = true, const TOptional<float>& AxisVal = TOptional<float>());

private:
	// Callbacks for the player controller
	void OnBrickEditorChanged(ABrickEditor* NewEditor);
	void OnIsInFreeCamChanged(bool bNewInFreeCam);
	void OnPlayerVehicleChanged(ABrickVehicle* NewVehicle);

	// Callbacks for the focused brick interaction
	bool ShouldFocusedBrickInteractionBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const;
	void GetFocusedBrickInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions) const;
	FVector GetFocusedBrickInteractionLocation() const;
};
