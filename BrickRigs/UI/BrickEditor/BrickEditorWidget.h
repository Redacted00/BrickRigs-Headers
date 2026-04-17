// Copyright Fluppisoft, 2016

#pragma once

#include "UI/Menu/Popups/UGCTaskPopupWidget.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "BrickEditor/BrickEditorTypes.h"
#include "UI/HUD/HUDWidgetInterface.h"
#include "BrickEditor/BrickEditorObjectFilter.h"
#include "UGC/UGCTypes.h"
#include "Blueprint/UserWidget.h"
#include "BrickEditorWidget.generated.h"

// Widgets
class UBrickTextBoxWidget;
class UBrickGridPanel;
class UPlacableObjectWidget;
class UPropertiesPanelWidget;
class UPopupWidget;
class UBrickEditorModeWidget;
class UBrickEditorMirrorAxisWidget;
class UPagedListHeaderWidget;
// Actors
class ABrickEditor;
// Misc
class UBrickEditorMode;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickEditorWidget : public UUserWidget, public IHUDWidgetInterface
{
	GENERATED_BODY()

	// ~Variables
	FPopupHandle PopupHandle_UGCTask;
	// The cached editor instance
	TWeakObjectPtr<ABrickEditor> BrickEditor;
	// Cached editor mode
	TWeakObjectPtr<UBrickEditorMode> CurrentEditorMode;
	// Current editor mode widget displayed
	UPROPERTY(Transient)
	UBrickEditorModeWidget* CurrentModeWidget;
	UPROPERTY(Transient)
	TArray<UPlacableObjectWidget*> PlacableWidgets;
	// The currently selected filter
	FBrickEditorObjectFilterRef PlacableFilter;
	// All mirror mode widgets that have been created
	UPROPERTY(Transient)
	TArray<UBrickEditorMirrorAxisWidget*> MirrorAxisWidgets;
	TOptional<bool> bIsPropertiesPanelEnabled;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPagedListHeaderWidget* ItemHeaderWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBoxWidget* PlacableSearchTextBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickGridPanel* PlacablesPanel;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSubclassOf<UPlacableObjectWidget> PlacableWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	int32 NumPlacablesPerRow;
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSoftClassPtr<UPopupWidget> UGCTaskPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSubclassOf<UBrickEditorModeWidget> MoveModeWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Editor)
	TSubclassOf<UBrickEditorMirrorAxisWidget> MirrorModeWidgetClass;
	// ~Properties

public:
	// ~Constructor
	UBrickEditorWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	// ~Super Interface

	// Returns the associated brick editor
	ABrickEditor* GetBrickEditor() const
	{
		return BrickEditor.Get();
	}

protected:
	// Resets the text and folder filter
	UFUNCTION(BlueprintCallable)
	void ClearPlacableFilter();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEditorUIScale(float NewScale);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelection(const FText& SelectionName, bool bAnythingSelected);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanPlaceObjects(bool bNewCanPlace);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanClearPlacableFilter(bool bNewCanClear);
	UFUNCTION(BlueprintImplementableEvent)
	void AddModeWidget(UBrickEditorModeWidget* Widget);
	UFUNCTION(BlueprintImplementableEvent)
	void AddMirrorModeWidget(UBrickEditorMirrorAxisWidget* Widget, int32 Index);
	// ~Blueprint Interface

private:
	// ~Editor
	// Updates the aerodynamic objects counts
	void UpdateNumObjectsWithAerodynamics();
	// ~Editor

	// ~Placables
	// Helper function to update the list of placable objects
	void UpdatePlacables();
	// Updates the appearance of the clear placable filter button
	void UpdateCanClearPlacableFilter();

public:
	// Called from the filter widgets to access a new filter
	void OpenPlacableFilter(const FBrickEditorObjectFilterRef& InFilter);
	// Called from the placable widgets
	void PlaceObject(UClass* InStaticInfoClass);
	// ~Placables

	// ~Mirror Axis
	// Called from the mirror mode widgets
	void SelectMirrorAxis(const EAxis::Type NewAxis);

private:
	// Called when the mirror axis changability has changed
	void UpdateCanChangeMirrorAxis();
	// ~Mirror Axis

	// ~Properties
	// Update the properties displayed
	void UpdatePropertiesPanel();
	// ~Properties

	// Callbacks for the editor
	void OnEditorModeChanged(UBrickEditorMode* NewMode);
	void SetEditorMode(UBrickEditorMode* NewMode);
	void OnSelectionChanged();
	void OnFileInfoChanged(const FUGCFileInfo& InFileInfo);
	void SetFileInfo(const FUGCFileInfo& InFileInfo);
	void OnHasUnsavedChangesChanged(bool bNewHasUnsavedChanges);
	void SetHasUnsavedChanges(bool bNewHasUnsavedChanges);
	// Callbacks for the editor interface
	void OnUGCDownloadStarted();
	// Callbacks for the game instance
	void OnUGCUploadStarted();
	// Callbacks for the user settings
	void OnMirrorAxisChanged(const EAxis::Type NewAxis);
	void SetMirrorAxis(const EAxis::Type NewAxis);
	UFUNCTION()
	void OnMeasurementSystemChanged(EMeasurementSystem NewSystem);
	// Callbacks for the placable text box
	UFUNCTION()
	void OnPlacableSearchTextChanged(const FText& NewText, EValueChangedEventType EventType);
};
