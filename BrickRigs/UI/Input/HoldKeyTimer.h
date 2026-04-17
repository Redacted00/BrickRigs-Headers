#pragma once

#include "Settings/BrickUserSettings.h"
#include "CoreMinimal.h"
#include "UI/Misc/BrickUITimer.h"

// This struct is used as a timer for key holding actions
struct FHoldKeyTimer : public FBrickUITimer
{
	// To be called when the key has been pressed with the held callback
	template <class UserClass, typename CallbackType, typename = typename TEnableIf<!TIsFloatingPoint<CallbackType>::Value>::Type>
	void OnPressed(UserClass* Object, CallbackType Function, float Delay = GetHoldTime())
	{
		StartTimer(Object, Function, Delay);
	}

	// Version that doesn't take a callback
	void OnPressed(UObject* Object, float Delay = GetHoldTime())
	{
		StartTimer(Object, Delay);
	}

	// To be called when the key has been released, return true if the key has been tapped and not held
	bool OnReleased()
	{
		if (IsTimerActive())
		{
			// Abort the hold timer
			CancelTimer();
			return true;
		}

		return false;
	}

private:
	static float GetHoldTime()
	{
		return UBrickUserSettings::GetUserSettings()->GetHoldKeyTime();
	}
};
