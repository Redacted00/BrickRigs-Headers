// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Character/Actions/CharacterAction.h"
#include "MeleeAction.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UMeleeAction : public UCharacterAction
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float ApplyDamageTime;
	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float DamageSweepDistance;
	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float DamageSweepRadius;
	UPROPERTY(EditDefaultsOnly, Category = Melee)
	float MeleeDamage;
	// ~Properties

	// ~Constructor
	UMeleeAction();

	// ~Super Interface
	virtual void StartAction() override;
	// ~Super Interface

private:
	// Callback for the damage timer
	void OnApplyDamage();
};
