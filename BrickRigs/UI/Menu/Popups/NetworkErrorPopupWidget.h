// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "NetworkErrorPopupWidget.generated.h"

class UBrickTextBlock;

UCLASS()
class BRICKRIGS_API UNetworkErrorPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeOnPopupClosed(const EPopupResult Result) override;
	// ~Super Interface
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UNetworkErrorPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* MessageTextBlock;
	// ~Widgets

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual FText GetPopupTitle_Implementation() const override;
	virtual bool ShouldBeVisibleInPopupContainer_Implementation() const override;
	// ~Super Interface
};
