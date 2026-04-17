#pragma once

#include "UI/Misc/BrickUIStyleTypes.h"
#include "DisplayInfo.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FDisplayInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBrickUIIconAtlas IconAtlas;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUIIconSlot IconSlot;

	FDisplayInfo(const FText& InName = FText())
		: Name(InName), IconAtlas(EBrickUIIconAtlas::Default)
	{
	}

	bool HasIcon() const
	{
		return IconSlot.IsValid();
	}
};
