// Copyright Fluppisoft, 2017

#pragma once

#include "GenericTeamAgentInterface.h"
#include "SpawnArea.h"
#include "TeamBase.generated.h"

class ABrickPlayerController;
class UBillboardComponent;

UCLASS(Abstract)
class BRICKRIGS_API ATeamBase : public ASpawnArea
{
	GENERATED_BODY()

private:
	// ~Components
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UBillboardComponent* BillboardComponent;
#endif
	// ~Components

public:
	// ~Properties
	// The team that is allowed to spawn on this base
	UPROPERTY(EditAnywhere, Category = Spawn)
	FGenericTeamId TeamId;
	// ~Properties

	// ~Constructor
	ATeamBase();

	// ~ISpawnPointInterface
	virtual int32 GetSpawnSortPriority() const override;
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const override;
	// ~ISpawnPointInterface
};
