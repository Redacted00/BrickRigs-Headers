#pragma once

#include "CoreMinimal.h"
#include "BrickEditorTypes.generated.h"

enum class ESelectObjectsMode : uint8
{
	AddToSelection,
	MoveToStart,
	RemoveFromSelection,
	Destroyed,
	ReplaceSelection,
	ToggleSelection
};

UENUM()
enum class EBrickEditorViewMode : uint8
{
	Default,
	Wireframe,
	Physics,
	InputChannels,
	Max
};

UENUM(BlueprintType)
enum class EBrickEditorMirrorMode : uint8
{
	None,
	X,
	Y,
	Z,
	Max
};

UENUM(BlueprintType)
enum class EMirrorMode : uint8
{
	None,
	YForward,
	ZForward,
	XYSymmetry,
	XYNegSymmetry
};

enum class EBrickEditorGizmoFeature : uint8
{
	// IMPORTANT: Changing the order of these would break the gizmo material!
	None,
	Move,
	MoveAxis,
	RotateAxis
};

struct FBrickEditorGizmoAxis
{
	EAxis::Type Axis;
	EBrickEditorGizmoFeature Feature;

	bool operator==(const FBrickEditorGizmoAxis& Other) const
	{
		return Axis == Other.Axis && Feature == Other.Feature;
	}

	bool operator!=(const FBrickEditorGizmoAxis& Other) const
	{
		return !(*this == Other);
	}

	bool IsValid() const
	{
		return Axis != EAxis::None && Feature != EBrickEditorGizmoFeature::None;
	}

	// Same as operator==, but if the feature isn't set, the axes don't have to match either
	bool IsSameAxis(const FBrickEditorGizmoAxis& Other) const
	{
		return IsValid() ? *this == Other : !Other.IsValid();
	}

	void OverrideWith(const FBrickEditorGizmoAxis& Other)
	{
		Axis = Other.Axis != EAxis::None ? Other.Axis : Axis;
		Feature = Other.Feature;
	}
};
