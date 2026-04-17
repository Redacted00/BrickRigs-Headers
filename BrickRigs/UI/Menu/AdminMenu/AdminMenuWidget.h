// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Menu/Popups/PopupParams.h"
#include "Properties/BrickPropertyInterface.h"
#include "Online/KickedPlayer.h"
#include "UI/Widgets/PagedListWidget.h"
#include "CoreMinimal.h"
#include "UI/Menu/MenuPageWidget.h"
#include "AdminMenuWidget.generated.h"

class UPagedListWidget;
class UPagedListEntryWidget;
class UPopupWidget;

/**
 * Menu that allows admins to manage kicks and bans
 */
UCLASS(Abstract)
class BRICKRIGS_API UAdminMenuWidget : public UMenuPageWidget, public IBrickPropertyInterface
{
	GENERATED_BODY()

private:
	// ~Variables
	FPopupHandle PopupHandle_EditKick;
	// Cached player list
	FKickedPlayerList KickedPlayerList;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPagedListWidget* PagedList;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Popups)
	TSoftClassPtr<UPopupWidget> KickPopupClass;
	// ~Properties

	// ~Brick Properties
	UPROPERTY(Transient)
	EKickedPlayerSortMethod KickedPlayerSortMethod;
	UPROPERTY(Transient)
	FString KickedPlayerSearchText;
	// ~Brick Properties

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

protected:
	// Button callbacks
	UFUNCTION(BlueprintCallable)
	void RefreshPlayerList();
	UFUNCTION(BlueprintCallable)
	void EditKick();
	UFUNCTION(BlueprintCallable)
	void ShowPlayerProfile();

	// ~Blueprint Interface
	// Update whether any player is selected
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateButtonsEnabled(bool bNewEnabled);
	// Update whether the player is an admin (and can view the kicks)
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIsAdmin(bool bNewIsAdmin);
	// ~Blueprint Interface

private:
	// Get the currently selected kick info
	const FKickedPlayer* GetSelectedPlayer() const;
	// Callback for the player
	void OnKickedPlayerListReceived(const FKickedPlayerList& NewList);
	// Callbacks for the paged list
	const FPagedListEntry* GetLoadedListEntry(int32 EntryIndex) const;
	UFUNCTION()
	void OnLoadListPage(int32 NewPage);
	UFUNCTION()
	void OnListEntrySelected(UPagedListEntryWidget* Widget, bool bDoubleClick);
	// Popup callbacks
	void OnEditKickPopupClosed(EPopupResult Result);
	void OnRevokeKickPopupClosed(EPopupResult Result);

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface

	// Property callbacks
	static void GetKickedPlayerSortMethodItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
