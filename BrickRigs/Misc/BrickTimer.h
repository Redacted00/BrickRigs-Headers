#pragma once

#include "CoreMinimal.h"

class UBrickGameInstance;

// This struct can be used for real time timers
struct FBrickTimer
{
	FBrickTimer()
		: bIsTimerActive(false)
	{
	}

	~FBrickTimer()
	{
		// Make sure the timer is unregistered
		CancelTimer();
	}

	// Whether the timer is still running
	bool IsTimerActive() const
	{
		return bIsTimerActive;
	}

	// Gets the time remaining, also works while paused
	float GetPausedTimeRemaining() const
	{
		return FMath::Max(TimeRemaining, 0.f);
	}

	// Get the time remaining until the timer executes, returns -1 if the timer isn't active
	float GetTimeRemaining() const
	{
		return bIsTimerActive ? GetPausedTimeRemaining() : -1.f;
	}

	// Starts the timer
	void StartTimer(const FSimpleDelegate& InDelegate, float Delay)
	{
		check(InDelegate.IsBound());
		SetTimerDelegate(InDelegate);
		StartTimerInternal(InDelegate.GetUObject(), Delay);
	}

	// Version that starts a timer without setting a callback
	void StartTimer(UObject* Object, float Delay)
	{
		StartTimerInternal(Object, Delay);
	}

	// Shorter syntax version
	template <class UserClass, typename CallbackType, typename = typename TEnableIf<!TIsFloatingPoint<CallbackType>::Value>::Type>
	void StartTimer(UserClass* Object, CallbackType Function, float Delay)
	{
		SetTimerDelegate(Object, Function);
		StartTimerInternal(Object, Delay);
	}

	// Can be used to store delegate without actually starting the timer
	void SetTimerDelegate(const FSimpleDelegate& InDelegate)
	{
		Delegate = InDelegate;
	}

	// Member function version
	template <class UserClass>
	void SetTimerDelegate(UserClass* Object, typename TMemFunPtrType<false, UserClass, void()>::Type Function)
	{
		SetTimerDelegate(FSimpleDelegate::CreateUObject(Object, Function));
	}

	// Lambda version
	void SetTimerDelegate(UObject* Object, const TFunction<void()>& Function)
	{
		SetTimerDelegate(FSimpleDelegate::CreateWeakLambda(Object, Function));
	}

	// Resumes a stopped timer
	void ResumeTimer()
	{
		StartTimer(Delegate, TimeRemaining);
	}

	// Cancels the timer if it is currently active
	void CancelTimer();

	// Pauses the timer (same as cancelling, can be resumed anyways)
	void PauseTimer()
	{
		CancelTimer();
	}

	// Called from the game instance to update the time remaining
	bool TickTimer(float DeltaTime);
	// Called from the game instance to execute the timer delegate
	void ExecuteDelegate();

private:
	// Internal version called to start the timer
	void StartTimerInternal(UObject* Object, float Delay);
	// Whether the timer is currently running
	bool bIsTimerActive;
	// Hold timer remaining seconds
	float TimeRemaining;
	// Delegate to be called when the key has been held
	FSimpleDelegate Delegate;
	// Cached game instance this timer is bound to
	TWeakObjectPtr<UBrickGameInstance> GameInstance;
};
