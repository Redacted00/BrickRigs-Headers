// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputActionCategoryWidget.generated.h"

class UInputCategory;
class UInputActionListWidget;
enum class EInputActionListMode : uint8;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputActionCategoryWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The parent list widget
	TWeakObjectPtr<UInputActionListWidget> ParentWidget;
	// The cached parent list mode
	EInputActionListMode ActionListMode;
	// The associated input category
	UPROPERTY(Transient)
	UInputCategory* InputCategory;
	// Whether the action is currently selected
	bool bIsSelected;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the parent list widget
	void SetParentWidget(UInputActionListWidget* InWidget);

	// Sets the associated category
	void InitializeCategory(UInputCategory* InCategory, EInputActionListMode InListMode);
	// Updates the selection sttate
	void SetIsSelected(bool bNewSelected);

protected:
	// Callback for the button
	UFUNCTION(BlueprintCallable)
	void OpenCategory();

public:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetWidgetToFocus() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInputActionListMode(EInputActionListMode NewMode);
	UFUNCTION(BlueprintImplementableEvent)
	void InitializeCategory(const FText& InCategoryName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsSelected(bool bNewSelected);
	// ~Blueprint Interface
};
