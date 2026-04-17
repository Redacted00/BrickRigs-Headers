// Copyright Fluppisoft, 2020

#pragma once

#include "Player/PlayerSpawnRequest.h"
#include "OnlineSubsystemUtils.h"
#include "Properties/BrickPropertyInterface.h"
#include "Game/BrickGameInstance.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "UGCBrowserWidget.generated.h"

class UPagedListWidget;
class UUGCItemWidget;

UENUM(BlueprintType)
enum class EUGCBrowserMode : uint8
{
	None,
	Spawn,
	Open,
	Import,
	Save,
	Upload,
	Property,
	ArrayProperty,
};

UCLASS()
class BRICKRIGS_API UUGCBrowserPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_OneParam(FOnSelectedItemsChanged, const TArray<FUGCFileInfo>&);

	// ~Properties
	EUGCBrowserMode Mode;
	EUGCType UGCType;
	TWeakObjectPtr<AActor> EntryPoint;
	TOptional<TArray<FUGCFileInfo>> SelectedItems;
	bool bOnlyShowSelection = false;
	int32 MaxNumSelectedItems = 0;
	FOnSelectedItemsChanged OnSelectedItemsChangedDelegate;
	// ~Properties
};

/**
 * 
 */
UCLASS(Abstract, Config = Game)
class BRICKRIGS_API UUGCBrowserWidget : public UPopupWidget, public IBrickPropertyInterface
{
	GENERATED_BODY()

	// ~Variables
	FPopupHandle PopupHandle_DeleteItem;
	// Delegate handles for the online subsystem
	FDelegateHandle DelegateHandle_OnUGCQueryDetailsComplete;
	FDelegateHandle DelegateHandle_OnUGCItemSubscribed;
	// Cached params from the popup params for easier access
	EUGCBrowserMode BrowserMode;
	EUGCType UGCType;
	// Cached indicators for the subscription, favorite and like states of the selected item
	uint8 bIsSelectedItemSubscribed : 1;
	uint8 bIsSelectedItemFavorited : 1;
	uint8 bIsSelectedItemVotedUp : 1;
	uint8 bIsSelectedItemVotedDown : 1;
	// Whether the current query has been cancelled
	uint8 bCancelledUGCQuery : 1;
	// Indices if the vehicle that is being spawned should replace the old one
	uint8 bReplaceSpawnedVehicle : 1;
	// Temporarily stores search results after a query
	TArray<FUGCQueryResult> QueryResult;
	// Last query type the user has purposely selected
	UPROPERTY(Config)
	EUGCQueryType LastSelectedUGCQueryType;
	// Last sort method the user has specifically selected
	UPROPERTY(Config)
	EUGCSortMethod LastSelectedUGCSortMethod;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPagedListWidget* PagedList;
	// ~Widgets

	// ~Brick Properties
	UPROPERTY(Transient)
	EUGCQueryType UGCQueryType;
	UPROPERTY(Transient)
	EUGCSortMethod UGCSortMethod;
	UPROPERTY(Transient)
	FString UGCSearchText;
	UPROPERTY(Config)
	bool bShowLegacyItems;
	UPROPERTY(Transient)
	FUGCTags UGCTags;
	UPROPERTY(Transient)
	EPlayerVehicleSpawnInvincibility VehicleSpawnInvincibility;
	// ~Brick Properties

public:
	// ~Constructor
	UUGCBrowserWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

private:
	// Reloads the items on the page
	void RefreshItems();
	// Used to sharing code for the edit, import, recover and create functions
	void OpenItemInternal(bool bSelected, bool bImport, bool bAutoSave);

public:
	// Opens the selected item in the editor or creates a new one if nothing is selected
	UFUNCTION(BlueprintCallable)
	void EditSelectedItem();
	// Opens the last auto save of the selected item
	UFUNCTION(BlueprintCallable)
	void RecoverSelectedAutoSave();
	// Imports the selected item into the editor
	UFUNCTION(BlueprintCallable)
	void ImportSelectedItem();
	// Enters the editor with an empty, new item
	UFUNCTION(BlueprintCallable)
	void CreateNewItem();
	// Used to spawn the selected item in the world
	UFUNCTION(BlueprintCallable)
	void SpawnSelectedItem(bool bReplaceCurrent);
	// Uploads a new item or updates the selected one
	UFUNCTION(BlueprintCallable)
	void UploadItem(bool bUpdateSelected);
	// Overwrites the selected item or saves as a new one
	UFUNCTION(BlueprintCallable)
	void SaveItem(bool bOverwriteSelected);
	// Deletes the currently selected item, opens a warning first
	UFUNCTION(BlueprintCallable)
	void DeleteSelectedItem();
	// Creates a copy of the selected item
	UFUNCTION(BlueprintCallable)
	void DuplicateSelectedItem();

