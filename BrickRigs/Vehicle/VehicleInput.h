#pragma once

#include "Online/BitfieldSerializer.h"
#include "VehicleInputAxis.h"
#include "VehicleInput.generated.h"

USTRUCT()
struct FVehicleInput
{
	GENERATED_BODY()

	// ~Static Variables
	constexpr static int32 NumAxes = static_cast<int32>(EVehicleInputAxis::LastAxis) - static_cast<int32>(EVehicleInputAxis::FirstAxis) + 1;
	constexpr static int32 NumActions = static_cast<int32>(EVehicleInputAxis::LastAction) - static_cast<int32>(EVehicleInputAxis::FirstAction) + 1;
	constexpr static int32 NumPlayerActions = static_cast<int32>(EVehicleInputAxis::LastPlayerAction) - static_cast<int32>(EVehicleInputAxis::FirstPlayerAction) + 1;
	constexpr static int32 NumFireActions = static_cast<int32>(EVehicleInputAxis::LastFireAction) - static_cast<int32>(EVehicleInputAxis::FirstFireAction) + 1;
	// ~Static Variables

	// ~Statics
	static bool IsInputAxis(EVehicleInputAxis Axis)
	{
		return Axis > EVehicleInputAxis::None && Axis < EVehicleInputAxis::AlwaysOn;
	}

	static bool IsAxis(EVehicleInputAxis Axis)
	{
		return Axis >= EVehicleInputAxis::FirstAxis && Axis <= EVehicleInputAxis::LastAxis;
	}

	static bool IsAltAxis(EVehicleInputAxis Axis)
	{
		return Axis >= EVehicleInputAxis::FirstAltAxis && Axis <= EVehicleInputAxis::LastAltAxis;
	}

	static bool IsPlayerAction(EVehicleInputAxis Action)
	{
		return Action >= EVehicleInputAxis::FirstPlayerAction && Action <= EVehicleInputAxis::LastPlayerAction;
	}

	static bool IsFireAction(EVehicleInputAxis Action)
	{
		return Action >= EVehicleInputAxis::FirstFireAction && Action <= EVehicleInputAxis::LastFireAction;
	}

	static EVehicleInputAxis AxisFromIndex(int32 Index)
	{
		check(Index < NumAxes);
		return static_cast<EVehicleInputAxis>(Index + static_cast<int32>(EVehicleInputAxis::FirstAxis));
	}

	static int32 IndexFromAxis(EVehicleInputAxis Axis)
	{
		check(IsAxis(Axis));
		return static_cast<int32>(Axis) - static_cast<int32>(EVehicleInputAxis::FirstAxis);
	}

	static EVehicleInputAxis AxisToAltAxis(EVehicleInputAxis Axis)
	{
		return static_cast<EVehicleInputAxis>(static_cast<int32>(Axis) + NumAxes);
	}

	static EVehicleInputAxis AltAxisToAxis(EVehicleInputAxis Axis)
	{
		return static_cast<EVehicleInputAxis>(static_cast<int32>(Axis) - NumAxes);
	}

	static EVehicleInputAxis PlayerActionFromIndex(int32 Index)
	{
		return static_cast<EVehicleInputAxis>(Index + static_cast<int32>(EVehicleInputAxis::FirstPlayerAction));
	}

	static int32 IndexFromPlayerAction(EVehicleInputAxis Action)
	{
		check(IsPlayerAction(Action));
		return static_cast<int32>(Action) - static_cast<int32>(EVehicleInputAxis::FirstPlayerAction);
	}

	static EVehicleInputAxis FireActionFromIndex(int32 Index)
	{
		return static_cast<EVehicleInputAxis>(Index + static_cast<int32>(EVehicleInputAxis::FirstFireAction));
	}

	static int32 IndexFromFireAction(EVehicleInputAxis Action)
	{
		check(IsFireAction(Action));
		return static_cast<int32>(Action) - static_cast<int32>(EVehicleInputAxis::FirstFireAction);
	}

	// ~Statics

private:
	// ~Variables
	// Stores the state of all input axes
	float InputAxes[NumAxes];
	// Stores the state of all input actions
	// NOTE: Only player actions are included here, non player actions light the taillight are calculated locally on every client
	uint32 InputActions;
	// Gear the motor is in
	int8 Gear;
	// Whether the throttle and bake axes are flipped for the motor
	bool bFlipMotorThrottle;
	// The currently active siren, 0 means none
	uint8 SirenIndex;
	// ~Variables

public:
	// ~Constructor
	FVehicleInput()
	{
		// Reset the values initially, also makes sure the arrays contain the right amount of entries
		Reset();
	}

