// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/BrickGameplayTags.h"
#include "UI/Input/HoldKeyTimer.h"
#include "CoreMinimal.h"
#include "Components/InputComponent.h"
#include "BaseInputComponent.generated.h"

class ABasePlayerController;
class ABrickPlayerController;
class ABrickEditor;
class IViewTargetInterface;

struct FBindInputActionsParams
{
private:
	struct FActionProperties
	{
		bool bRemovePrimary = false;
		bool bRemoveSecondary = false;
	};

	// ~Variables
	// The context tags to bind actions for
	FGameplayTagContainer Context;
	// Map of optional settings per action
	TMap<FName, FActionProperties> ActionProperties;
	// ~Variables

public:
	void AddContextTag(const FGameplayTag& InTag)
	{
		Context.AddTag(InTag);
	}

	const FGameplayTagContainer& GetContextTags() const
	{
		return Context;
	}

	void RemoveAction(const FName& InActionName, bool bRemovePrimary = true, bool bRemoveSecondary = true)
	{
		if (bRemovePrimary || bRemoveSecondary)
		{
			auto& FoundAction = ActionProperties.FindOrAdd(InActionName);
			FoundAction.bRemovePrimary |= bRemovePrimary;
			FoundAction.bRemoveSecondary |= bRemoveSecondary;
		}
	}

	const FActionProperties* GetActionProperties(const FName& InActionName) const
	{
		return ActionProperties.Find(InActionName);
	}

	const auto& GetActionProperties() const
	{
		return ActionProperties;
	}
};

/**
 * This is the very base class for all input components in game, for pawns and spectators
 */
UCLASS()
class BRICKRIGS_API UBaseInputComponent : public UInputComponent
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The owning player controller
	UPROPERTY(Transient)
	ABasePlayerController* BasePlayerController;
	UPROPERTY(Transient)
	ABrickPlayerController* PlayerController;
	// The context tags currently used
	FGameplayTagContainer InputContextTags;
	// Used for faster comparisions for the context menu
	uint32 InputHelpVersion;
	// ~Variables

public:
	// ~Properties
	// Whether the component should be included in the input help
	UPROPERTY(EditDefaultsOnly)
	uint32 bShowInInputHelp : 1;
	// Whether all actions should consume input
	UPROPERTY(EditDefaultsOnly)
	uint32 bForceConsumeInput : 1;
	// ~Properties

	// ~Constructor
	UBaseInputComponent();

	// ~Super Interface
	virtual void BeginPlay() override;
	// ~Super Interface

	// Clears all actions and sets them up again
	void ResetInputComponent();
	// Called upon registration, child classes should override this to bind actions
	void SetupInputComponent();

protected:
	// Unbinds all input actions
	void ClearInputActions();
	// Used to gather all actions that should be bound as well as exceptions
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const
	{
	}

public:
	// Should be used to set the player controller
	virtual void SetPlayerController(ABasePlayerController* PC);

	// Executes the action delegate for the given action and event
	bool ExecuteAction(const FName& ActionName, EInputEvent InputEvent, const FKey& Key);

	// Whether the given action and event type are bound currently
	bool IsActionBound(const FName& ActionName, EInputEvent InputEvent = IE_MAX) const;

	// Whether input is currently blocked on the viewport client
	bool IsInputIgnored() const;

	// Get the current context menu comparison version
	uint32 GetInputHelpVersion() const
	{
		return InputHelpVersion;
	}

	// Get the currently used context tags
	const FGameplayTagContainer& GetInputContextTags() const
	{
		return InputContextTags;
	}

	// Can be implemented by context menu input components
	virtual void SetContextMenuOwner(UObject* InOwner)
	{
	}

	// Optionally allows customizing the display info of any action or  axis
	virtual void OverrideActionDisplayInfo(const FName& ActionName, FDisplayInfo& InOutDisplayInfo) const
	{
	}

	// Allows overriding the quick access flag of any action or axis
	virtual bool OverrideActionQuickAccess(const FName& ActionName, bool bIsQuickAccess) const
	{
		return bIsQuickAccess;
	}

protected:
	// ~View
	UFUNCTION()
	virtual void MouseMoveUp(float Val);
	UFUNCTION()
	virtual void MouseMoveRight(float Val);
	UFUNCTION()
	virtual void LookUp(float Val);
	UFUNCTION()
	virtual void LookRight(float Val);

	UFUNCTION()
	void OnPressedPivotCamera();
	UFUNCTION()
	void OnReleasedPivotCamera();
	// This can be implemented to use the pivot input
	virtual bool OnPivotCamera(bool bPressed) { return false; }

	// Used to add mouse or gamepad view pitch input
	void AddViewPitchInput(float Val, float Scale);
	// Used to add mouse or gamepad view yaw input
	void AddViewYawInput(float Val, float Scale);
	// Whether view input can currently be added
	virtual bool CanAddViewInput() const
	{
		return true;
	}

	// Get the current input scale for the gamepad/keyboard view rate
	float GetRateLookInputScale() const;
	// ~View

	// ~Camera Movement
	UFUNCTION()
	void SpeedUp(float Val);
	UFUNCTION()
	void OnPressedSpeedUpStep();
	UFUNCTION()
	void OnPressedSpeedDownStep();
	// ~Camera Movement

	// ~Zoom
	UFUNCTION()
	void ZoomIn(float Val);
	UFUNCTION()
	void OnPressedZoomInStep();
	UFUNCTION()
	void OnPressedZoomOutStep();
	// ~Zoom

	// ~UI
	UFUNCTION()
	void OnPressedCycleHUDVisibility();
	UFUNCTION()
	FText GetCycleHUDVisibilityValueText() const;
	// ~UI

	// Used to get the active view target from the owning actor
	IViewTargetInterface* GetActiveViewTargetInterface() const;
};
