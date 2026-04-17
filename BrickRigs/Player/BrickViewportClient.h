// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Input/HoldKeyTimer.h"
#include "UI/Input/InputMethod.h"
#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "BrickViewportClient.generated.h"

class ABasePlayerController;
class UBaseInputComponent;

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

	// ~Variables
	// Hold key timers
	FHoldKeyTimer HoldKeyTimer_Menu;
	// Indicates the input method (device) over which input has been received the last time
	EInputMethod LastInputMethod;
	// Position of the mouse before it was hidden
	FVector2D LastMousePosition;
	// Last time RMB was pressed in the editor
	float RightClickTime;
	// Whether the current input mode needs a visible cursor
	uint8 bInputModeShowMouseCursor : 1;
	// ~Variables

public:
	// ~Constructor
	UBrickViewportClient();

	// ~Super Interface
	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
	virtual void Activated(FViewport* InViewport, const FWindowActivateEvent& InActivateEvent) override;
	// ~Super Interface

	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Viewport Client"))
	static UBrickViewportClient* Get(const UObject* WorldContextObject);

	// Gets the first local player associated with this viewport
	ULocalPlayer* GetLocalPlayer() const;
	// Gets the first player controller associated with this viewport
	ABasePlayerController* GetPlayerController() const;
	ABrickPlayerController* GetBrickPlayerController() const;

	// Public accessor to the last input method
	EInputMethod GetLastInputMethod() const
	{
		return LastInputMethod;
	}

	// Update the input mode and focused widget, i.e. toggle between UI and game
	void UpdateInputMode();

private:
	// Shows or hides the cursor as needed
	void UpdateCursorVisibility(bool bCenterMouse = false);

	// Should be called when input is changed between keyboard and gamepad
	void SetInputMethod(EInputMethod NewMethod, int32 ControllerId);
};
