// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "ScalableBrick.h"
#include "DisplayBrick.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UDisplayBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UDisplayBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UDisplayBrick : public UScalableBrick
{
	GENERATED_BODY()

	enum EDisplayPrimitiveData
	{
		ValueParam = Max,
		FractionalDigitsParam = ValueParam + 1,
		IntegralDigitsParam = FractionalDigitsParam + 1,
		DigitsParam = IntegralDigitsParam + 1,
		DisplayColorParam = DigitsParam + 1,
		Max = DisplayColorParam + 3
	};

	// ~Variables
	float InputChannelValue;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FBrickColor DisplayColor;
	UPROPERTY(EditDefaultsOnly, Category = Actuator)
	uint8 NumFractionalDigits = 1;
	// ~Brick properties

public:
	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults() override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual void OnIsControllableChanged() override;
	// ~Super Interface

private:
	// Updates the displayed value
	void UpdateDisplay() const;
	// Updates the value to display in the editor
	void UpdateEditorValue();

	// Return desired material parameter values
	float GetValueParam() const
	{
		return InputChannelValue;
	}

	float GetFractionalDigitsParam() const
	{
		return NumFractionalDigits;
	}

	float GetIntegralDigitsParam() const
	{
		// NOTE: The number of digits has to be limited to not overflow the floating point value
		return FMath::Min(FMath::RoundToInt(GetDigitsParam()) - NumFractionalDigits, 7);
	}

	float GetDigitsParam() const
	{
		const auto Scale = GetScalableBrickScale3D();
		return FMath::Max(FMath::RoundToFloat(Scale.X / Scale.Y), 1.f);
	}

	FVector GetDisplayColorParam() const
	{
		return !IsSpawned() || IsControllable() ? FVector(DisplayColor.ToLinearRGB()) : FVector::ZeroVector;
	}
};
