// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/BrickThumbnailSubsystem.h"
#include "UI/Misc/TooltipContent.h"
#include "UI/Misc/DisplayInfo.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ObjectPropertyItemInterface.generated.h"

class UTexture2D;
struct FTooltipContent;

UINTERFACE()
class BRICKRIGS_API UObjectPropertyItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Has to be implemented by object types that are selected by object properties
 */
class BRICKRIGS_API IObjectPropertyItemInterface
{
	GENERATED_IINTERFACE_BODY()
	// Implement to return the display name and icon
	virtual FDisplayInfo GetDisplayInfo() const { return FDisplayInfo(); }
	// NOTE: This function has been removed temporarily, since straight texture icons would adapt to the combo box color style (which is not desirable for render icons like maps)
	//virtual TSoftObjectPtr<UTexture2D> GetDisplayIconTexture() const { return nullptr; }
	// Allows items to display a runtime thumbnail
	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const
	{
	}

	// Can be used for custom tooltips
	virtual bool GetTooltipContent(FTooltipContent& OutContent) const { return false; }
	// Can be used for custom icon sizes
	virtual void OverrideDisplayIconSize(int32& InOutSizeX, int32& InOutSizeY) const
	{
	}
};
