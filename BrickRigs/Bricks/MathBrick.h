// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "ScalableBrick.h"
#include "MathBrick.generated.h"

UENUM(BlueprintType)
enum class EMathBrickOperation : uint8
{
	Add,
	Subtract,
	Multiply,
	Divide,
	Fmod,
	Power,
	Greater,
	Less,
	Min,
	Max,
	MAX_Binary = Max,
	Abs,
	Sign,
	Round,
	Ceil,
	Floor,
	Sqrt,
	Sin,
	SinDeg,
	Asin,
	AsinDeg,
	Cos,
	CosDeg,
	Acos,
	AcosDeg,
	Tan,
	TanDeg,
	Atan,
	AtanDeg,
	MAX_Unary = Atan
};

UCLASS(Abstract)
class BRICKRIGS_API UMathBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UMathBrickStaticInfo();
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UMathBrick : public UScalableBrick
{
	GENERATED_BODY()

	enum class EMathBrickPrimitiveData : uint8
	{
		Output = Max,
		Max = Output + 1
	};

	// ~Variables
	uint8 InputChannelDirtyCount;
	// Cached input channel values
	float InputChannelAValue;
	float InputChannelBValue;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Math)
	EMathBrickOperation Operation;
	UPROPERTY(EditDefaultsOnly, Category = Math)
	FVehicleInputChannel InputChannelA;
	UPROPERTY(EditDefaultsOnly, Category = Math)
	FVehicleInputChannel InputChannelB;
	UPROPERTY(EditAnywhere, Category = Math)
	FVehicleOutputChannel OutputChannel;
	// ~Brick properties

public:
	// ~Constructor
	UMathBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual void PostConstructVehicle() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PreRepairBrick(const FTransform& SpawnTransform) override;
	virtual void RepairBrick() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void OnIsControllableChanged() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;

	virtual bool HasAnyOutputChannel() const override
	{
		return true;
	}

	virtual FVehicleOutputChannel* GetOutputChannel() const override
	{
		return const_cast<FVehicleOutputChannel*>(&OutputChannel);
	}

	// ~Super Interface

private:
	// Whether the given operation is binary rather than unary
	static bool IsBinaryOperation(EMathBrickOperation Op);
	// Applies the selected operation to the two values
	float ApplyOperation(float A, float B) const;
	// To be called when any input channel has changed
	void MarkInputChannelsDirty()
	{
		// Set to two so ticking isn't disabled right away, would be inefficiently if the value is going to be marked dirty every frame
		InputChannelDirtyCount = 2;
		UpdateBrickTickEnabled();
	}

	// Updates the output channel value
	void SetOutputChannel(const float Value, const bool bInRepairing);
	// Returns the value to use for the material parameter
	float GetOutputMaterialParam() const;

	// Property callbacks
	static void GetOperationItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
