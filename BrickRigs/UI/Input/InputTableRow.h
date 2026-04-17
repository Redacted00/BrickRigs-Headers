// Copyright Fluppisoft, 2016

#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "InputCategory.h"
#include "Misc/BrickGameplayTags.h"
#include "GameFramework/PlayerInput.h"
#include "InputActionTriggerType.h"
#include "Engine/DataTable.h"
#include "InputTableRow.generated.h"

UENUM(BlueprintType)
enum class EInputRowType : uint8
{
	PressAction,
	ReleaseAction,
	PressReleaseAction,
	HoldAction,
	Axis,
	HalfAxis
};

USTRUCT(BlueprintType)
struct FDefaultInputMapping
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	uint8 bShift : 1 = false;
	UPROPERTY(EditAnywhere)
	uint8 bCtrl : 1 = false;
	UPROPERTY(EditAnywhere)
	uint8 bAlt : 1 = false;
	UPROPERTY(EditAnywhere)
	uint8 bCmd : 1 = false;
	UPROPERTY(EditAnywhere)
	FKey Key;
	UPROPERTY(EditAnywhere)
	float Scale = 1.f;

	FInputActionKeyMapping ToActionKeyMapping(const FName& ActionName) const
	{
		return FInputActionKeyMapping(ActionName, Key, bShift, bCtrl, bAlt, bCmd);
	}

	FInputAxisKeyMapping ToAxisKeyMapping(const FName& AxisName) const
	{
		return FInputAxisKeyMapping(AxisName, Key, Scale);
	}
};

USTRUCT(BlueprintType)
struct FInputTableRow : public FTableRowBase
{
	GENERATED_BODY()

private:
	// ~Variables
	UPROPERTY(EditAnywhere, Category = Display)
	FDisplayInfo DisplayInfo;
	// Suffix displayed for the primary action
	UPROPERTY(EditAnywhere, Category = Display)
	FText PrimaryActionName;
	// Suffix displayed for the secondary action
	UPROPERTY(EditAnywhere, Category = Display)
	FText SecondaryActionName;
	// Description to show for the primary action
	UPROPERTY(EditAnywhere, Category = Display)
	FText PrimaryActionDescription;
	// Description to show for the secondary action
	UPROPERTY(EditAnywhere, Category = Display)
	FText SecondaryActionDescription;
	// The number of the action, used for naming and for sorting (for example "Slot 1", "Slot 2")
	UPROPERTY(EditAnywhere, Category = Display)
	uint8 ActionIndex = 0;

public:
	UPROPERTY(EditAnywhere, Category = Display)
	EInputRowType Type = EInputRowType::PressAction;
	// Tags determine in which context this action is used and how it can conflict
	UPROPERTY(EditAnywhere, Category = Action)
	FGameplayTagContainer ContextTags;
	// Category to display the action in the input menu
	UPROPERTY(EditAnywhere, Category = Context, meta = (AllowedClasses = "InputCategory"))
	UObject* InputMenuCategory = nullptr;
	// Optional category override for the input help/context menu
	UPROPERTY(EditAnywhere, Category = Context, meta = (AllowedClasses = "InputCategory"))
	UObject* InputContextCategory = nullptr;
	// Whether the action should be shown in the quick access section
	UPROPERTY(EditAnywhere, Category = Context)
	bool bShowInQuickAccess = true;
	// Whether the action should always be shown in the editor top bar
	UPROPERTY(EditAnywhere, Category = Context)
	bool bShowInEditorTopBar = false;
	// Whether this action consumes input, also disables conflicts with non consuming actions
	UPROPERTY(EditAnywhere, Category = Action)
	bool bConsumeInput = false;
	UPROPERTY(EditAnywhere, Category = Action)
	TArray<FDefaultInputMapping> DefaultMappings;
	// ~Variables

	// ~Constructor
	FInputTableRow()
	{
		DisplayInfo.IconAtlas = EBrickUIIconAtlas::MenuButtons;
	}

	bool SortInputTableRow(const FName& ActionName, const FInputTableRow& Other, const FName& OtherActionName, bool bForInputMappings) const
	{
		// Sort actions that can be rebound first
		const auto bCanBeRebound = CanBeRebound();
		const auto bCanOtherBeRebound = Other.CanBeRebound();
		if (bCanBeRebound != bCanOtherBeRebound)
		{
			return bCanBeRebound;
		}

		// Sort axes first
		const auto bIsAction = IsAction();
		const auto bIsOtherAction = Other.IsAction();
		if (bForInputMappings && bIsAction != bIsOtherAction)
		{
			return !bIsAction;
		}

		if (bIsAction)
		{
			// Group actions together that belong together but have a different suffix, like ZoomIn and ZoomOut
			const static TMap<FString, FString> DirectionSuffixes = {{"Right", "Left"}, {"Forward", "Backward"}, {"Up", "Down"}, {"In", "Out"}};

			auto ShortActionName = ActionName.ToString();
			for (const auto& Pair : DirectionSuffixes)
			{
				// Try to remove the positive or negative suffix from our name
				const auto bThisPositive = ShortActionName.RemoveFromEnd(Pair.Key);
				const auto bThisNegative = !bThisPositive && ShortActionName.RemoveFromEnd(Pair.Value);
				if (bThisPositive || bThisNegative)
				{
					// Try to remove the opposite suffix from the other name
					auto OtherShortActionName = OtherActionName.ToString();
					if (OtherShortActionName.RemoveFromEnd(bThisPositive ? Pair.Value : Pair.Key))
					{
						// Check if we have an exact match
						if (ShortActionName == OtherShortActionName)
						{
							// Sort the positive action first
							return bThisPositive;
						}
					}

					break;
				}
			}
		}
		else if (bForInputMappings)
		{
			// Sort half axes before full axes
			const auto bHasNegativeAxis = HasNegativeAxis();
			const auto bOtherHasNegativeAxis = Other.HasNegativeAxis();
			if (bHasNegativeAxis != bOtherHasNegativeAxis)
			{
				return !bHasNegativeAxis;
			}
		}

		// Sort by name
		const auto NameCmp = GetShortDisplayName().CompareToCaseIgnored(Other.GetShortDisplayName());
		if (NameCmp != 0)
		{
			return NameCmp < 0;
		}

		// For actions with the same name we sort by their number, which shouldn't be identical
		return ActionIndex < Other.ActionIndex;
	}

