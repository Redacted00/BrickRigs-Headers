// Copyright Fluppisoft, 2020

#pragma once

#include "Online/KickedPlayer.h"
#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "Online/SmartUserInfoHandle.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "KickPlayerPopupWidget.generated.h"

class UPropertiesPanelWidget;
class ABrickPlayerState;

UCLASS()
class BRICKRIGS_API UKickPlayerPopupParams : public UPopupParams, public IBrickPropertyInterface
{
	GENERATED_BODY()

	// ~Variables
	FSmartUserInfoHandle UserInfoHandle;
	bool bCanEditPlayerId;
	// ~Variables

public:
	// ~Brick Props
	UPROPERTY(Transient)
	FUniqueNetIdRepl PlayerId;
	UPROPERTY(Transient)
	FString PlayerName;
	UPROPERTY(Transient)
	FString KickReason;
	UPROPERTY(Transient)
	FTimespan KickDuration;
	// ~Brick Props

	// ~Super Interface
	virtual void NativeOnPopupOpened() override;
	virtual void NativeOnPopupClosed(const EPopupResult Result) override;
	// ~Super Interface

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface

	// Copies the properties from an existing kicked player
	void CopyKickProperties(const FKickedPlayer& KickedPlayer);
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UKickPlayerPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

	// ~Variables
	bool bCanConfirmKick;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

public:
	// ~Constructor
	UKickPlayerPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void RevokeKick();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsLoadingKick(bool bNewLoading);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanConfirmKick(bool bNewCanConfirm);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanRevokeKick(bool bCanRevoke);
	// ~Blueprint Interface

private:
	// Calls the Blueprint version if necessary
	void UpdateCanConfirmKickInternal(bool bForceUpdate);
	void OnKickedPlayerListReceived(const FKickedPlayerList& List);
	void UnbindKickedPlayerListDelegate();
};
