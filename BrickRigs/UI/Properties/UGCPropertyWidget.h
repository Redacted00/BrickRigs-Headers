// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Menu/Popups/PopupParams.h"
#include "UGC/UGCTypes.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "UGCPropertyWidget.generated.h"

class UPopupWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UUGCPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

	// ~Variables
	FPopupHandle PopupHandle_UGCBrowser;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly)
	TSoftClassPtr<UPopupWidget> UGCBrowserPopupClass;
	// ~Properties

public:
	// ~Super Interface
	virtual void InitializeProperty() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void OnClickedButton();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItemsText(int32 NumSelected);
	// ~Blueprint Interface

private:
	// Callback for the UGC browser
	void OnUGCItemsSelected(const TArray<FUGCFileInfo>& NewItems);
};
