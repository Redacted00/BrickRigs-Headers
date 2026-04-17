// Copyright Fluppisoft, 2020

#pragma once

#include "UI/HUD/HUDWidgetInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpawnScreenWidget.generated.h"

class UMapWidget;
class USpawnPointPanelWidget;
class UHUDIconComponent;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API USpawnScreenWidget : public UUserWidget, public IHUDWidgetInterface
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UMapWidget* MapWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	USpawnPointPanelWidget* SpawnPointPanel;
	// ~Widgets

public:
	// ~Constructor
	USpawnScreenWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

private:
	// Callback for the map
	void OnMapIconSelected(UHUDIconComponent* Comp);
	// Callback for the spawn point panel
	void OnSpawnPointSelected(UHUDIconComponent* Comp);

	// ~IHUDWidgetInterface
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const override;
	// ~IHUDWidgetInterface
};
