// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Input/InputTableRow.h"
#include "CoreMinimal.h"
#include "BrickUserWidget.h"
#include "InputActionWidget.generated.h"

class UInputActionListWidget;
class UBrickButtonWidget;
class UControlHintWidget;
class UInputCategory;
class UBaseInputComponent;
enum class EInputActionListMode : uint8;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputActionWidget : public UBrickUserWidget
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_DELEGATE(FActionDelegate);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FAxisDelegate, float, NewValue);

	DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FIsActionEnabled, bool, bIsSecondaryAction);

	DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FIsActionEnabledNoParams);

	DECLARE_DYNAMIC_DELEGATE_RetVal(FText, FGetValueTextDelegate);

	// ~Variables
	// The outer list widget
	TWeakObjectPtr<UInputActionListWidget> ParentWidget;
	// The cached parent list mode
	EInputActionListMode ActionListMode;
	// Name of the associated action
	FName ActionName;
	// Cached display info of the action
	FDisplayInfo DisplayInfo;
	FText DescriptionText;
	EInputActionTriggerType ActionTriggerType;
	// Whether the displayed action can ever be executed
	uint8 bIsValidAction : 1;
	uint8 bCanActionBeRebound : 1;
	uint8 bIsActionEnabled : 1;
	// Whether this widget represents the secondary action (hold or negative axis)
	uint8 bIsSecondaryAction : 1;
	// Whether the action is currently pressed
	uint8 bIsActionPressed : 1;
	// Cached action delegates
	FActionDelegate PressedDelegate;
	FActionDelegate ReleasedDelegate;
	FAxisDelegate AxisDelegate;
	FIsActionEnabled IsActionEnabledDelegate;
	FIsActionEnabledNoParams IsActionEnabledNoParamsDelegate;
	FGetValueTextDelegate GetValueTextDelegate;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UControlHintWidget* ControlHintWidget;
	// ~Widget

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

	// Called once after the widget has been constructed
	void InitializeInputActionWidget(UInputActionListWidget* InParentWidget);
	// Initializes this widget as an action
	void InitializeAction(UBaseInputComponent* InInputComponent, const FName& InActionName, const FInputTableRow* InInputTableRow, bool bInIsSecondaryAction, EInputActionListMode InListMode);

protected:
	// To be called when the action button has been clicked or released respectively
	UFUNCTION(BlueprintCallable)
	void OnActionClicked();
	UFUNCTION(BlueprintCallable)
	void OnActionPressed();
	UFUNCTION(BlueprintCallable)
	void OnActionReleased();
	// Opens the popup to rebind the action
	UFUNCTION(BlueprintCallable)
	void RebindAction();

public:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	UWidget* GetWidgetToFocus() const;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInputActionListMode(EInputActionListMode NewMode);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanActionBeRebound(bool bInCanBeRebound);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateDisplayInfo(const FDisplayInfo& InDisplayInfo);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIsActionEnabled(bool bInCanBeExecuted);
	// ~Blueprint Interface

private:
	// Triggers either the pressed or released delegate
	void ExecuteActionDelegate(const FActionDelegate& InDelegate, bool bReleased);
	// Update the displayed text and icon
	void UpdateDisplayInfo();
	// Updates the enabled state of an action
	void UpdateIsActionEnabled();
};
