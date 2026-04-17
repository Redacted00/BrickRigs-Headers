#pragma once

#include "Framework/Commands/InputChord.h"
#include "GameFramework/PlayerInput.h"
#include "CoreMinimal.h"

// Combines the properties of an input action and axis mapping
struct FInputActionAndAxisMapping : public FInputActionKeyMapping
{
	// ~Variables
	bool bIsAxis;
	float AxisScale;
	// ~Variables

	FInputActionAndAxisMapping()
		: FInputActionKeyMapping(), bIsAxis(false), AxisScale(0.f)
	{
	}

	FInputActionAndAxisMapping(const FInputActionKeyMapping& InActionMapping)
		: FInputActionKeyMapping(InActionMapping), bIsAxis(false), AxisScale(0.f)
	{
	}

	FInputActionAndAxisMapping(const FInputAxisKeyMapping& InAxisMapping)
		: FInputActionKeyMapping(InAxisMapping.AxisName, InAxisMapping.Key), bIsAxis(true), AxisScale(InAxisMapping.Scale)
	{
	}

	FInputActionAndAxisMapping(const FName& InActionName, bool bIsAxis, const FInputChord& InInputChord = FInputChord())
		: FInputActionKeyMapping(InActionName), bIsAxis(bIsAxis), AxisScale(0.f)
	{
		Key = InInputChord.Key;
		bCtrl = InInputChord.bCtrl;
		bShift = InInputChord.bShift;
		bAlt = InInputChord.bAlt;
		bCmd = InInputChord.bCmd;
	}

	bool operator==(const FInputActionAndAxisMapping& Other) const
	{
		return FInputActionKeyMapping::operator==(Other) && AxisScale == Other.AxisScale;
	}

	bool operator!=(const FInputActionAndAxisMapping& Other) const
	{
		return !(*this == Other);
	}

	FInputChord ToInputChord() const
	{
		return FInputChord(Key, bShift, bCtrl, bAlt, bCmd);
	}

	FInputAxisKeyMapping ToAxisMapping() const
	{
		return FInputAxisKeyMapping(ActionName, Key, AxisScale);
	}

	bool IsValid() const
	{
		return !ActionName.IsNone() && Key.IsValid();
	}

	bool IsInConflictWith(const FInputActionAndAxisMapping& Other) const
	{
		// If the bound key is a modifier key, which is used on the other action, we are in conflict
		if (Key.IsModifierKey() && Other.HasModifierKey(Key) || Other.Key.IsModifierKey() && HasModifierKey(Other.Key))
		{
			return true;
		}

		// NOTE: Check the physical keys, since a joystick axis could have two keys for the same axis and analog and digital for example
		if (GetPhysicalKey(Key) == GetPhysicalKey(Other.Key))
		{
			// NOTE: Having the same key bound once with a modifier key and once without does work for actions, but not for axes!
			if (bIsAxis || Other.bIsAxis)
			{
				return true;
			}

			// Check if the modifier keys match and could conflict
			if (bCtrl == Other.bCtrl && bShift == Other.bShift && bAlt == Other.bAlt && bCmd == Other.bCmd)
			{
				return true;
			}
		}

		return false;
	}

	bool HasModifierKey(const FKey& InKey) const
	{
		check(InKey.IsModifierKey());
		if (InKey == EKeys::LeftControl || InKey == EKeys::RightControl)
		{
			return bCtrl;
		}
		if (InKey == EKeys::LeftShift || InKey == EKeys::RightShift)
		{
			return bShift;
		}
		if (InKey == EKeys::LeftAlt || InKey == EKeys::RightAlt)
		{
			return bAlt;
		}
		if (InKey == EKeys::LeftCommand || InKey == EKeys::RightCommand)
		{
			return bCmd;
		}

		return false;
	}

	bool IsGamepadMapping() const
	{
		return Key.IsGamepadKey();
	}

	bool IsNegativeAxisMapping() const
	{
		return AxisScale < 0.f;
	}

	// Whether the key is an axis key which ranges in the full -1 to 1 value range, rather than just 0 to 1
	static bool IsFullAxisKey(const FKey& InKey);
	// Whether the given key is a float axis mapped to a virtual button
	static bool IsVirtualAxisKey(const FKey& InKey);
	// Get the physical key that corrosponds with the in key which might be virtual, for example gamepad stick up
	static const FKey& GetPhysicalKey(const FKey& InKey);
};
