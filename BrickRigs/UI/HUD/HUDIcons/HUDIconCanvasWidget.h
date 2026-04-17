// Copyright Fluppisoft, 2020

#pragma once

#include "Blueprint/SlateBlueprintLibrary.h"
#include "UI/Interaction/InteractionWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconPanelWidget.h"
#include "HUDIconCanvasWidget.generated.h"

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDIconCanvasWidget : public UHUDIconPanelWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The interaction widget instance
	UPROPERTY(Transient)
	UInteractionWidget* InteractionWidget;
	// The icon that is currently owning the interaction widget
	TWeakObjectPtr<UHUDIconContainerWidget> InteractionWidgetParent;
	// ~Variables

protected:
	// ~Widgets
	// The main canvas panel icons will be added to
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UCanvasPanel* MainCanvasPanel;
	// ~Widgets

	// ~Properties
	// Scale applied at the minimum and maximum distance
	UPROPERTY(EditDefaultsOnly, Category = HUDIcon)
	FFloatInterval IconScaleRange;
	// ~Properties

public:
	// ~Constructor
	UHUDIconCanvasWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual bool ShouldIconBeVisible(UHUDIconComponent* Comp, const FHUDIconContext& IconContext, FVector& OutScreenLocation) const override;
	virtual void AddIconWidget(UHUDIconContainerWidget* Widget) override;
	// ~Super Interface

protected:
	// Removes the interaction widget from its parent if possible
	void RemoveInteractionWidget();
	// Adds a movable widget to the canvas panel
	UCanvasPanelSlot* AddWidgetToCanvas(UWidget* Widget, const FVector2D& Alignment = FVector2D(0.5f, 0.5f), const FVector2D& Size = FVector2D::ZeroVector);
	// Helper function to place a widget inside the canvas panel
	void UpdateCanvasChildPosition(UWidget* Widget, const FVector& ScreenLocation, float InScale, int32 ZOrder);
	// Converts a screen space position to a canvas space position
	FVector2D AbsoluteSpaceToCanvasSpace(const FVector2D& InPosition) const;
	// Converts a canvas space position to a screen space position
	FVector2D CanvasSpaceToAbsoluteSpace(const FVector2D& InPosition) const;
	// Projects a world location to canvas relative coordinates
	virtual bool ProjectWorldToCanvas(const FVector& Location, FVector& ScreenLocation) const
	{
		checkNoEntry();
		return false;
	}

	// Returns whether the given location would be on the canvas panel
	bool IsPositionOnCanvas(const FVector2D& ScreenPosition) const;
	// Version that takes a 3D vector
	bool IsPositionOnCanvas(const FVector& ScreenPosition) const;
	// Returns the cached geometry of the canvas panel
	const FGeometry& GetCanvasGeometry() const;
};

FORCEINLINE UCanvasPanelSlot* UHUDIconCanvasWidget::AddWidgetToCanvas(UWidget* Widget, const FVector2D& Alignment, const FVector2D& Size)
{
	if (MainCanvasPanel)
	{
		auto PanelSlot = MainCanvasPanel->AddChildToCanvas(Widget);
		if (PanelSlot)
		{
			PanelSlot->SetAutoSize(Size.IsZero());
			PanelSlot->SetSize(Size);
			PanelSlot->SetAlignment(Alignment);
			return PanelSlot;
		}
	}

	return nullptr;
}

FORCEINLINE void UHUDIconCanvasWidget::UpdateCanvasChildPosition(UWidget* Widget, const FVector& ScreenLocation, float InScale, int32 ZOrder)
{
	// Update the distance based scale
	if (InScale != Widget->RenderTransform.Scale.X)
	{
		Widget->SetRenderScale(FVector2D(InScale));
	}

	auto PanelSlot = Cast<UCanvasPanelSlot>(Widget->Slot);
	if (PanelSlot)
	{
		PanelSlot->SetPosition(FVector2D(ScreenLocation));

		// Update the Z order if necessary
		if (PanelSlot->GetZOrder() != ZOrder)
		{
			PanelSlot->SetZOrder(ZOrder);
		}
	}
}

FORCEINLINE FVector2D UHUDIconCanvasWidget::AbsoluteSpaceToCanvasSpace(const FVector2D& InPosition) const
{
	return GetCanvasGeometry().AbsoluteToLocal(InPosition);
}

FORCEINLINE FVector2D UHUDIconCanvasWidget::CanvasSpaceToAbsoluteSpace(const FVector2D& InPosition) const
{
	return GetCanvasGeometry().LocalToAbsolute(InPosition);
}

FORCEINLINE bool UHUDIconCanvasWidget::IsPositionOnCanvas(const FVector2D& ScreenPosition) const
{
	const auto CanvasSize = GetCanvasGeometry().GetLocalSize();
	return ScreenPosition.X > 0.f && ScreenPosition.Y > 0.f && ScreenPosition.X < CanvasSize.X && ScreenPosition.Y < CanvasSize.Y;
}

FORCEINLINE bool UHUDIconCanvasWidget::IsPositionOnCanvas(const FVector& ScreenPosition) const
{
	return IsPositionOnCanvas(FVector2D(ScreenPosition));
}

FORCEINLINE const FGeometry& UHUDIconCanvasWidget::GetCanvasGeometry() const
{
	if (MainCanvasPanel)
	{
		return MainCanvasPanel->GetCachedGeometry();
	}

	return GetCachedGeometry();
}
