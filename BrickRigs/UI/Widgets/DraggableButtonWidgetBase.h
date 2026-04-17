// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"
#include "UI/Widgets/ButtonWidgetBase.h"
#include "DraggableButtonWidgetBase.generated.h"

/**
 * This is the base class for sliders, check boxes and combo boxes
 */
UCLASS(Abstract)
class BRICKRIGS_API UDraggableButtonWidgetBase : public UButtonWidgetBase
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Currently displayed animation progress
	float AnimPosition;
	// Whether the animation should be updated in the next tick
	uint8 bAnimUpdatePending : 1;
	// Whether the mouse is dragging
	uint8 bIsDraggingContent : 1;
	// ~Variables

	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickBorder* FillBorder;
	// ~Widgets

	// ~Properties
	// How fast the animation should transition
	UPROPERTY(EditAnywhere, Category = Slider)
	float TransitionSpeed;
	// ~Properties

public:
	// ~Constructor
	UDraggableButtonWidgetBase(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& InGeometry, float DeltaTime) override;
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnDragged(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void OnPressed(const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bDoubleClick) override;
	virtual void OnReleased(const FGeometry* InGeometry, const FPointerEvent* InMouseEvent, bool bDoubleClick, bool bCancelled) override;
	virtual void OnUpdateContentStyle_Implementation(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InStyleState) override;
	virtual bool ShouldCaptureHighPrecisionMouseMovement() const final override;
	// ~Super Interface

protected:
	// Initializes the anim position after the value has been set for example
	void InitAnimPosition();
	// Update the animation
	void SetAnimPosition(float NewPos);
	// Get the relative X position of a pointer event
	float GetMouseEventRelativePosition(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) const;
	// Get the current animation target
	virtual float GetTargetAnimPosition() const;
	// Updates the dragging flag
	virtual void SetIsDraggingContent(bool bNewDragging);
	// Converts the current mouse position to the target anim value
	virtual float MouseMovementToAnimPosition(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, bool& bOutLockCursorToWidget);
	// Commit the given anim position to a value
	virtual void ApplyAnimPosition(float Pos, EValueChangedEventType EventType);
	// Called when the button was pressed but not dragged
	virtual void CommitClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, bool bDoubleClick);
	// Go back or forth in the selection of options
	virtual float GetNextAnimPosition(int32 Dir) const;
	// Calculates the next anim position from the tick function
	virtual float CalcAnimPosition(float Target, float DeltaTime) const;
	// Implement to actually update the animation progress
	virtual void UpdateAnimPosition();
};
