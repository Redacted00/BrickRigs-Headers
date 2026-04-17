// Copyright Fluppisoft, 2020

#pragma once

#include "UI/UGC/UGCBrowserWidget.h"
#include "UI/UGC/UGCMetaDataPopupWidget.h"
#include "Misc/BrickTimer.h"
#include "CoreMinimal.h"
#include "BrickEditorMode.h"
#include "BrickEditorDefaultMode.generated.h"

struct FBrickEditorObjectFilterRef;

// Default mode used when no other mode is active
UCLASS()
class BRICKRIGS_API UBrickEditorDefaultMode : public UBrickEditorMode
{
	GENERATED_BODY()

	// ~Variables
	FPopupHandle PopupHandle_Editor;
	// Used for periodic auto saving
	FBrickTimer Timer_AutoSave;
	// Whether an auto save should be executed the next time possible
	uint8 bAutoSavePending : 1;
	// The last used mirror mode
	EBrickEditorMirrorMode LastMirrorMode;
	// List of objects that are currently highlighted
	TArray<TWeakObjectPtr<UBrickEditorObject>> HighlightedObjects;
	// The file pending to be loaded or save over
	FUGCFileInfo FilePendingLoadOrSave;
	// The item that is pending to be updated, instead of uploading a new item
	FFluUGCItemIdWrapper ItemIdToUpdate;
	// Whether a backup file has been saved already
	uint8 bSavedBackupFile : 1;
	// Whether the currently opened file should be updated as a new item
	uint8 bIsUploadingAsNewItem : 1;
	// Whether the FileToLoad should be imported rather than opened
	uint8 bImportFileToLoad : 1;
	// Used during ExitEditor to indicate if the player wants to take the current item
	uint8 bIsExitingEditorWithItem : 1;
	// ~Variables

public:
	// ~Constructor
	UBrickEditorDefaultMode();

	// ~Super Interface
	virtual bool EndMode(bool bRevertChanges) override;
	virtual void OnExitEditor() override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual bool CanPlaceObjects() const override { return true; }
	virtual bool ShouldShowEditorVisualization() const override { return true; }
	virtual bool ShouldShowSelectionProperties() const override { return true; }

	virtual bool GetUserSettingsPropertyFilter(FBrickPropertyReflectionFilter& OutFilter) const override
	{
		OutFilter.Tags.AddTagFast(FBrickGameplayTags::Get().Properties.Editor);
		return true;
	}

	virtual bool CanDeleteSelection() const override { return true; }
	virtual void SelectObjects(const TArray<UBrickEditorObject*>& Objects, bool bAddToSelection) override;
	virtual bool ShouldUpdateHoveredObject() const override { return true; }
	// ~Super Interface

	// Custom begin function
	void BeginMode();

	// ~Editor
	// Get the currently opened file info
	const FUGCFileInfo& GetFileInfo() const;
	// Changes the current view mode
	void SetViewMode(EBrickEditorViewMode NewMode);
	// Switches to the next view mode
	void CycleViewMode();
	// ~Editor

	// ~Thumbnails
	// Captures a new thumbnail for the current item
	void CaptureItemThumbnail();

private:
	// Internal version that just captures the thumbnail
	void CaptureItemThumbnailInternal(const bool bIsAutoCapture);
	// ~Thumbnails

public:
	// ~Undo/Redo
	// Undoes the last step
	void Undo();
	// Redoes the last step
	void Redo();
	// ~Undo/Redo

