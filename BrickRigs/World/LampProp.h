// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "World/StaticMeshProp.h"
#include "LampProp.generated.h"

class ULightComponent;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API ALampProp : public AStaticMeshProp
{
	GENERATED_BODY()

private:
	// ~Variables
	// Whether the lamp is currently powered on
	bool bIsPoweredOn;
	// Randomly generated power on time range
	UPROPERTY()
	FFloatInterval PowerTimeRange;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	ULightComponent* LightComponent;
	// ~Components

protected:
	// ~Properties
#if WITH_EDITORONLY_DATA
	// Time at which the lamp is turned on
	UPROPERTY(EditAnywhere, Category = Lamp, Meta = (ClampMin = 0, ClampMax = 24))
	float PowerOnTime;
	// Time at which the lamp is turned off again
	UPROPERTY(EditAnywhere, Category = Lamp, Meta = (ClampMin = 0, ClampMax = 24))
	float PowerOffTime;
	// Maximum random time added or removed
	UPROPERTY(EditDefaultsOnly, Category = Lamp, Meta = (ClampMin = 0))
	float PowerTimeVariation;
#endif
	// Template for the light component to instantiate
	UPROPERTY(EditAnywhere, Category = Lamp)
	TSubclassOf<ULightComponent> LightComponentClass;
	// ~Properties

public:
	// ~Constructor
	ALampProp();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	// ~Super Interface

private:
	// Callback for the world setup actor
	void OnTimeOfDayChanged(float TimeOfDay);
};
