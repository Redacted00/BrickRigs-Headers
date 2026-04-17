#pragma once

#include "Misc/FluTimer.h"
#include "Settings/BrickUserSettings.h"
#include "CoreMinimal.h"

// This struct is used as a timer for hold key actions
struct FHoldKeyTimer
{
	// To be called when the key has been pressed with the held callback
	template <class UserClass>
	void OnPressed(UserClass* Object, void (UserClass::*Function)(), const float Delay = GetHoldTime())
	{
		Timer.Set(Object, Function, Delay);
	}

	// Delegate version
	void OnPressed(const UObject* WorldContextObject, const FTimerDelegate& Delegate, const float Delay = GetHoldTime())
	{
		Timer.Set(WorldContextObject, Delegate, Delay);
	}

	// Weak lambda version
	void OnPressed(UObject* Object, const TFunction<void()>& Callback, const float Delay = GetHoldTime())
	{
		Timer.Set(Object, Callback, Delay);
	}

	// Version that doesn't take a callback
	void OnPressed(const UObject* WorldContextObject, const float Delay = GetHoldTime())
	{
		Timer.Set(WorldContextObject, Delay);
	}

	// To be called when the key has been released, return true if the key has been tapped and not held
	bool OnReleased(const UObject* WorldContextObject)
	{
		if (Timer.IsActive(WorldContextObject))
		{
			// Abort the hold timer
			Timer.Clear(WorldContextObject);
			return true;
		}

		return false;
	}

private:
	static float GetHoldTime()
	{
		return UBrickUserSettings::GetUserSettings()->GetHoldKeyTime();
	}

	FFluRealTimer Timer;
};
