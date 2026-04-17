// Copyright Fluppisoft, 2021

#pragma once

#include "InputActionListWidget.h"
#include "CoreMinimal.h"
#include "MenuAnchorWidget.h"
#include "ContextMenuWidget.generated.h"

struct FContextMenuParams
{
	// The text to display as the title
	FText TitleText;
	// The object owning the context menu
	TWeakObjectPtr<UObject> Owner;
	// Optional category to display
	TWeakObjectPtr<UInputCategory> InputCategory;
	// The input component class to use
	TWeakObjectPtr<UClass> InputComponentClass;
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UContextMenuWidget : public UMenuAnchorWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	FContextMenuParams ContextMenuParams;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInputActionListWidget* ActionListWidget;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void InitializeInputComponent() override;
	virtual UWidget* GetWidgetToFocus_Implementation() const override;
	// ~Super Interface

	// Sets up the context menu
	void InitializeContextMenu(const FContextMenuParams& Params);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTitleText(const FText& InText);
	// ~Blueprint Interface

private:
	// Callback for the action list
	UFUNCTION()
	void OnActionTriggered(const FName& ActionName, bool bReleased);
};
