// Copyright Fluppisoft, 2017

#pragma once

#include "Interfaces/Interface_CollisionDataProvider.h"
#include "Bricks/WheelBrick.h"
#include "CarWheelBrick.generated.h"

class UBodySetup;

UCLASS(Abstract)
class BRICKRIGS_API UCarWheelBrickStaticInfo : public UWheelBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Radius of the rim only
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float RimRadius;

	// Min damage needed to pop the tire
	UPROPERTY(EditDefaultsOnly, Category = Tire)
	float MinTirePopDamage;
	UPROPERTY(EditDefaultsOnly, Category = Tire)
	UParticleSystem* TirePopEmitter;
	UPROPERTY(EditDefaultsOnly, Category = Tire)
	USoundBase* TirePopSound;
	// ~Properties

	// ~Constructor
	UCarWheelBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UCarWheelBrick : public UWheelBrick, public IInterface_CollisionDataProvider
{
	GENERATED_BODY()

private:
	enum ECarWheelPrimitiveData
	{
		WheelRadius = Max,
		ContactLocation = WheelRadius + 1,
		ContactNormal = ContactLocation + 3,
		TireContactLocation = ContactNormal + 3,
		Compression = TireContactLocation + 3,
		Max = Compression + 1
	};

	// ~Variables
	// Handle used for the OnPhysDispatchNotifications delegate
	FDelegateHandle DelegateHandle_PhysDispatchNotificationsForWheel;
	// Last frame where the wheel has received a hit notification
	uint32 LastWheelHitFrame;
	// Set to true when the component has just woken up
	bool bJustWokeUp;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(EditDefaultsOnly, Category = Wheel)
	float TirePressureRatio;
	// ~Brick Properties

public:
	// ~Constructor
	UCarWheelBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void SetupCreateStaticMeshComponentParams(FBrickStaticMeshComponentParams& Params) override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void RepairBrick() override;
	virtual bool ShouldSpawnCollisionEffects(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const override;
	virtual void SpawnCollisionEffects(const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) override;
	virtual void ReceiveDamageInternal(int32 DamageDepth, float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutMin, FVector& OutMax) const override;
	virtual float GetDriveRadius() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

private:
	// Get the 3D scale factor for the wheel collision
	FVector GetWheelCollisionScale3D() const;
	// Updates the tire material with a given hit or null
	void UpdateTireMaterial(const FPostContactModifyParams* Params);
	// Callback for the root component
	UFUNCTION()
	void OnRootComponentWake(UPrimitiveComponent* SleepingComponent, FName BoneName);
	// Callback for the global physics delegate
	void OnPhysDispatchNotificationsForWheel(FPhysScene* PhysicsScene);
	// Used to bind unbind the physics delegate
	void BindOnPhysDispatchNotificationsForWheel();
	void UnbindOnPhysDispatchNotificationsForWheel();
};
