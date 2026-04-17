// Copyright Fluppisoft, 2017

#pragma once

#include "GameFramework/Actor.h"
#include "BrickPlayerStart.generated.h"

class ASpawnArea;
class UCapsuleComponent;
class UArrowComponent;

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickPlayerStart : public AActor
{
	GENERATED_BODY()

private:
	// ~Components
#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(Transient)
	UArrowComponent* ArrowComponent;
#endif
	// ~Components

public:
	// ~Properties
	// The spawn area this player start is part of
	UPROPERTY(EditAnywhere, Category = PlayerStart)
	TSoftObjectPtr<ASpawnArea> SpawnArea;
	// ~Properties

	// ~Constructor
	ABrickPlayerStart();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	// ~Super Interface
};
