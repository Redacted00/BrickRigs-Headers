// Copyright Fluppisoft, 2016

#pragma once

#include "Misc/BrickAssetManager.h"
#include "Misc/SmartDelegateHandle.h"
#include "Components/FirearmComponent.h"
#include "Vehicle/VehicleInputChannel.h"
#include "UI/Misc/BrickUITimer.h"
#include "HurtMarkerWidget.h"
#include "UI/HUD/HUDIcons/HUDIconCanvasWidget.h"
#include "PlayerWidget.generated.h"

// Actors
class ABaseCharacter;
class ABrickVehicle;
class AInventoryItem;
// Widgets
class UCurrentItemWidget;
class UHealthBarWidget;
class UCrosshairWidget;
class URestrictedAreaWidget;
class UDashboardWidget;
class UCameraBrickWidget;
// Bricks
class UBrick;
class USeatBrick;
class UCameraBrick;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPlayerWidget : public UHUDIconCanvasWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	FSmartDelegatePool DelegatePool_PC;
	// Handles used for async loading
	FSmartStreamableHandle StreamableHandle_DashboardWidgetClass;
	FSmartStreamableHandle StreamableHandle_CameraBrickWidgetClass;
	// Used to delay the fading out the current item widget
	FBrickUITimer Timer_FadeOutCurrentItem;

	// The viewed character, needed to unbind delegates later
	UPROPERTY(Transient)
	ABaseCharacter* Character;
	// The viewed vehicle
	UPROPERTY(Transient)
	ABrickVehicle* Vehicle;
	// The currently equipped item
	TWeakObjectPtr<AInventoryItem> CurrentItem;
	// The seat the character is in
	TWeakObjectPtr<USeatBrick> VehicleSeat;
	// The current view target
	TWeakObjectPtr<UObject> ViewTargetSubobject;

	// The current item widget instance
	UPROPERTY(Transient)
	UCurrentItemWidget* CurrentItemWidget;
	// The health bar instance
	UPROPERTY(Transient)
	UHealthBarWidget* HealthBarWidget;
	// The crosshair widget instance
	UPROPERTY(Transient)
	UCrosshairWidget* CrosshairWidget;
	// List of all hurt markers, active and inactive
	UPROPERTY(Transient)
	TArray<UHurtMarkerWidget*> HurtMarkers;
	// The restricted area widget instance
	UPROPERTY(Transient)
	URestrictedAreaWidget* RestrictedAreaWidget;
	// The dashboard widget instance
	UPROPERTY(Transient)
	UDashboardWidget* DashboardWidget;
	// Widget displayed for the camera HUD
	UPROPERTY(Transient)
	UCameraBrickWidget* CameraBrickWidget;

	// Cached projection parameters updated before every tick
	FSceneViewProjectionData ViewProjectionData;
	FMatrix ViewProjectionMatrix;
	FVector2D ScreenToCanvasPosition;
	FVector2D ScreenToCanvasSize;

	// The cached interaction visibility
	EInteractionVisibility InteractionVisibility;
	// Whether the player is in the playing state
	uint8 bIsInPlayingState : 1;
	// Whether the character is currently in a restricted area
	uint8 bIsInRestrictedArea : 1;
	// Flags indicate if the widgets are currently added to the screen
	uint8 bIsCurrentItemVisible : 1;
	uint8 bIsHealthBarVisible : 1;
	uint8 bIsCrosshairVisible : 1;
	// ~Variables

protected:
	// ~Properties
	// Minimum and maximum distance to use for scaling
	UPROPERTY(EditDefaultsOnly, Category = Player)
	FFloatInterval IconScaleDistanceRange;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UCurrentItemWidget> CurrentItemWidgetClass;
	// Time until the current item widget is automatically faded out
	UPROPERTY(EditAnywhere, Category = CurrentItem)
	float CurrentItemFadeOutDelay;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UHealthBarWidget> HealthBarWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UCrosshairWidget> CrosshairWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<UHurtMarkerWidget> HurtMarkerClass;
	// Maximum number of hurt markers to display at once
	UPROPERTY(EditDefaultsOnly, Category = Player)
	int32 MaxNumHurtMarkers;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSubclassOf<URestrictedAreaWidget> RestrictedAreaWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Player)
	TSoftClassPtr<UDashboardWidget> DashboardWidgetClass;
	// ~Properties

