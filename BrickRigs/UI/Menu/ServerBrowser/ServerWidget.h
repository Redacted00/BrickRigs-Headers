// Copyright Fluppisoft, 2018

#pragma once

#include "Online/BrickOnlineSessionInfo.h"
#include "CoreMinimal.h"
#include "UI/Widgets/PagedListEntryWidget.h"
#include "ServerWidget.generated.h"

class UBrickTextBlock;
class UPingIndicatorWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UServerWidget : public UPagedListEntryWidget
{
	GENERATED_BODY()

	// ~Variables
	// The session displayed, has to be a UPROPERTY called Entry so  it can be assigned automatically and reflected
	UPROPERTY(Transient)
	FBrickOnlineSessionInfo Entry;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* MapTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* GameModeTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* PlayerCountTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPingIndicatorWidget* PingIndicator;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UPingIndicatorWidget* FrameRateIndicator;
	// ~Widgets

public:
	// ~Super Interface
	virtual void InitializeEntry_Implementation() override;
	// ~Super Interface

	// Returns the associated session info
	const FBrickOnlineSessionInfo& GetSessionInfo() const
	{
		return Entry;
	}

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateServer(const FBrickOnlineSessionInfo& SessionInfo);
	// ~Blueprint Interface
};
