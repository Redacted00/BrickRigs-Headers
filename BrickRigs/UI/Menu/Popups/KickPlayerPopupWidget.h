// Copyright Fluppisoft, 2020

#pragma once

#include "Online/KickedPlayer.h"
#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "KickPlayerPopupWidget.generated.h"

class UPropertiesPanelWidget;
class ABrickPlayerState;

UCLASS()
class BRICKRIGS_API UKickPlayerPopupParams : public UPopupParams, public IBrickPropertyInterface
{
	GENERATED_BODY()

	// ~Brick Props
	UPROPERTY(Transient)
	FString KickReason;
	UPROPERTY(Transient)
	FTimespan KickDuration;
	// ~Brick Props

public:
	// ~Properties
	// ID of the player to kick
	FUniqueNetIdRepl PlayerId;
	// Name of the player to kick
	FString PlayerName;
	// ~Properties

	// ~Super Interface
	virtual void OnPopupOpened() override;
	virtual void OnPopupClosed(EPopupResult Result) override;
	// ~Super Interface

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
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
	virtual FText GetPopupTitle_Implementation() const override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void RevokeKick();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsLoadingKick(bool bNewLoading);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanRevokeKick(bool bCanRevoke);
	// ~Blueprint Interface

private:
	void OnKickedPlayerListReceived(const FKickedPlayerList& List);
	void UnbindKickedPlayerListDelegate();
};