	// Opens the selected item in the steam overlay
	UFUNCTION(BlueprintCallable)
	void ViewSelectedItemInBrowser();
	// Opens the folder containing the selected local item in the file explorer
	UFUNCTION(BlueprintCallable)
	void OpenSelectedItemInExplorer();
	// Adds a down or upvote for the selected item
	UFUNCTION(BlueprintCallable)
	void VoteOnSelectedItem(bool bVoteUp);
	// Adds or removes the item to/from the subscriptions
	UFUNCTION(BlueprintCallable)
	void SubscribeSelectedItem(bool bSubscribe);
	// Adds or removes the item to/from the favorites list
	UFUNCTION(BlueprintCallable)
	void FavoriteSelectedItem(bool bFavorite);
	// Used to switch the vehicle spawn mode
	UFUNCTION(BlueprintCallable)
	void CycleVehicleSpawnInvincibility();

	// Adds the selected item to the property
	UFUNCTION(BlueprintCallable)
	void AddOrRemoveSelectedItem(bool bAdd);
	// Removes all items from the property
	UFUNCTION(BlueprintCallable)
	void ClearItems();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void CreateCancelButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateSpawnButton(bool bCanReplaceCurrent, bool bCanSpawn, const FText& SpawnFailureText);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateSpawnInvincibilityButton(const FText& CurrentInvincibilityText);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateEditButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateRecoverAutoSaveButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateImportButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateNewItemButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateVoteButtons(bool bVotedUp, bool bVotedDown);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateSubscribeButton(bool bIsSubscribed);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFavoriteButton(bool bIsFavorited);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateViewInBrowserButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateOpenInExplorerButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateDeleteButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateDuplicateButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateUpdateExistingButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateUploadNewButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateSaveNewButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateSaveOverwriteButton();
	UFUNCTION(BlueprintImplementableEvent)
	void CreateAddOrRemoveItemButton(bool bIsArrayProperty, bool bIsSelected);
	UFUNCTION(BlueprintImplementableEvent)
	void CreateClearItemsButton(bool bIsArrayProperty);
	// ~Blueprint Interface

private:
	// Updates the displayed buttons according to the current context
	void UpdateButtons();
	// Popup callbacks
	void OnSpawnItemUnsavedChangesPopupClosed(EPopupResult Result);
	void OnDeleteItemPopupClosed(EPopupResult Result);

	// Returns the selected item widget
	UUGCItemWidget* GetSelectedItemWidget() const;
	// Returns the file info of the selected item
	const FUGCFileInfo* GetSelectedFileInfo() const;
	// Get the UGC interface associated with the given item
	IOnlineUGCPtr GetFileInfoUGCInterface(const FUGCFileInfo& FileInfo) const;
	// Callback for the game instance
	void OnUGCQueryComplete(const TArray<FUGCQueryResult>& Results, uint32 TotalNumResults, uint32 NumPages);
	// Callbacks for the online subsystem
	void OnUGCQueryDetailsComplete(const TArray<FOnlineUGCQueryResult>& Results, bool bSuccess);
	void OnUGCItemSubscribed(const TSharedRef<FOnlineUGCItemId>& ItemId, bool bSubscribed);
	void OnGetOrSetItemVoteComplete(const TSharedRef<FOnlineUGCItemId>& ItemId, bool bSuccess, bool bVotedUp, bool bVotedDown);
	void OnSubscribeItemComplete(const TSharedRef<FOnlineUGCItemId>& ItemId, bool bSuccess, bool bSubscribed);
	// Callbacks for the paged list
	const FPagedListEntry* GetLoadedListEntry(int32 EntryIndex);
	UFUNCTION()
	void OnLoadListPage(int32 NewPage);
	UFUNCTION()
	void OnListEntrySelected(UPagedListEntryWidget* Widget, bool bDoubleClick);
	// Makes sure a valid sort method is selected
	void VerifyUGCSortMethod();

	// Creates the vehicle spawn request spawn request
	FPlayerSpawnRequest MakeVehicleSpawnRequest(bool bReplace) const;

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface

	// Property callbacks
	static void GetUGCQueryTypeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetUGCSortMethodItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetVehicleSpawnInvincibilityItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static bool StaticIsUGCQueryTypeSupported(const FBrickPropertyContainer& Container, EUGCQueryType InQueryType);
	static bool StaticIsUGCSortMethodSupported(const FBrickPropertyContainer& Container, EUGCSortMethod InSortMethod);
	// Member function versions for easier syntax
	void GetUGCQueryTypeItems(TArray<FEnumPropertyItem>& OutItems) const;
	bool IsUGCQueryTypeSupported(EUGCQueryType InQueryType) const;
	bool IsUGCSortMethodSupported(EUGCSortMethod InSortMethod) const;
};
