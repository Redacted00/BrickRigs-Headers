// Copyright Fluppisoft, 2017

#pragma once

#include "GenericTeamAgentInterface.h"
#include "Character/BaseCharacter.h"
#include "Zombie.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API AZombie : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// ~Constuctor
	AZombie(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void Kill(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual bool CanBePossessed() const override
	{
		return false;
	}

	virtual int32 GetDesiredFaceIndex() const override;
	// ~Super Interface
};
