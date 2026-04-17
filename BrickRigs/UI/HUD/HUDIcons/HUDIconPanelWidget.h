// Copyright Fluppisoft, 2020

#pragma once

#include "HUDIconDomain.h"
#include "HUDIconContainerWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDIconPanelWidget.generated.h"

class UHUDIconComponent;
class UHUDIconWidget;
class ABrickPlayerState;
enum class EInteractionVisibility : uint8;

/**
 * This widget manages HUD icons, should be used as the base class for the playing HUD and map
 */
UCLASS(Abstract)
class BRICKRIGS_API UHUDIconPanelWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHUDIconSelected, UHUDIconComponent*);

	struct FVisibleIcon
	{
		TPair<TWeakObjectPtr<UHUDIconComponent>, TWeakObjectPtr<UHUDIconContainerWidget>>& Pair;
		FVector ScreenLocation;
	};

	// ~Variables
	// Maps from the icon components to the icon widgets used
	TMap<TWeakObjectPtr<UHUDIconComponent>, TWeakObjectPtr<UHUDIconContainerWidget>> HUDIconComponentsMap;
	// Number of icons currently visible
	int32 NumVisibleIcons;
	// The currently selected component
	TWeakObjectPtr<UHUDIconComponent> SelectedHUDIconComponent;
	// The currently selected container widget
	TWeakObjectPtr<UHUDIconContainerWidget> SelectedIconContainerWidget;
	// Cached team ID of the local player
	FGenericTeamId LocalPlayerTeamId;

private:
	// If and how interaction should be drawn
	EInteractionVisibility InteractionVisibility;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = HUDIcons)
	TSubclassOf<UHUDIconContainerWidget> IconContainerWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = HUDIcons)
	TSubclassOf<UInteractionWidget> InteractionWidgetClass;
	UPROPERTY(EditAnywhere, Category = HUDIcons)
	EHUDIconDomain IconDomain;
	// ~Properties

	// ~Delegates
	FOnHUDIconSelected OnHUDIconComponentSelected;
	// ~Delegates

	// ~Constructor
	UHUDIconPanelWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// ~Super Interface

	// Called from the icon containers
	virtual void OnIconClicked(UHUDIconContainerWidget* Widget, bool bDoubleClick);
	// Sets the currently selected component
	virtual void SetSelectedHUDIconComponent(UHUDIconComponent* NewComp);
	// Get the currently selected component
	UHUDIconComponent* GetSelectedHUDIconComponent() const;
	// Whether the selected component can be drawn on this panel
	bool CanShowSelectedHUDIcon() const;
	// Get the owner of the selected component
	AActor* GetSelectedActor() const;

protected:
	// Whether dedicated interaction icons should be shown
	virtual bool ShouldShowInteractionIcons() const
	{
		return false;
	}

	// Whether interaction options should be shown for the selected icon
	virtual bool ShouldShowInteractionOptions() const
	{
		return false;
	}

	// Callback for the local player state
	void OnLocalPlayerChangedTeam(const FGenericTeamId& InTeamId);
	// Sets the team id of the local player
	void SetLocalPlayerTeamId(const FGenericTeamId& InTeamId);
	// Callbacks for the world settings component array
	void OnHUDIconComponentAdded(UHUDIconComponent* Comp);
	void OnHUDIconComponentRemoved(UHUDIconComponent* Comp);
	// Implement to return whether the given component should be shown
	virtual bool ShouldIconBeVisible(UHUDIconComponent* Comp, const FHUDIconContext& IconContext, FVector& OutScreenLocation) const;
	// Reuse an icon widget or create a new one for the given component
	UHUDIconContainerWidget* CreateIconWidget(UHUDIconComponent* Comp);
	// Implement to add a new icon container to the widget
	virtual void AddIconWidget(UHUDIconContainerWidget* Widget);
	// Remove and unregister an icon widget
	virtual void RemoveIconWidget(UHUDIconContainerWidget* Widget);
	// Can be overridden by subclasses to update the widget location in the viewport or map UI
	virtual void UpdateIconPosition(UHUDIconContainerWidget* Widget, const FVector& ScreenLocation)
	{
	}

	// Called in the tick function when the visible icons are known
	virtual void PostTickHUDIcons(const TArray<FVisibleIcon>& VisibleIcons, bool bIconsAddedOrRemoved)
	{
	}

	// Called when the selected widget has changed
	virtual void SetSelectedHUDIconContainer(UHUDIconContainerWidget* NewContainer);
	// Return the icon widget assigned to a component
	UHUDIconContainerWidget* FindIconWidgetForComponent(UHUDIconComponent* Comp) const;
};
