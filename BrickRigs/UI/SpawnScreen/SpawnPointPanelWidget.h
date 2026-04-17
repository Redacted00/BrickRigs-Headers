// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconPanelWidget.h"
#include "SpawnPointPanelWidget.generated.h"

class UGridPanel;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API USpawnPointPanelWidget : public UHUDIconPanelWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UGridPanel* GridPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Spawn, meta = (ClampMin = 1))
	int32 NumSpawnPointsPerRow;
	// ~Properties

public:
	// ~Constructor
	USpawnPointPanelWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void AddIconWidget(UHUDIconContainerWidget* Widget) override;
	virtual void PostTickHUDIcons(const TArray<FVisibleIcon>& VisibleIcons, bool bIconsAddedOrRemoved) override;
	virtual void OnIconClicked(UHUDIconContainerWidget* Widget, bool bDoubleClick) override;
	// ~Super Interface
};
