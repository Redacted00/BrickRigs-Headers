#pragma once

#include "CoreMinimal.h"

struct FBrickEditorSnappingOffset
{
	void SetSnapping(const FVector& InOffset, const FVector& InPivot, const FQuat& InRotation)
	{
		Offset = InOffset;
		Pivot = InPivot;
		Rotation = InRotation;
	}

	void FilterSnapping(bool bSnapLocation, bool bSnapRotation)
	{
		if (!bSnapLocation)
		{
			Offset = FVector::ZeroVector;
			Pivot = FVector::ZeroVector;
		}
		if (!bSnapRotation)
		{
			Rotation = FQuat::Identity;
		}
	}

	void ApplySnappingOffset(FVector& InOutLocation, FQuat& InOutRotation) const
	{
		InOutRotation = Rotation * InOutRotation;
		InOutLocation += Offset;
		const auto PivotOffset = Pivot - InOutLocation;
		const auto RotationOffset = PivotOffset - Rotation.RotateVector(PivotOffset);
		InOutLocation += RotationOffset;
	}

private:
	// ~Variables
	// Editor space location offset to apply
	FVector Offset = FVector::ZeroVector;
	// Editor space location to rotate around
	FVector Pivot = FVector::ZeroVector;
	// Editor space snapping rotation
	FQuat Rotation = FQuat::Identity;
	// ~Variables
};
