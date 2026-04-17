// Copyright Fluppisoft, 2020

#pragma once

#include "Online/KickedPlayer.h"
#include "CoreMinimal.h"
#include "UI/Widgets/PagedListEntryWidget.h"
#include "KickedPlayerWidget.generated.h"

class UBrickTextBlock;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UKickedPlayerWidget : public UPagedListEntryWidget
{
	GENERATED_BODY()

	// Variables
	// The kicked player displayed, has to be a UPROPERTY called Entry so  it can be assigned automatically and reflected
	UPROPERTY(Transient)
	FKickedPlayer Entry;
	// Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* IdTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* KickTimeTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* KickDurationTextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* KickTimeRemainingTextBlock;
	// ~Widgets

public:
	// ~Super Interface
	virtual void InitializeEntry_Implementation() override;
	// ~Super Interface

	// Returns the associated kicked player
	const auto& GetKickedPlayer() const
	{
		return Entry;
	}
};
