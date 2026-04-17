#pragma once

#include "UI/Input/BrickInputMode.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDWidgetInterface.generated.h"

class UWidget;

UINTERFACE()
class BRICKRIGS_API UHUDWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BRICKRIGS_API IHUDWidgetInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// Get the desired input mode
	virtual bool GetInputMode(FBrickInputMode& OutInputMode) const { return false; }
	// Optionally adds the game overlay as a subwidget, return true if it has been added
	UFUNCTION(BlueprintImplementableEvent)
	bool AddGameOverlayWidget(UGameOverlayWidget* Widget);
};
