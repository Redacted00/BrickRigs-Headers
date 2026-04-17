// Copyright Fluppisoft, 2020

#pragma once

#include "UI/BrickUIStyle.h"
#include "HUDIconDomain.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Misc/BrickAssetManager.h"
#include "HUDIconContainerWidget.generated.h"

class UHUDIconPanelWidget;
class UHUDIconWidget;
class UHUDIconComponent;
class UBrickButtonWidget;
class UBrickTextBlock;
class UInteractionWidget;
class UMapCrosshairWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDIconContainerWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	FSmartStreamableHandle StreamableHandle_IconClass;
	// Pointer to our owning panel
	TWeakObjectPtr<UHUDIconPanelWidget> PanelWidget;
	// Cached pointer to the owning icon component
	TWeakObjectPtr<UHUDIconComponent> HUDIconComponent;
	// The icon widget we are hosting
	UPROPERTY(Transient)
	UHUDIconWidget* IconWidget;
	// The crosshair widgets currently used
	UPROPERTY(Transient)
	TArray<UMapCrosshairWidget*> CrosshairWidgets;
	// The type domain this widget belongs to
	EHUDIconDomain IconDomain;
	// Whether the icon is being initialized right now
	uint8 bIsInitializingIcon : 1;
	// Whether this is a spawn point
	uint8 bCanEverSpawn : 1;
	// Whether the player is currently allowed to spawn on this point
	uint8 bCanSpawn : 1;
	// Whether this icon (spawn point) is selected
	uint8 bIsSelected : 1;
	// Whether the name is close enough to be drawn
	uint8 bShowNameOnHUD : 1;
	// The custom color style
	EBrickUIColorStyle ColorStyleOverride;

	// Teams of the actor this widget is displaying
	// NOTE: We need multiple possible teams for vehicles with multiple passengers from different teams
	TSet<FGenericTeamId> IconTeamIds;
	// Cached team ID of the local player
	FGenericTeamId LocalPlayerTeamId;
	// The current team attitude
	ETeamAttitude::Type TeamAttitude;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* SpawnButton;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	// ~Widgets

	// ~Properties
	// Max world distance at which the name will be drawn
	UPROPERTY(EditDefaultsOnly, Category = HUDIcon)
	float MaxNameDrawDist;
	// ~Properties

public:
	// ~Constructor
	UHUDIconContainerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	// ~Super Interface

	// Set our owning panel widget
	void SetOwningPanel(UHUDIconPanelWidget* InWidget, EHUDIconDomain InDomain);
	// Initialize the container
	void InitializeContainer(UHUDIconComponent* Comp, const TSoftClassPtr<UHUDIconWidget>& IconClass, const FGenericTeamId& InLocalPlayerTeamId);
	// Uninitialize the container and return the freed up icon widget
	void UninitializeContainer();
	// Public accessor to our component
	UHUDIconComponent* GetHUDIconComponent() const;
	// Returns the owning actor of the icon component
	AActor* GetHUDIconOwner() const;
	// Called when the widget position is updated in the HUD
	void UpdateDrawDistance(float NewDist, float& InOutIconScale);
	// Update the rotation of the icon
	void UpdateIconRotation(float ViewportRotation);
	// Updates the spawn button
	void UpdateSpawnPoint(bool bCanPlayerRestart);
	// Get the cached geometry of the spawn button
	const FGeometry& GetButtonGeometry() const;
	// Returns the currently used color style
	EBrickUIColorStyle GetColorStyle() const;
	// Adds the crosshair widgets
	void NativeAddCrosshairWidgets(UClass* CrosshairClass);
	// Removes all attached crosshair widgets
	void RemoveCrosshairWidgets();

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	void AddIconWidget(UHUDIconWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddInteractionWidget(UInteractionWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddCrosshairWidgets(const TArray<UMapCrosshairWidget*>& Widgets);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateContainerStyle(bool bNewSelected, bool bNewCanSpawn, EBrickUIColorStyle NewColorStyle);
	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateButtonContentStyle(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InContentStyleState);
	// ~Blueprint Interface

	// Overrides the color style
	void SetIconColorStyle(EBrickUIColorStyle NewColorStyle);
	// Update the displayed name
	void UpdateNameText();

private:
	// Update the name text visibility
	void UpdateNameVisibility();
	// Update the button style
	void UpdateContainerStyle();

public:
	// Update the button selected state
	void SetIsSelected(bool bSelected);
	// Return whether this icon is currently selected
	bool GetIsSelected() const;
	// Return whether this icon represents a spawn point
	bool IsSpawnPoint() const;
	// Whether the player can currently spawn at this point
	bool CanSpawn() const;

	// Allows icons to set the associated teams
	void SetIconTeamIds(const TSet<FGenericTeamId>& InTeamIds);
	// Get the current team id of the local player
	FGenericTeamId GetLocalPlayerTeamId() const;
	// Called from the icon panel when the player changes teams
	void OnLocalPlayerChangedTeam(const FGenericTeamId& InTeamId);

private:
	// Sets the team id of the local player
	void SetLocalPlayerTeamId(const FGenericTeamId& InTeamId);
	// Updates the team attitude of the container
	void UpdateTeamAttitude();

	// Callbacks for the button
	UFUNCTION()
	void OnSpawnButtonClicked();
	UFUNCTION()
	void OnSpawnButtonDoubleClicked();
};
