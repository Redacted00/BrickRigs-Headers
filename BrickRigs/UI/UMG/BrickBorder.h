// Copyright Fluppisoft, 2018

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Components/Border.h"
#include "BrickBorder.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickBorder : public UBorder
{
	GENERATED_BODY()

	// ~Variables
	FBrickUIStyleSafePtr UIStyle;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIBrushStyle BrushStyle;
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIStyleState StyleState;
	UPROPERTY(EditAnywhere, Category = Border)
	EBrickUIPaddingStyle PaddingStyle;
	// ~Properties

public:
	// ~Constructor
	UBrickBorder();

	// ~Super Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void SynchronizeProperties() override;
	virtual void OnSlotAdded(UPanelSlot* InSlot) override;
	// ~Super Interface

	// Sets the current brush style
	UFUNCTION(BlueprintCallable)
	void SetBrushStyle(EBrickUIBrushStyle NewStyle);
	// Sets the current color style
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	// Sets the current style state
	UFUNCTION(BlueprintCallable)
	void SetStyleState(EBrickUIStyleState NewState);
	// Sets the padding style
	UFUNCTION(BlueprintCallable)
	void SetPaddingStyle(EBrickUIPaddingStyle NewStyle);

private:
	// Callback for the style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
	// Resets the padding property binding
	void SynchronizePadding();
	// Callbacks for the slate widget
	const FSlateBrush* GetBorderBrush() const;
	FSlateColor GetBorderColor() const;
	FMargin GetBorderPadding() const;
};
