// Copyright Fluppisoft, 2016

#pragma once

#include "World/ExplosiveMaterial.h"
#include "Bricks/Brick.h"
#include "TankBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UTankBrickBaseStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Capacity of the tank in liters
	UPROPERTY(EditDefaultsOnly, Category = Tank)
	float FuelCapacity;
	// Default fuel type to use
	UPROPERTY(EditDefaultsOnly, Category = Fuel)
	TSubclassOf<UExplosiveMaterial> FuelType;
	// ~Properties

	// ~Constructor
	UTankBrickBaseStaticInfo();

	// ~Super Interface
	virtual float GetBrickEditorObjectPrice() const override;
	// ~Super Interface

	// Calculates the price for the contained fuel
	float CalcFuelPrice(UClass* ExplosiveMaterial, float FuelLevel) const;
};

UCLASS(Abstract)
class BRICKRIGS_API UTankBrickStaticInfo : public UTankBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UTankBrickStaticInfo();
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UTankBrickBase : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// The last updated fuel level
	float LastFuelLevel;
	// ~Variables

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults() override;
	virtual float CalcBrickEditorObjectPrice() const override;
	virtual void PreRepairBrick(const FTransform& SpawnTransform) override;
	virtual void ReceiveDamageInternal(int32 DamageDepth, float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const override;
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) override;
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState) override;
	virtual void MarkBrickBurnt() override;
	virtual bool GatherFuelLevel(float& InOutLevel, float& InOutCapacity, int32& InOutNum) const override;
	virtual void UpdateFuelLevel() override;
	virtual void MarkFuelTankEmpty() override;
	// ~Super Interface

	// Public accessor to the fuel capacity
	float GetFuelCapacity() const;

private:
	// Returns the exact fuel level, server only
	float GetCurrentFuelLevel() const;

protected:
	// Get the fuel used by the tank
	virtual UClass* GetExplosiveMaterial() const
	{
		checkNoEntry();
		return nullptr;
	}

	// Used to access the CDO of the explosive material
	const auto* GetExplosiveMaterialCDO() const
	{
		auto* Class = GetExplosiveMaterial();
		return Class ? GetDefault<UExplosiveMaterial>(Class) : nullptr;
	}

public:
	// Explode the tank
	void Explode();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UTankBrick : public UTankBrickBase
{
	GENERATED_BODY()

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, Category = Tank)
	TSubclassOf<UExplosiveMaterial> FuelType;
	// ~Brick Properties

public:
	// ~Constructor
	UTankBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults() override;
	virtual UClass* GetExplosiveMaterial() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface
};
