#pragma once

#include "Serialization/FluSerializationStatics.h"
#include "ActuatorTypes.generated.h"

class UActuatorBrick;

UENUM()
enum class EActuatorMode : uint8
{
	Accumulated,
	Seeking,
	Cycle,
	PhysicsDriven,
	Static,
	Spring
};

USTRUCT()
struct FActuatorState
{
	GENERATED_BODY()

private:
	// ~Variables
	// Current actuation ratio, -1:1
	float Actuation = 0.f;
	// Current input value, -1:1
	float Input = 0.f;
	// Server time this state was created at
	float OwnerTimestamp = 0.f;
	// Used for the cycle drive mode, indicates that the drive direction is currently inverted
	bool bInvertCycleDirection = false;
	// ~Variables

public:
	bool operator==(const FActuatorState& Other) const
	{
		return Actuation == Other.Actuation && Input == Other.Input && bInvertCycleDirection == Other.bInvertCycleDirection;
	}

	bool operator!=(const FActuatorState& Other) const
	{
		return !(*this == Other);
	}

	bool IsNearlyEqual(const FActuatorState& Other) const
	{
		return FMath::IsNearlyEqual(Actuation, Other.Actuation, 0.001f) && FMath::IsNearlyEqual(Input, Other.Input, 0.01f) && bInvertCycleDirection == Other.bInvertCycleDirection;
	}

	float GetActuation() const
	{
		return Actuation;
	}

	void SetActuation(float NewActuation)
	{
		Actuation = NewActuation;
	}

	float GetInput() const
	{
		return Input;
	}

	void SetInput(float NewInput)
	{
		Input = NewInput;
	}

	float GetOwnerTimestamp() const
	{
		return OwnerTimestamp;
	}

	void SetOwnerTimestamp(float NewTimestamp)
	{
		OwnerTimestamp = NewTimestamp;
	}

	bool GetInvertCycleDirection() const
	{
		return bInvertCycleDirection;
	}

	void SetInvertCycleDirection(bool bNewInvert)
	{
		bInvertCycleDirection = bNewInvert;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		FFluSerializationStatics::SerializeFloatCompressed<uint16>(Ar, Actuation, -1.f, 1.f);
		FFluSerializationStatics::SerializeFloatCompressed<uint16>(Ar, Input, -1.f, 1.f);
		Ar << OwnerTimestamp;
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FActuatorState> : TStructOpsTypeTraitsBase2<FActuatorState>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

// Struct used to send actuator states via RPCs
USTRUCT()
struct FRepActuatorState
{
	GENERATED_BODY()

	// ~Variables
	TWeakObjectPtr<UActuatorBrick> Actuator;
	FActuatorState ActuatorState;
	// ~Variables

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Actuator;
		ActuatorState.NetSerialize(Ar, Map, bOutSuccess);
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FRepActuatorState> : TStructOpsTypeTraitsBase2<FRepActuatorState>
{
	enum
	{
		WithNetSerializer = true,
		// NOTE: Can't use this because of the brick reference, also not needed for RPCs
		WithNetSharedSerialization = false
	};
};

// Used for movement smoothing
struct FActuatorTargetState
{
	float Actuation;
	float Input;
};
