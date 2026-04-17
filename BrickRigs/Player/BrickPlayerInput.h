// Copyright Fluppisoft, 2022

#pragma once

#include "UI/Input/InputActionConflict.h"
#include "UI/Input/InputMethod.h"
#include "UI/Input/InputActionAndAxisMapping.h"
#include "UI/Input/InputTableRow.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerInput.h"
#include "BrickPlayerInput.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickPlayerInput : public UPlayerInput
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputMethodChanged, EInputMethod, NewInputMethod);

	// ~Variables
	// Cached list actions in conflict
	FInputActionConflictsMap InputActionConflicts;
	// Whether the conflicts have been updated initially
	uint8 bInitializedInputConflicts : 1;
	// Input stack last processed
	UPROPERTY(Transient)
	TArray<UInputComponent*> LastInputStack;
	// Indices of action mappings that are currently handled
	mutable TSet<int32> HandledActionMappingIndices;
	// ~Variables

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnInputMethodChanged OnInputMethodChangedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual void PostInitProperties() override;
	virtual void ProcessInputStack(const TArray<UInputComponent*>& InputComponentStack, const float DeltaTime, const bool bGamePaused) override;
	virtual bool IsKeyHandledByAction(FKey Key) const override;
	// ~Super Interface

	// Get the last input method used
	EInputMethod GetLastInputMethod() const;
	// Return whether any of the keys of the action binding is pressed in the event
	bool IsInputActionPressed(const FName& ActionName, const FKeyEvent& Event) const
	{
		return IsInputActionPressed(ActionName, Event.GetKey());
	}

	// Version that takes a specific key
	bool IsInputActionPressed(const FName& ActionName, const FKey& Key) const;

	// Adds, removes or replaces an action or axis mapping
	void UpdateInputMapping(const FInputActionAndAxisMapping& PrevMapping, const FInputActionAndAxisMapping& NewMapping);
	// Deletes the user created input mappings and restores to game defaults
	void ResetDefaultInputMappings();

	// The the action or axis mappings associated to a specific action
	void GetActionAndAxisMappings(const FName& ActionName, bool bIsAxis, bool bIncludeKeyboard, bool bIncludeGamepad, bool bIncludePositiveAxis, bool bIncludeNegativeAxis, TArray<FInputActionAndAxisMapping>& OutMappings) const;

	// Finds all conflicts associated with an action, updates the list of conflicts first if it hasn't been initialized
	const FInputActionConflicts* FindInputActionConflicts(const FName& ActionName);
	// Calculate the conflicts to another mapping
	void GenerateInputActionConflicts(const FInputActionAndAxisMapping& InMapping, FInputActionConflicts& OutConflicts) const;

private:
	// Update the conflicted state of all input actions, or optionally only against a single action
	void UpdateInputActionConflicts(const FName& InActionName = NAME_None);
	// Shared function used for GenerateInputActionConflicts and UpdateInputActionConflicts
	void GenerateInputActionConflicts(FInputActionConflictsMap& OutConflicts, const FName& InActionName = NAME_None, const FInputActionAndAxisMapping* InMapping = nullptr) const;

	// Called whenever one or more input mappings have been modified, rebuilds key maps and saves the settings
	void PostEditInputMappings(const FName& ActionName);
	// Internal version that also allows resetting to default mappings
	void ForceRebuildingKeyMapsInternal(bool bRestoreDefaults, bool bIgnoreCustomMappings);

	// Used to add action or axis mappings from a custom mapping with a + or - prefix
	template <typename MappingType, FName MappingType::* NameMember>
	void AddCustomMapping(const MappingType& Mapping, TArray<MappingType>& OutMappings, bool bIsAction, const TMap<FName, const FInputTableRow*>& InputTableRows, TArray<FName>& OutOverriddenActions)
	{
		// Remove the prefix
		auto ActionNameString = (Mapping.*NameMember).ToString();
		const auto bIsAdd = ActionNameString.RemoveFromStart("+");
		const auto bIsRemove = !bIsAdd && ActionNameString.RemoveFromStart("-");
		const auto ActionName = FName(*ActionNameString);

		// Make sure the mapping is associated to a valid input table row
		const auto* InputTableRowPtr = InputTableRows.Find(ActionName);
		const auto* InputTableRow = InputTableRowPtr ? *InputTableRowPtr : nullptr;
		if (InputTableRow && InputTableRow->IsAction() == bIsAction && InputTableRow->CanBeRebound())
		{
			auto ActualMapping = Mapping;
			ActualMapping.*NameMember = ActionName;

			if (bIsAdd || !bIsRemove)
			{
				// This is for mappings without any prefix (legacy), where we expect to override the default mappings
				// Only clear default mappings if this is the first override
				if (!bIsAdd && !OutOverriddenActions.Contains(ActionName))
				{
					OutOverriddenActions.Add(ActionName);
					OutMappings.RemoveAllSwap([ActionName](const auto& Mapping)
						{
							return Mapping.*NameMember == ActionName;
						});
				}

				OutMappings.AddUnique(ActualMapping);
			}
			else
			{
				OutMappings.Remove(ActualMapping);
			}
		}
	}

	// This function is used to detect deltas between the default and actual input mappings and adds them to an out array
	template <typename MappingType, FName MappingType::* NameMember>
	void CompareMappings(const TArray<MappingType>& A, const TArray<MappingType>& B, TArray<MappingType>& OutMappings, bool bAdd) const
	{
		for (const auto& Mapping : A)
		{
			if (!B.Contains(Mapping))
			{
				auto CustomMapping = Mapping;
				CustomMapping.*NameMember = *((bAdd ? "+" : "-") + (CustomMapping.*NameMember).ToString());
				OutMappings.Add(CustomMapping);
			}
		}
	}
};
