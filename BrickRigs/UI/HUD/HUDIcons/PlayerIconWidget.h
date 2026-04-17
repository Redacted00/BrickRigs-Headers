// Copyright Fluppisoft, 2020

#pragma once

#include "Character/CharacterHealth.h"
#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/PawnIconWidget.h"
#include "PlayerIconWidget.generated.h"

class ABaseCharacter;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPlayerIconWidget : public UPawnIconWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The outer character of the icon component
	UPROPERTY(Transient)
	ABaseCharacter* Character;
	// The player state of the character
	UPROPERTY(Transient)
	ABrickPlayerState* CharacterPlayerState;
	// Whether this is the icon for the local player
	uint32 bIsLocalPlayer : 1;
	// Whether the owner is team leader currently
	uint32 bIsTeamLeader : 1;
	// Cached state of health
	ECharacterStateOfHealth StateOfHealth;
	// ~Variables

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual void UninitializeIcon_Implementation() override;
	virtual FText GetIconDisplayName_Implementation() const override;
	virtual bool GetIconWorldRotation_Implementation(float& OutRotation) const override;
	virtual void OnCanBeDamagedChanged(bool bNewCanBeDamaged) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePlayerIcon(bool bNewIsLocalPlayer, bool bNewIsTeamLeader, ECharacterStateOfHealth NewStateOfHealth, bool bNewCanBeDamaged);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with the required parameters
	void UpdatePlayerIcon();
	// Sets the owning characters player state and binds/unbinds delegates
	void SetCharacterPlayerState(ABrickPlayerState* NewPlayerState);
	// Character callback
	void OnCharacterPlayerStateChanged(ABrickPlayerState* NewPlayerState);
	// Sets the team leader flag
	void SetIsTeamLeader(bool bNewTeamLeader);
	// Player state callback
	void OnIsTeamLeaderChanged(bool bNewTeamLeader);
	// Sets the state of health
	void SetStateOfHealth(ECharacterStateOfHealth NewState);
	// Character callback
	void OnStateOfHealthChanged(ECharacterStateOfHealth NewState);
};