	void Reset()
	{
		FMemory::Memzero(InputAxes, NumAxes * sizeof(float));
		InputActions = 0;
		Gear = 0;
		bFlipMotorThrottle = false;
		SirenIndex = 0;
	}

	bool operator==(const FVehicleInput& Other) const
	{
		return FMemory::Memcmp(InputAxes, Other.InputAxes, NumAxes * sizeof(float)) == 0 && InputActions == Other.InputActions && Gear == Other.Gear && bFlipMotorThrottle == Other.bFlipMotorThrottle && SirenIndex == Other.SirenIndex;
	}

	bool operator!=(const FVehicleInput& Other) const
	{
		return !(*this == Other);
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		// NOTE: We wrap the axles to 254 and not 255 in order to have an accurate zero value
		if (Ar.IsSaving())
		{
			for (float Value : InputAxes)
			{
				uint8 Byte = (FMath::Clamp(Value, -1.f, 1.f) * 0.5f + 0.5f) * 254.f;
				Ar << Byte;
			}
		}
		else
		{
			for (float& Value : InputAxes)
			{
				uint8 Byte;
				Ar << Byte;
				Value = Byte / 254.f * 2.f - 1.f;
			}
		}

		TBitfieldSerializer<uint32> Bitfield(Ar);
		// Add the actions
		Bitfield.Add(InputActions, NumPlayerActions);
		// Store the gear
		// NOTE: We have to store the gear manually since it is signed and would break when a bit is removed
		bool bIsNegativeGear = Gear < 0;
		Bitfield.Add(bIsNegativeGear);
		uint8 AbsGear = FMath::Abs(Gear);
		Bitfield.Add(AbsGear, 6);
		// Store the reversing flag
		Bitfield.Add(bFlipMotorThrottle);
		Bitfield.Finish();

		if (Ar.IsLoading())
		{
			Gear = bIsNegativeGear ? -AbsGear : AbsGear;
		}

		Ar << SirenIndex;

		return true;
	}

	float GetAxis(EVehicleInputAxis Axis) const
	{
		const int32 AxisIndex = IndexFromAxis(Axis);
		return InputAxes[AxisIndex];
	}

	float GetAxis(EVehicleInputAxis Axis, float Min, float Max) const
	{
		return FMath::Clamp(GetAxis(Axis), Min, Max);
	}

	bool SetAxis(EVehicleInputAxis Axis, float Value)
	{
		Value = FMath::Clamp(Value, -1.f, 1.f);
		const int32 AxisIndex = IndexFromAxis(Axis);
		float& Ref = InputAxes[AxisIndex];
		if (Value != Ref)
		{
			Ref = Value;
			return true;
		}

		return false;
	}

	bool GetAction(EVehicleInputAxis Action) const
	{
		const auto ActionIndex = IndexFromPlayerAction(Action);
		return ((InputActions >> ActionIndex) & 1) != 0;
	}

	bool SetAction(EVehicleInputAxis Action, bool bValue)
	{
		if (bValue != GetAction(Action))
		{
			const auto ActionIndex = IndexFromPlayerAction(Action);
			InputActions &= ~(1 << ActionIndex);
			InputActions |= bValue ? (1 << ActionIndex) : 0;
			return true;
		}

		return false;
	}

	int32 GetCurrentGear() const
	{
		return Gear;
	}

	void SetCurrentGear(int32 NewGear)
	{
		Gear = NewGear;
	}

	bool GetFlipMotorThrottle() const
	{
		return bFlipMotorThrottle;
	}

	void SetFlipMotorThrottle(bool bNewFlip)
	{
		bFlipMotorThrottle = bNewFlip;
	}

	int32 GetSirenIndex() const
	{
		return SirenIndex - 1;
	}

	void SetSirenIndex(int32 InIndex)
	{
		SirenIndex = InIndex + 1;
	}
};

template <>
struct TStructOpsTypeTraits<FVehicleInput> : public TStructOpsTypeTraitsBase2<FVehicleInput>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};
