// Copyright Fluppisoft, 2018

#pragma once

#include "Settings/BrickUserSettingsTypes.h"
#include "Online/BrickOnlineSessionInfo.h"
#include "Properties/BrickPropertyInterface.h"
#include "UI/Widgets/PagedListWidget.h"
#include "OnlineSessionSettings.h"
#include "CoreMinimal.h"
#include "UI/Menu/MenuPageWidget.h"
#include "ServerBrowserWidget.generated.h"

class UPagedListWidget;

/**
 *
 */
UCLASS(Abstract, Config = Game)
class BRICKRIGS_API UServerBrowserWidget : public UMenuPageWidget, public IBrickPropertyInterface
{
	GENERATED_BODY()

	// ~Variables
	// Cached list of all search results
	TArray<FBrickOnlineSessionInfo> SessionSearchResults;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPagedListWidget* PagedList;
	// ~Widgets

	// ~Brick Properties
	UPROPERTY(Config)
	ESearchSessionType SearchSessionType;
	UPROPERTY(Config)
	EServerSortMethod ServerSortMethod;
	UPROPERTY(Config)
	bool bSearchPrivateServers;
	UPROPERTY(Config)
	bool bSearchServersWithDifferentMods;
	UPROPERTY(Config)
	FString ServerSearchText;
	// ~Brick Properties

public:
	// ~Constructor
	UServerBrowserWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Refreshes the server list
	UFUNCTION(BlueprintCallable)
	void RefreshServers();
	// Join the currently selected server
	UFUNCTION(BlueprintCallable)
	void JoinServer();

protected:
	// ~Blueprint Interface
	// Used to update the refresh button
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanRefresh(bool bCanRefresh);
	// Used to update the join button
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanJoin(bool bCanJoin);
	// ~Blueprint Interface

private:
	// Get the search result associated with the currently selected widget
	const FBrickOnlineSessionInfo* GetSelectedSessionInfo() const;
	// Callbacks for the game instance
	void OnFindSessionsStart();
	void OnFindSessionsResult(const TArray<FOnlineSessionSearchResult>& InSearchResults, bool bComplete);
	// Sorts the list of search results by the desired metric
	void SortServerList();
	// Callbacks for the paged list
	const FPagedListEntry* GetLoadedListEntry(int32 EntryIndex);
	UFUNCTION()
	void OnLoadListPage(int32 NewPage);
	UFUNCTION()
	void OnListEntrySelected(UPagedListEntryWidget* Widget, bool bDoubleClick);

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface

	// Property callbacks
	static void GetSearchSessionTypeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetServerSortMethodItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
