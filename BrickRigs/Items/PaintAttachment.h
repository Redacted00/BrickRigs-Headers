// Copyright Fluppisoft, 2021

#pragma once

#include "Items/StaticInfo/AttachmentStaticInfo.h"
#include "CoreMinimal.h"
#include "Items/Attachment.h"
#include "PaintAttachment.generated.h"

UCLASS(Abstract)
class BRICKRIGS_API UPaintAttachmentStaticInfo : public UAttachmentStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UPaintAttachmentStaticInfo();
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API APaintAttachment : public AAttachment
{
	GENERATED_BODY()

public:
	// NOTE: Properties are stored here rather than the static info so we don't have to create a new static info class for every color

	// ~Properties
	// Name displayed for the paint and pattern
	UPROPERTY(EditDefaultsOnly, Category = Wearale)
	FText PaintDisplayName;
	// Color to apply
	UPROPERTY(EditDefaultsOnly, Category = PaintAttachment)
	FLinearColor Color = FLinearColor::White;
	// Metallic value
	UPROPERTY(EditDefaultsOnly, Category = PaintAttachment, Meta = (ClampMin = 0, ClampMax = 1))
	float Metallic = 0.f;
	// Roughness value
	UPROPERTY(EditDefaultsOnly, Category = PaintAttachment, Meta = (ClampMin = 0, ClampMax = 1))
	float Roughness = 0.4f;
	// Optional pattern to apply
	UPROPERTY(EditDefaultsOnly, Category = PaintAttachment)
	UTexture2D* Texture = nullptr;
	// Tiling scale of the texture
	UPROPERTY(EditDefaultsOnly, Category = PaintAttachment)
	float Tiling = 1.f;
	// ~Properties

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual FText GetDisplayName() const override;
	virtual bool NeedsThumbnailMID() const override { return true; }
	virtual void UpdateThumbnailMID(UMaterialInstanceDynamic* InMID) const override;
	// ~Super Interface

	// Applies or removes the paint parameters to the given MID
	void ApplyPaint(UMaterialInstanceDynamic* InMID, int32 PaintLayerIndex, bool bApply) const;
};
