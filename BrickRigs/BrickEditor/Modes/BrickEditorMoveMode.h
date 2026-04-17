// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "BrickEditorMode.h"
#include "BrickEditorMoveMode.generated.h"

class UBrickEditorObject;

struct FMoveTransform
{
	FVector Location;
	FQuat Rotation;
};

// Mode used to move or rotate objects
UCLASS()
class BRICKRIGS_API UBrickEditorMoveMode : public UBrickEditorMode
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnGizmoAxisChanged, const FBrickEditorGizmoAxis&);

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
	// Whether precision mode has been used at all during this move
	uint8 bWasInPrecisionMode : 1;
	// Whether the effective location and rotation should be preserved if possible
	uint8 bPreserveEffectiveLocation : 1;
	uint8 bPreserveEffectiveRotation : 1;
	// Whether the spawn location and rotation should be preserved if possible
	uint8 bPreserveSpawnLocation : 1;
	uint8 bPreserveSpawnRotation : 1;
	// The mirror mode used for moving
	EAxis::Type MirrorAxis;
	// Objects being used as the normal and mirrored move roots
	TWeakObjectPtr<UBrickEditorObject> MoveRoot;
	TWeakObjectPtr<UBrickEditorObject> MirroredMoveRoot;
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

	// Returns the desired gizmo transform
	FMoveTransform GetGizmoTransformInternal(bool bVisualTransform) const;
	// Get the current move transform
	FMoveTransform GetMoveTransform() const;
	// Get the current move transform with the accumulated input and grid snapping
	FMoveTransform GetMoveTransformWithAccumulatedInput() const;
	// Returns the currently effective transform, with snapping
	FMoveTransform GetEffectiveMoveTransform() const;
	// Outputs the effective transform of the mirrored move root
	FMoveTransform GetEffectiveMirroredTransform(const FMoveTransform& MoveTransform) const;
	// Outputs the final transform of an object
	FMoveTransform GetEffectiveObjectTransform(const UBrickEditorObject* Object, const FMoveTransform& MoveTransform, const FMoveTransform& MirroredTransform) const;
	// Returns the location and rotation to apply to an object
	TTuple<FVector, FRotator> SanitizeObjectTransform(const UBrickEditorObject* Object, const FMoveTransform& MoveTransform) const;
	// Returns whether the object is on the positive side of the mirror axis
	bool IsObjectOnPositiveMirrorAxis(const UBrickEditorObject* InObject) const;

private:
	// Applies the accumulated input to the actual move transform, commits snapping
	void ApplyAccumulatedInput();
	// Callbacks for the user settings
	void OnSnappingSettingsChanged();
	void OnGizmoWorldSpaceChanged(bool bNewWorldSpace);
};
