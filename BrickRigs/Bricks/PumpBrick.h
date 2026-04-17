// Copyright Fluppisoft, 2016

#pragma once

#include "Components/FireExtinguisherComponent.h"
#include "Bricks/Brick.h"
#include "PumpBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UPumpBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Pump)
	FFireExtinguisherProperties ExtinguisherProperties;
	// ~Properties

	// ~Constructor
	UPumpBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UPumpBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	bool bInputChannelValue;
	// ~Variables

	// ~Components
	TWeakObjectPtr<UFireExtinguisherComponent> ExtinguisherComp;
	// ~Components

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Pump)
	FVehicleInputChannel InputChannel;
	// ~Brick properties

public:
	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnIsControllableChanged() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Whether the pump is currently active
	bool IsPumpActive() const
	{
		return ExtinguisherComp.IsValid() && ExtinguisherComp->IsActive();
	}

private:
	// Activates or deactivates the pump if needed
	void UpdatePumpActive();
};
