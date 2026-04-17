// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "PawnIconWidget.generated.h"

class APawn;
class ABrickPlayerState;

/**
 * Base class for all pawn icons
 */
UCLASS(Abstract)
class BRICKRIGS_API UPawnIconWidget : public UHUDIconWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Owning pawn of the associated icon component
	UPROPERTY(Transient)
	APawn* OwningPawn;
	// Whether the pawn can be damaged
	bool bCanBeDamaged;
	// ~Variables

public:
	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual void UninitializeIcon_Implementation() override;
	// ~Super Interface

protected:
	// Pawn callbacks
	void OnPawnTeamAffiliationChanged();
	virtual void OnCanBeDamagedChanged(bool bNewCanBeDamaged);
	// Sets the invincibility state
	void SetCanBeDamaged(bool bNewCanBeDamaged);
};
