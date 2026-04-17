// Copyright Fluppisoft, 2018

#pragma once

#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "UI/Widgets/BrickUserWidget.h"
#include "MenuButtonWidget.generated.h"

DECLARE_DYNAMIC_DELEGATE(FOnClickedMenuButton);

class UBrickButtonWidget;
class UBrickTextBlock;
class UBrickImage;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuButtonWidget : public UBrickUserWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* TextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	// ~Widgets

public:
	// ~Delegates
	FOnClickedMenuButton OnClickedDelegate;
	// ~Delegates

	// ~Constructor
	UMenuButtonWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Initializes the button
	void InitializeButton(const FText& InDisplayText, int32 InIconIndex, int32 InChildIndex, const FOnClickedMenuButton& InDelegate);
	// Get the sub widget that should receive user focus by default
	UWidget* GetWidgetToFocus() const;
	// Changes the displayed text
	UFUNCTION(BlueprintCallable)
	void SetDisplayText(FText InText);
	// Changes the displayed icon
	UFUNCTION(BlueprintCallable)
	void SetDisplayIcon(int32 InIconIndex);
	// Changes the current color style
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle InColorStyle);

protected:
	// Callback when the button has been clicked
	UFUNCTION(BlueprintCallable)
	void OnClicked();

	// ~Blueprint Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateDisplayText(const FText& InText);
	// ~Blueprint Interface
};
