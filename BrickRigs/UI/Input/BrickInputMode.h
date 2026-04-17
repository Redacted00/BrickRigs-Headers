#pragma once

#include "CoreMinimal.h"

class UWidget;

struct FBrickInputMode
{
	bool bEnableMouse = false;
	bool bBlockInput = false;
	bool bCenterMouse = true;
	// Whether a descendant to focus should be searched
	bool bFocusDescendant = false;
	// Explicit descendant of the focus root to focus
	UWidget* WidgetToFocus = nullptr;
};
