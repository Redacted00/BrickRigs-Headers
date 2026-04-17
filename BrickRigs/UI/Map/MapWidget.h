// Copyright Fluppisoft, 2020

#pragma once

#include "Components/HUDIconComponent.h"
#include "MapCrosshairWidget.h"
#include "World/WorldSetupActor.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconCanvasWidget.h"
#include "MapWidget.generated.h"

class UMaterialInstanceDynamic;
class UMenuButtonPanelWidget;
class UBrickTextBlock;

/**
 * Displays a 2D world map, can be used for the mini map, spawn screen etc
 */
UCLASS(Abstract)
class BRICKRIGS_API UMapWidget : public UHUDIconCanvasWidget
{
	GENERATED_BODY()

	// ~Variables
	// Current input rates from the gamepad and keyboard
	FVector2D PanInputRate;
	float ZoomInputRate;
	// Target world size of the viewport
	float TargetViewportSize;
	// Whether the map has been panned or zoomed before
	uint8 bViewportPannedOrZoomed : 1;
	// Whether the viewport is currently being moved to the selected icon
	uint8 bScrollToSelectedIcon : 1;
	// Whether the map render params have been changed on the last frame
	uint8 bChangedMapRenderParams : 1;
	// Time until the map should be rerendered
	float TimeUntilMapRender;
	// The frame in which the map render was requested
	uint32 LastMapRenderFrame;
	// Cached parameters of the last tick
	FMapRenderParams LasTickMapRenderParams;
	// Cached parameters of the last map rendering
	FMapRenderParams MapRenderParams;
	// Cached map render parameters of the previous render
	FMapRenderParams LastMapRenderParams;

	// Current position of the viewport
	FVector2D ViewportCenter;
	// Cached projection parameters updated every tick
	float ViewportRotation;
	// Canvas space origin and size of the viewport
	FVector2D ViewportOrigin;
	FVector2D ViewportSize;
	FVector2D HalfViewportSize;
	FVector2D CanvasSize;
	// The world the viewport has been set up for
	TWeakObjectPtr<UWorld> InitializedViewportForWorld;
	// Cached playable area parameters for the world
	FVector2D PlayableAreaCenter;
	// NOTE: This is in viewport space!
	FVector2D PlayableAreaSize;
	// Cached world bounds size and position
	FVector2D WorldBoundsCenter;
	// NOTE: This is in viewport space!
	FVector2D WorldBoundsSize;

	// HUD icon component used to display the clicked position
	TWeakObjectPtr<UHUDIconComponent> CursorIconComponent;
	// The currently selected world position
	FVector SelectedWorldLocation;

	// Material instance used for the background map
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MapMID;
	// Cached indices for the MID
	int32 UVRegionParamIndex;
	int32 GridParamIndex;
	int32 PlayableAreaParamIndex;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UMenuButtonPanelWidget* ButtonPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* ScaleTextBlock;
	// ~Widgets

	// ~Properties
	// The min size of the viewport when zooming in
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float MinViewportSize;
	// How fast the map moves when panning via axis input
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float PanSpeed;
	// How fast the zoom ratio transitions from rate input
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float ZoomSpeed;
	// The fraction to zoom with one step of the mouse wheel
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float ZoomStep;
	// At what speed the viewport is moved after clicking something
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float ViewportInterpSpeed;
	// Resolution multiplier for the map texture
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float MapResolutionScale;
	// Minimum delay between map renders
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float MapRenderInterval;
	// Initial delay after the map has been moved
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float MapMovedRenderDelay;
	// World size of the grid
	UPROPERTY(EditDefaultsOnly, Category = Map)
	float GridSize;
	// Viewport size at which icons are at their min and max size
	UPROPERTY(EditDefaultsOnly, Category = Map)
	FFloatInterval ViewportSizeIconScaleRange;
	// The icon to display for the selection cursor
	UPROPERTY(EditDefaultsOnly, Category = Map)
	FHUDIconProperties CursorIconProperties;
	// Widget class to create for the crosshair lines
	UPROPERTY(EditDefaultsOnly, Category = Crosshair)
	TSubclassOf<UMapCrosshairWidget> CrosshairWidgetClass;
	// ~Properties

public:
	// ~Constructor
	UMapWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void BeginDestroy() override;
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual FCursorReply NativeOnCursorQuery(const FGeometry& InGeometry, const FPointerEvent& InCursorEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual bool ProjectWorldToCanvas(const FVector& WorldLocation, FVector& OutLocation) const override;
	virtual void UpdateIconPosition(UHUDIconContainerWidget* Widget, const FVector& ScreenLocation) override;
	virtual void OnIconClicked(UHUDIconContainerWidget* Widget, bool bDoubleClick) override;
	virtual void SetSelectedHUDIconComponent(UHUDIconComponent* NewComp) override;
	virtual void SetSelectedHUDIconContainer(UHUDIconContainerWidget* NewContainer) override;

	virtual bool ShouldShowInteractionOptions() const override
	{
		return true;
	}

	// ~Super Interface

	// Scrolls the map view to the selected icon
	void ScrollToSelectedIcon();
	// Reset the panning and zoom parameters to their defaults
	UFUNCTION(BlueprintCallable)
	void ResetPanAndZoom(bool bInitialize = false);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	UMaterialInstanceDynamic* CreateMapMID() const;
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateViewportPannedOrZoomed(bool bValue);

public:
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

	// Sets the currently selected world location
	void SetSelectedWorldLocation(const FVector& NewLocation);
	// Start moving the map
	FReply HandleStartPanning();
	// Stop moving the map
	FReply HandleStopPanning();
	// Resets the map panning
	FReply HandleResetPanning();
	// Pans the map
	void PanViewport(const FVector2D& InPan);
	// Add accumulated pan input
	void AddPanInput(const FVector2D& Input);
	// Add accumulated zoom input
	void AddZoomInput(float Input);
	// Sets the current zoom
	void SetTargetViewportSize(float NewSize, bool bInitialize = false);
	// Updates the viewport panned or zoomed
	void SetViewportPannedOrZoomed(bool bValue);
	// Projects a position on the map to a 2D world position
	bool ProjectCanvasToWorld(const FVector2D& InPosition, FVector2D& OutLocation) const;

private:
	// Get the desired scale for an icon at the given screen position and height
	float GetIconScale(const FVector& ScreenLocation) const;
	// Updates the scale displayed
	void UpdateScaleText();
	// Get the desired UV region for the map MID
	FLinearColor GetMapUVRegion() const;
	// Get the vector parameter for the border texture
	FLinearColor GetGridUVRegion() const;
	// Get the vector parameter for the playable area
	FLinearColor GetPlayableAreaUVRegion() const;
	// Helper function for the UV region getters
	FLinearColor GetUVRegionParam(const FVector2D& Offset, const FVector2D& Size) const;
	// Callback for the cursor icon
	FVector GetCursorIconLocation() const;
	// Callback for the user settings
	UFUNCTION()
	void OnMeasurementSystemChanged(EMeasurementSystem NewSystem);
};
