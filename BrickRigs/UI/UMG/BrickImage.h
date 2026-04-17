// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/BrickThumbnailSubsystem.h"
#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/Image.h"
#include "BrickImage.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickImage : public UImage
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// Our own pointer to the slate image
	TSharedPtr<SImage> BrickImage;
	// The brush we actually use for displaying
	FSlateBrush CustomBrush;
	// The thumbnail handle used for brick thumbnails
	FBrickThumbnailHandle ThumbnailHandle;
	// The brush used for the thumbnail
	TSharedPtr<FSlateBrush> ThumbnailBrush;
	// Whether the custom brush should currently be used
	uint8 bUseCustomBrush : 1;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Image)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, Category = Image)
	EBrickUIStyleState StyleState;
	UPROPERTY(EditAnywhere, Category = Image)
	EBrickUIIconAtlas IconAtlas;
	UPROPERTY(EditAnywhere, Category = Image)
	FBrickUIIconSlot IconSlot;
	UPROPERTY(EditAnywhere, Category = Image)
	bool bIsThrobber;
	// ~Properties

	// ~Constructor
	UBrickImage();

	// ~Super Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void CancelImageStreaming() override;
	// ~Super Interface

	// Sets the current color style
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	// Sets the current color state
	UFUNCTION(BlueprintCallable)
	void SetStyleState(EBrickUIStyleState NewState);
	// Set whether this image should appear as a throbber
	UFUNCTION(BlueprintCallable)
	void SetIsThrobber(bool bNewIsThrobber);
	// Sets the icon atlas to display
	UFUNCTION(BlueprintCallable)
	void SetIconAtlas(EBrickUIIconAtlas NewAtlas);
	// Sets the icon atlas slot
	UFUNCTION(BlueprintCallable)
	void SetIconSlot(FBrickUIIconSlot NewSlot);
	// Sets the atlas and slot at the same time
	UFUNCTION(BlueprintCallable)
	void SetIconAtlasAndSlot(EBrickUIIconAtlas NewAtlas, FBrickUIIconSlot NewSlot);
	// Requests a thumbnail from the thumbnail manager and sets it as the brush
	void SetBrushFromThumbnail(FBrickThumbnailRequest Request, bool bMatchSize = false);
	// Releases the current thumbnail
	void ReleaseThumbnail();
	// Whether the image is currently being loaded
	UFUNCTION(BlueprintPure)
	bool IsLoadingImage() const;

private:
	// Callback for the thumbnail manager
	void OnThumbnailLoaded(const TSharedPtr<FSlateBrush>& InBrush);
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Updates the displayed brush
	void UpdateCustomBrush();
	// Callbacks for the slate widget
	const FSlateBrush* GetImage();
	FSlateColor GetImageColor() const;
	TOptional<FSlateRenderTransform> GetImageRenderTransform() const;
};
