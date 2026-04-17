// Copyright Fluppisoft, 2019

#pragma once

#include "TimerManager.h"
#include "CharacterActionTypes.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CharacterAction.generated.h"

// Actors
class ABaseCharacter;
class ABrickCharacter;
// Components
class UAudioComponent;
// Assets
class UAnimMontage;

/**
 *
 */
UCLASS(Abstract, Blueprintable, Within = BaseCharacter)
class BRICKRIGS_API UCharacterAction : public UObject
{
	GENERATED_BODY()

protected:
	struct FActionTimer
	{
		float Time;
		FTimerDelegate Delegate;
		bool bInvoked;

		FActionTimer(float InTime, const FTimerDelegate& InDelegate)
			: Time(InTime), Delegate(InDelegate), bInvoked(false)
		{
		}
	};

private:
	// ~Variables
	// NOTE: This is stored as a separate pointer (rather than the outer) since it is also used on the CDO
	// IMPORTANT: This CANNOT be a UPROPERTY pointer! Would cause a GC crash due to the CDO referencing an actor
	TWeakObjectPtr<ABaseCharacter> CharacterPrivate;
	// World time seconds when the action has been started
	float ActionStartTime;
	// Whether this action has been started locally, false if started from replication
	uint8 bStartedLocally : 1;
	// Whether the action has been finished yet
	uint8 bHasFinished : 1;
	// Our current net mode, cached upon initialization
	ENetMode NetMode;
	// List of active timers
	TArray<FActionTimer> Timers;
	// Cached pointer to the audio component that is playing the sound for this animation
	TWeakObjectPtr<UAudioComponent> FoleyAudioComponent;

protected:
	// Indicates who has the authority over this action
	ECharacterActionAuth ActionAuthority;
	// Sequence to play on the item, used by UItemAction's
	UPROPERTY(Transient)
	UAnimMontage* ItemMontage;
	// ~Variables

	// ~Properties
	// Time until this action completes, <= 0 means indefinite
	UPROPERTY(EditDefaultsOnly, Category = Action)
	float ActionLength;
	// Playback speed scale
	UPROPERTY(EditDefaultsOnly, Category = Action)
	float PlayRate;
	// Layer to play the animation on
	UPROPERTY(EditDefaultsOnly, Category = Action)
	ECharacterActionLayer ActionLayer;
	// Animation montage to be played on the character
	UPROPERTY(EditDefaultsOnly, Category = Action)
	UAnimMontage* CharacterMontage;
	// The sound to be played along with the action
	UPROPERTY(EditDefaultsOnly, Category = Action)
	USoundBase* FoleySound;
	// ~Properties

public:
	// ~Constructor
	UCharacterAction();

	// ~Super Interface
	virtual UWorld* GetWorld() const override;
	virtual bool IsSupportedForNetworking() const override;
	// ~Super Interface

	// Return whether a new instance of our action can be started with the given params
	virtual bool CanStartAction() const;
	// Return whether the other action is allowed to be started
	virtual bool CanStartOtherAction(const UCharacterAction* OtherAction) const;

	// Get the outer character
	ABaseCharacter* GetCharacter() const;
	// Get the character as a brick character
	ABrickCharacter* GetBrickCharacter() const;

	// Public accessor for the action authority setting
	ECharacterActionAuth GetActionAuthority() const;
	// Public accessor to the action layer
	ECharacterActionLayer GetActionLayer() const;
	// Get the anim slot node name to play our character animation on
	FName GetCharacterAnimSlot() const;
	// Return whether this action has been started on the local machine
	bool WasStartedLocally() const;
	// Get time since the action has been started
	float GetTimeSinceStart() const;
	// Get our net mode
	ENetMode GetNetMode() const;
	// Helper function to compare the net mode
	bool IsNetMode(ENetMode Mode) const;
	// For soldier actions only, return whether the player can aim while the action is performing
	virtual bool CanCharacterAim() const;
	// Return whether the character is allowed to fire their weapon during this action
	virtual bool CanCharacterFire() const;

	// Initialize the basic parameters of an action
	void InitializeAction(ABaseCharacter* InCharacter, bool bInvokedLocally);
	// Required for actions without parameters
	void SetActionParams();
	// Play animation, start timers etc.
	virtual void StartAction();
	// Virtual hook to play the animation
	virtual void StartAnimation();
	// Tick the action, server and clients
	virtual void TickAction(float DeltaTime);
	// Helper function to finish this action successfully, for example called by the action timer
	void FinishAction();
	// Helper function to let the character cancel this action
	void CancelAction();
	// Whether the action has been finished yet
	bool HasActionFinished() const
	{
		return bHasFinished;
	}

	void MarkFinished()
	{
		bHasFinished = true;
	}

	// Called whenever the action is finished or cancelled
	virtual void OnActionFinished(bool bSuccess);

protected:
	// Add a timer to the action, return the index
	int32 AddActionTimer(const FActionTimer& Timer);
	// Stop a timer and mark it as executed
	void ClearTimer(int32 TimerIndex);
	// Override the time remaining for a certain timer
	void SetTimerRemaining(int32 TimerIndex, float Remaining);
	// Get the time remaining for the given timer
	float GetTimerRemaining(int32 TimerIndex) const;
	// Return whether the timer is still running
	bool IsTimerActive(int32 TimerIndex) const;
};

FORCEINLINE ECharacterActionAuth UCharacterAction::GetActionAuthority() const
{
	return ActionAuthority;
}

FORCEINLINE ECharacterActionLayer UCharacterAction::GetActionLayer() const
{
	return ActionLayer;
}

FORCEINLINE bool UCharacterAction::WasStartedLocally() const
{
	return bStartedLocally;
}

FORCEINLINE ENetMode UCharacterAction::GetNetMode() const
{
	return NetMode;
}

FORCEINLINE bool UCharacterAction::IsNetMode(ENetMode Mode) const
{
	return Mode == NetMode;
}
