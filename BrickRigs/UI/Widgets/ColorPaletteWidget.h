// Copyright Fluppisoft, 2020

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "ColorPaletteWidget.generated.h"

class UBrickImage;
class UMaterialInstanceDynamic;
class UTexture2D;

/**
 * Shows a range of colors that can be clicked
 */
UCLASS(Abstract)
class BRICKRIGS_API UColorPaletteWidget : public UBrickUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnColorChanged, const FLinearColor&, NewColor, EValueChangedEventType, EventType);

	// ~Variables
	// Cached color samples from the color palette texture
	// NOTE: These are cooked and need to be serialized
	UPROPERTY()
	TArray<FColor> ColorPaletteSamples;
	// The MID used for the hovered and pressed effects
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// Whether a color is currently pressed
	uint8 bIsPressed : 1;
	// Whether a color is currently hovered
	uint8 bIsColorHovered : 1;
	// The currently hovered color coordinates
	FIntPoint HoveredColorCoord;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* ColorImage;
	// ~Widgets

	// ~Properties
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditDefaultsOnly, Category = ColorPalette)
	UTexture2D* ColorPaletteTexture;
#endif
	UPROPERTY(EditDefaultsOnly, Category = ColorPalette)
	int32 NumColorsX;
	UPROPERTY(EditDefaultsOnly, Category = ColorPalette)
	int32 NumColorsY;
	// ~Properties

public:
	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnColorChanged OnColorChangedDelegate;
	// ~Delegates

	// ~Constructor
	UColorPaletteWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
#if WITH_EDITOR
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
#endif
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

private:
	// Used to press or unpress a color
	bool SetIsPressed(bool bNewPressed);
	// Updates the hovered color from the given mouse event
	void UpdateHoveredColor(const FPointerEvent* InMouseEvent);
	// Returns the currently hovered color
	TOptional<FColor> GetHoveredColor() const
	{
		const auto ColorIndex = HoveredColorCoord.X + HoveredColorCoord.Y * NumColorsX;
		if (bIsColorHovered && ColorPaletteSamples.IsValidIndex(ColorIndex))
		{
			return ColorPaletteSamples[ColorIndex];
		}

		return TOptional<FColor>();
	}
};
