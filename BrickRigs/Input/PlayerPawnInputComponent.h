// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "PawnInputComponent.h"
#include "PlayerPawnInputComponent.generated.h"

class ABaseCharacter;
class ABrickCharacter;
class ABrickVehicle;

/**
 * The base class for playing pawns, i.e. characters and vehicles, NOT spectators
 */
UCLASS(Abstract)
class BRICKRIGS_API UPlayerPawnInputComponent : public UPawnInputComponent
{
	GENERATED_BODY()

protected:
	// ~Variables
	FHoldKeyTimer HoldKeyTimer_Kill;
	FHoldKeyTimer HoldKeyTimer_CycleSlots;
	FHoldKeyTimer HoldKeyTimer_CycleExplosives;
	// Cached pointer to the character
	UPROPERTY(Transient)
	ABaseCharacter* Character;
	// Cached pointer to the character as a brick character
	UPROPERTY(Transient)
	ABrickCharacter* BrickCharacter;
	// Cached vehicle owner
	UPROPERTY(Transient)
	ABrickVehicle* Vehicle;
	// ~Variables

public:
	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetPlayerController(ABasePlayerController* PC) override;
	// ~Super Interface

protected:
	// Sets the owning character reference
	void OnOwningCharacterChanged(ABaseCharacter* NewCharacter);
	virtual void SetOwningCharacter(ABaseCharacter* NewCharacter);
	// Sets the owning vehicle, used on subclasses
	void OnOwningVehicleChanged(ABrickVehicle* NewVehicle);
	virtual void SetOwningVehicle(ABrickVehicle* NewVehicle);

	// Pawns
	UFUNCTION()
	void OnPressedKill();
	UFUNCTION()
	void OnReleasedKill();
	UFUNCTION()
	void OnTappedKill();
	UFUNCTION()
	void OnHeldKill();

	// Gunplay
	UFUNCTION()
	void OnPressedAim();
	UFUNCTION()
	void OnReleasedAim();
	// Whether aiming should be toggle rather than hold
	bool IsAimToggle() const;
	UFUNCTION()
	void OnPressedFire();
	UFUNCTION()
	void OnReleasedFire();
	UFUNCTION()
	void OnPressedReload();
	UFUNCTION()
	void OnPressedThrowItem();
	UFUNCTION()
	void OnPressedCycleFireMode();
	// Helper function to cycle the fire mode
	void CycleFireMode();

	// Inventory
	UFUNCTION()
	void OnPressedCycleSlots()
	{
		HoldKeyTimer_CycleSlots.OnPressed(this, &ThisClass::OnHeldCycleSlots);
	}

	UFUNCTION()
	void OnReleasedCycleSlots()
	{
		if (HoldKeyTimer_CycleSlots.OnReleased(this))
		{
			OnTappedCycleSlots();
		}
	}

	UFUNCTION()
	void OnTappedCycleSlots()
	{
		SwitchToNextItem(1);
	}

	UFUNCTION()
	void OnHeldCycleSlots()
	{
		SwitchToNextItem(-1);
	}

	UFUNCTION()
	void OnPressedCycleExplosives();
	UFUNCTION()
	void OnReleasedCycleExplosives();
	UFUNCTION()
	void OnTappedCycleExplosives();
	UFUNCTION()
	void OnHeldCycleExplosives();

	UFUNCTION()
	void OnPressedUnequipItem();

	// Slots
	UFUNCTION()
	void OnPressedPrimarySlot()
	{
		SwitchToSlot(0);
	}

	UFUNCTION()
	void OnPressedSecondarySlot()
	{
		SwitchToSlot(1);
	}

	UFUNCTION()
	void OnPressedSpecialSlot()
	{
		SwitchToSlot(2);
	}

	// Switches to the item in the given special slot index
	void SwitchToSlot(int32 SlotIndex);
	// Cycle through the inventory and find the next equippable item with the given tag
	void SwitchToNextItem(int8 Dir, const FGameplayTag& ItemTag = FGameplayTag());
};