	// ~Loading
	// Opens the editor popup in order to pick a file
	void OpenItemBrowser(bool bImport);
	// Opens the given item
	void LoadItem(const FUGCFileInfo& InFileInfo, bool bImport);

private:
	// Called when the user has confirmed to load the item
	void OnLoadItemUnsavedChangesPopupClosed(EPopupResult Result);
	// ~Loading

public:
	// ~Saving
	// Whether the current item has to be saved as a new file
	bool HasToSaveAsNewItem(bool bIsAutoSave) const;
	// Whether the current item can be saved
	bool CanSaveItem(bool bSaveAsNewItem) const;
	// Saves the current progress, optionally as a new file
	bool SaveItem(bool bSaveAsNewItem, bool bSkipSaveAsPopup = false);
	// Called by the UGC browser
	void OnSaveItemAs(const FUGCFileInfo& InFileInfo);

private:
	// Used for saving and auto saving
	bool SaveItemInternal(bool bIsAutoSave, bool bSaveAsNewItem, const FUGCFileInfo* OverwriteFile = nullptr);
	// Callback for the overwrite existing message
	void OnSaveOverwritePopupClosed(EPopupResult Result);
	// ~Saving

public:
	// ~Auto Saving
	// Save a backup for the current item
	void AutoSave();
	// Get the time remaining until the next auto save
	float GetAutoSaveTimerRemaining() const;

private:
	// Starts or stops the auto save timer as needed
	void UpdateAutoSaveTimer(bool bResetTimer = true);
	// Callback for the editor
	void OnHasUnsavedChangesChanged(bool bNewHasUnsavedChanges);
	// ~Auto Saving

public:
	// ~Uploading
	// Whether the current item has to be uploaded as a new file
	bool HasToUploadAsNewItem() const;
	// Whether the current item can be uploaded or updated
	bool CanUploadItem(bool bUploadAsNew) const;
	// Opens the popup for uploading an item
	void UploadItem(bool bUploadAsNew);
	// Uploads a new item or updates an exiting one
	void OnUploadItemAs(const FUGCFileInfo& FileToUpdate);

private:
	// Whether uploading to the given file is allowed
	bool CanUploadItemAs(const FUGCFileInfo& FileToUpdate) const;
	// Callback for the reupload popup
	void OnReuploadPopupClosed(EPopupResult Result);
	// Callback for the update existing message
	void OnUpdateExistingPopupClosed(EPopupResult Result);
	// Callback for the game instance
	void OnUGCUploadComplete(const EFluUGCUploadResult Result, const FFluUGCItemIdWrapper& NewItemId);
	// ~Uploading

public:
	// ~Meta Data
	// Opens the popup for editing the current item's meta data
	void EditMetaData();

private:
	// Internal version that is used for saving,uploading and editing the meta data
	void EditMetaDataInternal(EUGCMetaDataPopupMode Mode);
	// Callback for the meta data popup
	void OnMetaDataCommitted(const FUGCFileInfo& InFileInfo, EUGCMetaDataPopupMode InMode, bool bUpdateTitle, bool bUpdateDescription, bool bUpdateThumbnail);
	// ~Meta Data

public:
	// ~Exiting
	// Whether the editor can currently be exited
	bool CanExitEditor(bool bWithItem);
	// Allows the player to exit the editor
	void ExitEditor(bool bWithItem);

private:
	// Callback for the unsaved changes popup
	void OnExitEditorUnsavedChangesPopupClosed(EPopupResult Result);
	// ~Exiting

public:
	// ~Placing
	// Returns whether an object of the given class could potentially be placed
	bool CanPlaceObject(UClass* InStaticInfoClass) const;
	// Called to spawn a new object
	bool PlaceObject(UClass* InStaticInfoClass);
	// ~Placing

	// ~Selection
	// Select all objects
	void SelectAll();
	// Unselect all objects
	void UnselectAll();
	// Toggle between all and no objects selected
	void ToggleSelection();
	// Unselect all selected objects and vice versa
	void InvertSelection();
	// Selects all objects directly or indirectly attached to the current selection
	void SelectAttachedObjects(bool bRecursive);
	// Adds all objects with the same property value to the selection
	void SelectObjectsByPropertyValue(const FBrickPropertyInstance& Property, const UBrickEditorObject* SelectedObject);
	// Version that searches the property in the active object
	void SelectObjectsByPropertyValue(const FName& PropertyName);
	// Select all objects of the same class, either checking the static info for object class
	void SelectObjectsByClass(UClass* InClass, bool bIsStaticInfoClass);
	// Version that compares against the class of the active object
	void SelectObjectsByClass(bool bUseStaticInfoClass);
	// Select all objects that are included in the given filter
	void SelectObjectsByFilter(const FBrickEditorObjectFilterRef& Filter);
	// Start moving or rotating the selected objects
	void MoveSelection(const FBrickEditorGizmoAxis& InGizmoAxis);
	// Creates copies of the selected objects, optionally mirroring them
	void DuplicateSelection(bool bMirror = false, bool bDestroyOriginalObjects = false);
	// ~Selection

	// ~Visibility
	// Hide all selected objects
	void HideSelection();
	// Unhide all hidden objects
	void UnhideAll();
	// ~Visibility

	// ~Mirroring
	// Changes the current mirror mode
	void SetMirrorMode(EBrickEditorMirrorMode NewMode);
	// Cycles through all possible mirror modes
	void CycleMirrorMode();
	// Toggles between mirror mode on and off
	void ToggleMirrorMode();
	// ~Mirroring

	// ~Properties
	// Highlights objects that are associated to a focused property
	void HighlightBrickPropertyObjects(const TMap<UBrickEditorObject*, EBrickValidityState>& InObjects);
	// ~Properties

private:
	// Opens the UGC browser for loading, uploading etc.
	void OpenUGCBrowser(EUGCBrowserMode Mode, bool bToggleOpen);
	// Callback for the user settings
	void OnMirrorModeChanged(EBrickEditorMirrorMode NewMode);
};
