// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "World/StaticMeshProp.h"
#include "Windmill.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API AWindmill : public AStaticMeshProp
{
	GENERATED_BODY()

	// ~Variables
	// Current rotation of the rotor
	float CurrentRotation;
	// Current rotation speed
	float RotationSpeed;
	// Saved rand seed generated from the static mesh prop rand stream
	UPROPERTY()
	uint8 WindmillRandSeed;
	// ~Variables

protected:
	// ~Components
	UPROPERTY(Transient, BlueprintReadOnly)
	UStaticMeshComponent* RotorMeshComponent;
	// ~Components

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Windmill)
	UStaticMesh* RotorMesh;
	// Min and max random rotation speed in degrees per second
	UPROPERTY(EditAnywhere, Category = Windmill)
	FFloatInterval RotationSpeedRange;
	// ~Properties

public:
	// ~Constructor
	AWindmill();

	// ~Super Interface
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	// ~Super Interface

private:
	// Updates the current rotation
	void SetCurrentRotation(float NewRotation);
	// Updates the rotation speed
	void SetRotationSpeed(float NewSpeed);
};
