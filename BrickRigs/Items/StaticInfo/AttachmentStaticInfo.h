// Copyright Fluppisoft, 2020

#pragma once

#include "Items/Attachment.h"
#include "CoreMinimal.h"
#include "Items/StaticInfo/ItemStaticInfo.h"
#include "AttachmentStaticInfo.generated.h"

UENUM(BlueprintType)
enum class EAttachmentTag : uint8
{
	Sight,
	Barrel,
	Paint
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UAttachmentStaticInfo : public UItemStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UAttachmentStaticInfo();

protected:
	// ~Tags
	// Helper to add an attachment tag
	void AddAttachmentTag(EAttachmentTag InTag);
	// ~Tags
};
