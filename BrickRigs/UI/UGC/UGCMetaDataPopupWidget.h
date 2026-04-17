// Copyright Fluppisoft, 2020

#pragma once

#include "UGC/UGCTypes.h"
#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "UGCMetaDataPopupWidget.generated.h"

class UPropertiesPanelWidget;

enum class EUGCMetaDataPopupMode : uint8
{
	Edit,
	Save,
	UploadNew,
	UploadAs
};

UCLASS()
class BRICKRIGS_API UUGCMetaDataPopupParams : public UPopupParams, public IBrickPropertyInterface
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_FiveParams(FOnMetaDataCommitted, const FUGCFileInfo&, EUGCMetaDataPopupMode, bool, bool, bool);

protected:
	// ~Brick Properties
	UPROPERTY(Transient)
	bool bUpdateTitle = true;
	UPROPERTY(Transient)
	bool bUpdateDescription = true;
	UPROPERTY(Transient)
	bool bUpdateThumbnail = true;
	// ~Brick Properties

public:
	// ~Properties
	EUGCMetaDataPopupMode MetaDataPopupMode;
	UPROPERTY(Transient)
	FUGCFileInfo FileInfo;
	FOnMetaDataCommitted OnMetaDataCommittedDelegate;
	// ~Properties

	// ~Super Interface
	virtual void NativeOnPopupClosed(const EPopupResult Result) override;
	// ~Super Interface

private:
	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~IBrickPropertyInterface
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UUGCMetaDataPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPropertiesPanelWidget* PropertiesPanel;
	// ~Widgets

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	// ~Super Interface
};