	// Get the general display info
	FDisplayInfo GetDisplayInfo() const
	{
		auto OutDisplayInfo = DisplayInfo;
		OutDisplayInfo.Name = GetDisplayName(false, false);
		return OutDisplayInfo;
	}

	// Get the display info for the primary or secondary action
	FDisplayInfo GetDisplayInfo(bool bSecondaryAction) const
	{
		auto OutDisplayInfo = DisplayInfo;
		OutDisplayInfo.Name = GetDisplayName(!bSecondaryAction, bSecondaryAction);
		return OutDisplayInfo;
	}

	// Version that takes the trigger type for convenience
	FDisplayInfo GetDisplayInfo(EInputActionTriggerType InTriggerType) const
	{
		return GetDisplayInfo(InTriggerType == EInputActionTriggerType::Hold || InTriggerType == EInputActionTriggerType::AxisNegative);
	}

	// Get the display name without the index
	FText GetShortDisplayName() const
	{
		return GetDisplayName(false, false, false);
	}

	// Internal version that can produce all sorts of display names
	FText GetDisplayName(bool bPrimaryAction, bool bSecondaryAction, bool bIncludeIndex = true, bool bIncludeHold = true) const
	{
		auto OutName = DisplayInfo.Name;
		// NOTE: Also display the suffix for rows that don't have a secondary action
		// This is useful for actions the use the PrimaryActionName as a localized suffix, like Zoom Step: In
		if (bPrimaryAction || bSecondaryAction || !HasSecondaryAction())
		{
			// Append the suffix
			const auto SuffixName = bSecondaryAction ? SecondaryActionName : PrimaryActionName;
			if (!SuffixName.IsEmpty())
			{
				OutName = OutName.IsEmpty() ? SuffixName : FText::Format(INVTEXT("{0}: {1}"), OutName, SuffixName);
			}
		}
		else if (OutName.IsEmpty())
		{
			// Use the primary action name if the general action name isn't used
			OutName = PrimaryActionName;
		}

		// Add the action index
		if (bIncludeIndex && ActionIndex > 0)
		{
			OutName = FText::Format(OutName, ActionIndex);
		}

		// Add the hold text
		bIncludeHold &= IsAction() && HasHoldAction();
		OutName = bSecondaryAction ? GetHoldText(OutName, bIncludeHold) : GetTapText(OutName, bIncludeHold);

		return OutName;
	}

	// Returns the description for the primary or secondary action
	FText GetDescription(bool bSecondaryAction, bool bIncludeHold) const
	{
		// Don't include hold for axes or actions without a secondary action
		bIncludeHold &= IsAction() && HasHoldAction();
		return bSecondaryAction && (!SecondaryActionDescription.IsEmpty() || IsAction()) ? GetHoldText(SecondaryActionDescription, bIncludeHold) : GetTapText(PrimaryActionDescription, bIncludeHold);
	}

	static FText GetTapText(const FText& Text, bool bIncludeHold)
	{
		return !Text.IsEmpty() && bIncludeHold ? FText::Format(INVTEXT("{0} ({1})"), Text, NSLOCTEXT("Misc", "TapKey", "Tap")) : Text;
	}

	static FText GetHoldText(const FText& Text, bool bIncludeTap)
	{
		return !Text.IsEmpty() && bIncludeTap ? FText::Format(INVTEXT("{0} ({1})"), Text, NSLOCTEXT("Misc", "HoldKey", "Hold")) : Text;
	}

	bool CanBeRebound() const
	{
		return InputMenuCategory != nullptr;
	}

	bool CanConflictWith(const FName& OwnName, const FName& OtherName, const FInputTableRow& Other) const
	{
		// This is a workaround for the interaction action vs axis options
		if (OwnName == "InteractPri" && OtherName == "InteractAxis")
		{
			return false;
		}

		// Check both ways
		return bConsumeInput == Other.bConsumeInput && (CanConflictWithInternal(Other) || Other.CanConflictWithInternal(*this));
	}

private:
	bool CanConflictWithInternal(const FInputTableRow& Other) const
	{
		return ContextTags.HasAny(Other.ContextTags);
	}

public:
	bool IsAction() const
	{
		return Type < EInputRowType::Axis;
	}

	bool IsAxis() const
	{
		return Type >= EInputRowType::Axis;
	}

	bool HasHoldAction() const
	{
		return Type == EInputRowType::HoldAction;
	}

	bool HasNegativeAxis() const
	{
		return Type == EInputRowType::Axis;
	}

	bool HasSecondaryAction() const
	{
		return HasHoldAction() || HasNegativeAxis();
	}

	bool HasPressedAndReleasedEvent() const
	{
		return Type == EInputRowType::PressReleaseAction || Type == EInputRowType::HoldAction;
	}

	bool HasPressedEvent() const
	{
		return Type == EInputRowType::PressAction || HasPressedAndReleasedEvent();
	}

	bool HasReleasedEvent() const
	{
		return Type == EInputRowType::ReleaseAction || HasPressedAndReleasedEvent();
	}
};
