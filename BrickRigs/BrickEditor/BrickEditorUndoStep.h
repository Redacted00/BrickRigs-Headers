#pragma once

#include "BrickEditorObjectReference.h"
#include "CoreMinimal.h"

enum class EBrickEditorUndoFlag : uint8
{
	Selection,
	Visibility,
	Objects,
	Max
};

// Defines a single undo/redo step
struct FBrickEditorUndoStep
{
	// IMPORTANT: Selected and hidden objects have to be saved as indices
	// Weak pointers can't be used since they will be invalidated upon and undo, object IDs as well
	// List of selected object indices
	TArray<uint16> SelectedObjects;
	// List of hidden object indices
	TArray<uint16> HiddenObjects;
	// Serialization data of the item state
	TArray<uint8> Data;
	// Flags indicate what states have been changed
	uint32 Flags = 0;

	static void StaticSetFlag(uint32& InFlags, EBrickEditorUndoFlag InFlag)
	{
		InFlags |= 1 << static_cast<int32>(InFlag);
	}

	static bool StaticHasFlag(uint32 InFlags, EBrickEditorUndoFlag InFlag)
	{
		return (InFlags & 1 << static_cast<int32>(InFlag)) != 0;
	}

	void SetFlag(EBrickEditorUndoFlag InFlag)
	{
		StaticSetFlag(Flags, InFlag);
	}

	bool HasFlag(EBrickEditorUndoFlag InFlag) const
	{
		return StaticHasFlag(Flags, InFlag);
	}

	// Joins this step with all previous ones
	void Join(const TArray<FBrickEditorUndoStep>& List, int32 StartIdx, uint32 InFilterFlags)
	{
		for (auto StepIdx = StartIdx; StepIdx >= 0; --StepIdx)
		{
			const auto& OtherStep = List[StepIdx];
			for (auto FlagIdx = 0; FlagIdx < static_cast<int32>(EBrickEditorUndoFlag::Max); ++FlagIdx)
			{
				const auto Flag = static_cast<EBrickEditorUndoFlag>(FlagIdx);
				if (StaticHasFlag(InFilterFlags, Flag) && !HasFlag(Flag) && OtherStep.HasFlag(Flag))
				{
					CopyFlag(OtherStep, Flag);
				}
			}
		}

		Flags |= InFilterFlags;
	}

	// Copies a single flag from the other step
	void CopyFlag(const FBrickEditorUndoStep& Other, EBrickEditorUndoFlag InFlag)
	{
		SetFlag(InFlag);

		switch (InFlag)
		{
		case EBrickEditorUndoFlag::Selection:
			SelectedObjects = Other.SelectedObjects;
			break;
		case EBrickEditorUndoFlag::Visibility:
			HiddenObjects = Other.HiddenObjects;
			break;
		case EBrickEditorUndoFlag::Objects:
			Data = Other.Data;
			break;
		default: ;
		}
	}

	// Returns the approximate size of this step in bytes
	int32 ApproximateSize() const
	{
		return Data.Num() * sizeof(uint8) + SelectedObjects.Num() * sizeof(uint16) + HiddenObjects.Num() * sizeof(uint16) + sizeof(Flags);
	}
};
