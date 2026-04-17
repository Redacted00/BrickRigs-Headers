// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BrickEditorMode.h"
#include "BrickEditorMoveMode.generated.h"

class UBrickEditorObject;

// Mode used to move or rotate objects
UCLASS()
class BRICKRIGS_API UBrickEditorMoveMode : public UBrickEditorMode
{
	GENERATED_BODY()

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGizmoAxisChanged, const FBrickEditorGizmoAxis&);

	struct FMoveTransformHelper
	{
		FVector PreMoveLocation;
		FRotator PreMoveRotation;
		FVector DeltaLocation;
		FQuat DeltaRotation;
		FVector PivotPoint;

		// ~Constructor
		FMoveTransformHelper() = default;

		// ~Constructor
		FMoveTransformHelper(UBrickEditorObject* MoveRoot, const FVector& InNewLocation, const FRotator& InNewRotation);

		// Get the new transform after applying the move delta
		void GetObjectTransform(UBrickEditorObject* Object, bool bRevertTransform, FVector& OutLocation, FRotator& OutRotation) const;
		// Mirrors this transform along a mirror axis
		void Mirror(UBrickEditorObject* MirroredMoveRoot, EBrickEditorMirrorMode MirrorMode);
	};

	// ~Variables
	// The gizmo axis being used for moving
	FBrickEditorGizmoAxis GizmoAxis;
	// Whether the gizmo is used in world space
	uint8 bGizmoWorldSpace : 1;
	// Whether the moved objects have just been created with those move
	uint8 bJustPlacedObjects : 1;
	// Set to true upon the initial tick
	uint8 bInitialTick : 1;
	// Whether transform should be updated next tick
	uint8 bUpdateTransform : 1;
	// Whether any object that is being moved is currently in an invalid state
	uint8 bAnyObjectInvalid : 1;
	// Whether the move perpendicular key is being held down
	uint8 bMouseMovePerpendicular : 1;
	// Whether precision mode has been used at during this move
	uint8 bWasInPrecisionMode : 1;
	// Set to true while a location or rotation slider is being moved
	uint8 bPrecisionMovePendingCommit : 1;
	// The mirror mode used for moving
	EBrickEditorMirrorMode MirrorMode;
	// Index of the object that is being used as the mirrored move root
	int32 MirroredMoveRootIndex;
	// Accumulated input vectors for moving/rotating
	FVector MoveInput;
	FVector2D MouseMoveInput;
	// The accumulated location and rotation inputs applied during the current move operation before snapping is applied
	FVector AccumulatedLocationInput;
	FVector AccumulatedRotationInput;
	// The current base transform of the move, before accumulated input and snapping is applied
	FVector CurrentMoveLocation;
	FRotator CurrentMoveRotation;
	// The current snapping offset applied to the move
	FBrickEditorSnappingOffset CurrentMoveSnappingOffset;
	// ~Variables

protected:
	// ~Brick Properties
	UPROPERTY(Transient)
	FVector EffectiveMoveLocation;
	UPROPERTY(Transient)
	FRotator EffectiveMoveRotation;
	// ~Brick Properties

public:
	// ~Delegates
	FOnGizmoAxisChanged OnGizmoAxisChangedDelegate;
	// ~Delegates

	// ~Constructor
	UBrickEditorMoveMode();

	// ~Super Interface
	virtual bool CanBeginMode() const override;
	virtual bool EndMode(bool bRevertChanges) override;
	virtual void TickMode(float DeltaTime) override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual bool ShouldEnablePropertiesPanel() const override { return IsInPrecisionMoveMode(); }

	virtual bool GetUserSettingsPropertyFilter(FBrickPropertyReflectionFilter& OutFilter) const override
	{
		OutFilter.Tags.AddTagFast(FBrickGameplayTags::Get().Properties.Editor.Move);
		return true;
	}

	virtual bool ShouldCaptureMouse() const override;
	virtual bool CanDeleteSelection() const override { return true; }
	virtual void DeleteSelection() override;
	virtual TOptional<FTransform> GetGizmoTransform() const override;
	virtual void PressObjectAtScreenPosition(const FVector2D& ScreenPosition, bool bAddToSelection) override;
	virtual void ReleaseObjectAtScreenPosition(const FVector2D& ScreenPosition, bool bAddToSelection) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~Super Interface

	// Custom begin function with params
	void BeginMode(const FBrickEditorGizmoAxis& InGizmoAxis, bool bInJustPlacedObjects = false);
	// Sets the axis along which the objects are being moved or rotated
	void SetGizmoAxis(const FBrickEditorGizmoAxis& InGizmoAxis, bool bInitial = false);
	// Returns the current gizmo axis
	const auto& GetGizmoAxis() const
	{
		return GizmoAxis;
	}

	// Whether the precision move mode is currently enabled
	bool IsInPrecisionMoveMode() const
	{
		return !GizmoAxis.IsValid();
	}

	// Adds accumulated movement input
	void AddMoveInput(EAxis::Type Direction, float Val);
	// Adds accumulated mouse movement input
	void AddMouseMoveInput(EAxis::Type Direction, float Val);
	// Enables or disables the perpendicular movement mode
	void SetMouseMovePerpendicular(bool bNew)
	{
		bMouseMovePerpendicular = bNew;
	}

private:
	// Returns the desired gizmo transform
	void GetGizmoTransformInternal(FVector& OutLocation, FRotator& OutRotation, bool bVisualTransform) const;
	// Get the current move transform
	void GetMoveTransform(FVector& OutLocation, FRotator& OutRotation) const;
	// Get the current move transform with the accumulated input and grid snapping
	void GetMoveTransformWithAccumulatedInput(FVector& OutLocation, FRotator& OutRotation) const;
	// Returns the currently effective transform, with snapping
	void GetEffectiveMoveTransform(FVector& OutLocation, FRotator& OutRotation) const;
	// Get the effective move transform, including snapping
	void GetEffectiveMoveTransform(UBrickEditorObject* MoveRoot, UBrickEditorObject* MirroredMoveRoot, FMoveTransformHelper& OutTransform, FMoveTransformHelper& OutMirroredTransform) const;
	// Returns whether the object is on the positive side of the mirror axis
	bool IsObjectOnPositiveMirrorAxis(UBrickEditorObject* InObject) const;
	// Applies the accumulated input to the actual move transform, commits snapping
	void ApplyAccumulatedInput();
	// Callbacks for the user settings
	void OnSnappingSettingsChanged();
	void OnGizmoWorldSpaceChanged(bool bNewWorldSpace);

	// Property callbacks
	static void GetMoveLocationValueRange(const FBrickPropertyContainer& Container, FVector& OutMin, FVector& OutMax);
};
