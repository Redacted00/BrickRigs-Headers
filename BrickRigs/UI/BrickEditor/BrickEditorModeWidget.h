// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickEditorModeWidget.generated.h"

class ABrickEditor;
class UBrickEditorMode;

/**
 * The base class for custom editor mode widgets
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickEditorModeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The outer brick editor
	TWeakObjectPtr<ABrickEditor> BrickEditor;
	// The associated brick editor mode
	TWeakObjectPtr<UBrickEditorMode> EditorMode;
	// ~Variables

public:
	// Sets the associated mode
	void SetBrickEditorMode(ABrickEditor* InEditor, UBrickEditorMode* InMode);
	// Get the associated mode
	template <class T>
	T* GetBrickEditorMode() const
	{
		return Cast<T>(EditorMode.Get());
	}
};
