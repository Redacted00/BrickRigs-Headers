// Copyright Fluppisoft, 2019

#pragma once

#include "UGC/UGCTypes.h"
#include "CoreMinimal.h"
#include "UI/Widgets/PagedListEntryWidget.h"
#include "UGCItemWidget.generated.h"

class UBrickImage;
class UBrickTextBlock;

/**
 * This widget represents a single UGC item
 */
UCLASS(Abstract)
class BRICKRIGS_API UUGCItemWidget : public UPagedListEntryWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The file displayed by the widget, has to be a UPROPERTY called Entry so  it can be assigned automatically and reflected
	UPROPERTY(Transient)
	FUGCFileInfo Entry;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* Image;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = UGCItem)
	float ThumbnailRoundedEdgeRadius;
	UPROPERTY(EditDefaultsOnly, Category = UGCItem)
	FVector2D ThumbnailShadowOffset;
	// ~Properties

public:
	// ~Constructor
	UUGCItemWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void InitializeEntry_Implementation() override;
	virtual void UpdateButtonSelected_Implementation(bool bNewSelected) override;
	// ~Super Interface

	// Get the UGC file info
	const FUGCFileInfo& GetFileInfo() const
	{
		return Entry;
	}

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateItemWidget(bool bInIsSelected, bool bInIsLegacyFile, bool bInHasAutoSave, bool bInIsDedicatedAutoSave);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with all parameters
	void UpdateItemWidget();
};