public:
	// ~Constructor
	UPlayerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual bool ProjectWorldToCanvas(const FVector& Location, FVector& ScreenLocation) const override;
	virtual bool ShouldIconBeVisible(UHUDIconComponent* Comp, const FHUDIconContext& IconContext, FVector& OutScreenLocation) const override;
	virtual void UpdateIconPosition(UHUDIconContainerWidget* Widget, const FVector& ScreenLocation) override;

	virtual bool ShouldShowInteractionIcons() const override
	{
		return InteractionVisibility != EInteractionVisibility::Hidden;
	}

	virtual bool ShouldShowInteractionOptions() const override
	{
		return InteractionVisibility == EInteractionVisibility::Visible;
	}

	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddCurrentItemWidget(UCurrentItemWidget* InWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddHealthBarWidget(UHealthBarWidget* InWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddCrosshairWidget(UCrosshairWidget* InWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddRestrictedAreaWidget(URestrictedAreaWidget* InWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddDashboardWidget(UDashboardWidget* InWidget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddCameraBrickWidget(UCameraBrickWidget* InWidget);
	// ~Blueprint Interface

private:
	// Whether the current item widget should currently be visible
	bool ShouldShowCurrentItem() const;
	// Updates the equipped item/bricks
	void UpdateCurrentItem(bool bIsDestructing = false);
	// Timer callback
	void OnFadeOutCurrentItem();
	// Get the input channel that is used by the current fire action
	FVehicleInputChannel GetFireActionInputChannel() const;

	// Whether the health bar should currently be visible
	bool ShouldShowHealthBar() const;
	// Shows or hides the health bar
	void UpdateHealthBarVisibility(bool bIsDestructing = false);

	// Whether the crosshair should be visible
	bool ShouldShowCrosshair() const;
	// Shows or hides the crosshair
	void UpdateCorsshairVisibility(bool bIsDestructing = false);

	// Removes all hurt markers from the screen
	void RemoveHurtMarkers();
	// Whether hurt markers should be displayed with the given HUD visibility
	bool ShouldShowHurtMarkers() const;
	// Callback for the hurt markers
	void OnHurtMarkerFadedOut(UHurtMarkerWidget* HurtMarker);

	// Whether the restricted area widget should currently be visible
	bool ShouldShowRestrictedAreaWidget() const;
	// Shows or hides the restricted area widget if needed
	void UpdateRestrictedAreaWidgetVisibility(bool bIsDestructing = false);

public:
	// Called from the restricted area widget when the fade out animation has finished
	void OnRestrictedAreaWidgetFadedOut();

private:
	// Whether the dashboard should currently be visible
	bool ShouldShowDashboard() const;
	// Shows or hides the dashboard if needed
	void UpdateDashboardVisibility();
	// Sets the vehicle and adds or removes the dashboard if needed
	void UpdateDashboardVisibilityInternal();

	// Whether the camera brick widget should currently be shown
	bool ShouldShowCameraBrickWidget() const;
	// Loads, adds or removes the camera brick widget as needed
	void UpdateCameraBrickWidget();
	// Removes the camera HUD widget and cancels loading
	void RemoveCameraBrickWidget();

	// Callbacks for the player
	void OnStateNameChanged(const FName& NewState);
	void SetStateName(const FName& NewState);
	void OnCharacterChanged(ABaseCharacter* NewCharacter);
	void SetCharacter(ABaseCharacter* NewCharacter);
	void OnVehicleChanged(ABrickVehicle* NewVehicle);
	void SetVehicle(ABrickVehicle* NewVehicle);
	void OnViewTargetSubobjectChanged(UObject* NewViewTargetSubobject);
	void SetViewTargetSubobject(UObject* NewViewTargetSubobject);
	// Callbacks for the character
	void OnCurrentItemChanged(AInventoryItem* NewItem);
	void SetCurrentItem(AInventoryItem* NewItem);
	void OnVehicleSeatChanged(USeatBrick* NewSeat);
	void SetVehicleSeat(USeatBrick* NewSeat);
	void OnReceivedDamage(const FCharacterDamageInfo& DamageInfo);
	void OnRestrictedAreaChanged(bool bIsInArea);
	void SetIsInRestrictedArea(bool bIsInArea);
	// Callbacks for the seat
	void OnFireActionModeChanged(int32 NewMode);
	// Callbacks for the current item
	void OnFirearmStateChanged(const FFirearmState& NewState);
	// Callbacks for the user settings
	UFUNCTION()
	void OnHUDVisibilityChanged(EHUDVisibility NewVisibility);
};
