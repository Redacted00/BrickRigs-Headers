// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditor/BrickEditorTypes.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BrickEditorMirrorModeWidget.generated.h"

class UBrickEditorWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickEditorMirrorModeWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The owning widget
	TWeakObjectPtr<UBrickEditorWidget> BrickEditorWidget;
	// The mirror mode represented by this widget
	EBrickEditorMirrorMode MirrorMode;
	// Whether this mode is currently selected
	uint8 bIsSelected : 1;
	// Whether this mode can currently be clicked
	uint8 bCanBeSelected : 1;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Initializes the mirror mode widget
	void InitializeMirrorModeWidget(UBrickEditorWidget* InBrickEditorWidget, EBrickEditorMirrorMode InMirrorMode);
	// ~Getter
	EBrickEditorMirrorMode GetMirrorMode() const
	{
		return MirrorMode;
	}

	// Updates the selection state if needed
	void SetIsSelected(bool bNewSelected);
	// Updates the enabled state
	void SetCanBeSelected(bool bNewCanBeSelected);

protected:
	// To be called when the button has been clicked
	UFUNCTION(BlueprintCallable)
	void OnClicked();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMirrorMode(EBrickEditorMirrorMode InMirrorMode);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsSelected(bool bNewSelected);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanBeSelected(bool bNewCanBeSelected);
	// ~Blueprint Interface
};
