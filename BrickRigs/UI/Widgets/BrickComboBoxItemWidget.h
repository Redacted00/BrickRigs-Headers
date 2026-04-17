// Copyright Fluppisoft, 2019

#pragma once

#include "UI/Misc/DisplayInfo.h"
#include "CoreMinimal.h"
#include "Misc/BrickThumbnailSubsystem.h"
#include "UI/Misc/TooltipContent.h"
#include "BrickUserWidget.h"
#include "BrickComboBoxItemWidget.generated.h"

class UBrickTextBlock;
class UBrickImage;
class UTexture2D;

USTRUCT(BlueprintType)
struct FBrickComboBoxItemParams
{
	GENERATED_BODY()

	// Whether the icon is going to be displayed as a list item, otherwise the combo box button
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsListItem = false;
	// Display name and optional icon atlas
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDisplayInfo DisplayInfo;
	// Optional texture to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> IconTexture;
	// Optional thumbnail to display
	FBrickThumbnailRequest ThumbnailRequest;
	// Target size of the icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IconSizeX = 128;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IconSizeY = 128;
	// Whether an empty icon should be shown when no icon is given
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowEmptyIcon = false;
	// Whether the item should appear enabled
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEnabled = true;
	// Optional tooltip to display
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTooltipContent TooltipContent;
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickComboBoxItemWidget : public UBrickUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The item id this widget represents
	int32 Item;
	// Cached tooltip info for the item
	FTooltipContent ItemTooltipContent;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* TextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

	// Initializes the item widget
	void NativeInitializeItem(int32 InItem, const FBrickComboBoxItemParams& Params);
	// Return the associated item ID
	int32 GetItem() const
	{
		return Item;
	}

	// Updates the displayed text
	void SetDisplayText(const FText& NewText);

	// ~Blueprint Interface
	// Updates the color of the item
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateItemStyle(EBrickUIColorStyle InColorStyle, EBrickUIStyleState InStyleState);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void InitializeItem(int32 InItem, const FBrickComboBoxItemParams& Params);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateTextVisibility(bool bNewVisible);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIconVisibility(bool bNewVisible);
	UFUNCTION(BlueprintImplementableEvent)
	void SetNoItemBrush();
	// ~Blueprint Interface
};
