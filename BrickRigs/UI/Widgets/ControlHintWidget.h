// Copyright Fluppisoft, 2019

#pragma once

#include "Properties/BrickPropertyChangedEvent.h"
#include "UI/Misc/DisplayInfo.h"
#include "UI/Input/InputActionAndAxisMapping.h"
#include "UI/Input/InputActionTriggerType.h"
#include "UI/Input/InputMethod.h"
#include "Framework/Application/IInputProcessor.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlHintWidget.generated.h"

class UControlHintWidget;

// Used to listen to input events
class FControlHintInputProcessor : public IInputProcessor
{
public:
	FControlHintInputProcessor(UControlHintWidget* InOwner)
	{
		Owner = InOwner;
	}

	// ~Super Interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override;
	virtual bool HandleMotionDetectedEvent(FSlateApplication& SlateApp, const FMotionEvent& MotionEvent) override;
	// ~Super Interface

private:
	void HandleKeyDownEvent(const FKey& InKey);
	void HandleKeyUpEvent(const FKey& InKey);
	void HandleKeyEvent(const FKey& InKey, bool bDown);

	TWeakObjectPtr<UControlHintWidget> Owner;
};

class UScaleBox;
class UInputChordWidget;
class UBrickTextBlock;
class UBrickImage;

UENUM(BlueprintType)
enum class EControlHintDisplayInfoMode : uint8
{
	Auto,
	// Text and icon are automatically updated from the input table
	Custom,
	// Text and icon have to be set manually
	Hidden // Text and icon are not visible
};

/**
 * This widget can display the keys required for an input action
 */
UCLASS(Abstract)
class BRICKRIGS_API UControlHintWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Whether the action has been initialized yet
	uint8 bIsActionInitialized : 1;
	// Whether the text and icon are currently visible
	uint8 bIsTextVisible : 1;
	uint8 bIsIconVisible : 1;
	// Whether our key is currently being held
	uint8 bIsInputChordPressed : 1;
	// The currently active input mapping we are displaying
	FInputActionAndAxisMapping ActiveInputMapping;
	// The number of input mappings associated with this action
	int32 NumInputMappings;
	// Index of the input mapping currently displayed
	int32 CurrentInputMappingIndex;
	// Real time seconds when the key has been pressed
	float InputChordPressTime;
	// Current progress of the hold key action
	float HoldProgress;
	// How much padding is currently applied to the key widget
	float CurrentKeyPaddingRatio;
	// The input processor instance
	TSharedPtr<FControlHintInputProcessor> InputProcessor;
	// The currently displayed text and icon
	FDisplayInfo CurrentDisplayInfo;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UScaleBox* InputChordScaleBox;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInputChordWidget* InputChordWidget;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* TextBlock;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* IconImage;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	FName ActionName;
	// How the action is activated, through a simple press or hold etc.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	EInputActionTriggerType TriggerType;
	// Whether the "unbound" key should be displayed if the action is unbound
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	bool bShowUnboundKey;
	// Whether the input chord should be visible
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	bool bShowInputChord;
	// Optional custom display info
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	FDisplayInfo CustomDisplayInfo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	EControlHintDisplayInfoMode DisplayInfoMode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"), Category = Action)
	EBrickUIStyleState StyleState;

	// Padding between the text and input chord
	UPROPERTY(EditDefaultsOnly, Category = ControlHint)
	float TextSpacing;
	UPROPERTY(EditDefaultsOnly, Category = ControlHint)
	float HoldProgressInterpSpeed;
	// Padding applied to the key widget when it is pressed
	UPROPERTY(EditDefaultsOnly, Category = ControlHint)
	FVector2D PressedKeyPadding;
	UPROPERTY(EditDefaultsOnly, Category = ControlHint)
	float KeyPaddingInterpSpeed;
	// ~Properties

public:
	// ~Constructor
	UControlHintWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaSeconds) override;
	virtual void SetIsEnabled(bool bNewEnabled) override;
	virtual void SetVisibility(ESlateVisibility NewVisibility) override;
	// ~Super Interface

	// Change the action associated with this widget
	UFUNCTION(BlueprintCallable)
	void SetAction(FName InActionName, EInputActionTriggerType InTriggerType);
	// Show or hide the unbound key when the action is not bound
	UFUNCTION(BlueprintCallable)
	void SetShowUnboundKey(bool bShow);
	// Shows or hides the input chord
	UFUNCTION(BlueprintCallable)
	void SetShowInputChord(bool bShow);
	// Sets how the display info is generated
	UFUNCTION(BlueprintCallable)
	void SetDisplayInfoMode(EControlHintDisplayInfoMode NewMode);
	// Assigns a custom display text and icon
	UFUNCTION(BlueprintCallable)
	void SetDisplayInfo(const FDisplayInfo& InDisplayInfo);

	// Get the currently displayed input mapping
	const FInputActionAndAxisMapping& GetActiveInputMapping() const
	{
		return ActiveInputMapping;
	}

	// Called from the input processor
	void OnInputProcessorKeyEvent(const FKey& Key, bool bDown);

	// ~Blueprint Interface
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTextStyle(EBrickUITextStyle NewStyle);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetStyleState(EBrickUIStyleState NewState);
	// Sets the custom scale factor for the input chord widget
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetInputChordScale(float InScale = 1.f);

protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateTextVisibility(bool bNewVisible);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIconVisibility(bool bNewVisible);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateDisplayInfo(const FDisplayInfo& NewDisplayInfo);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInputChordVisibility(bool bNewVisible);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateInputChordStyleState(EBrickUIStyleState InStyleState, bool bInIsHoldAction);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsHoldAction(bool bIsHoldAction);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHoldProgress(float InHoldProgress);
	// ~Blueprint Interface

private:
	// Syncs the displayed style to all sub widgets
	void UpdateColorStyle();
	void UpdateStyleState();
	// Called when the player switches between keyboard and gamepad control
	UFUNCTION()
	void OnInputMethodChanged(EInputMethod NewInputMethod);
	// Creates or destroys the input processor if needed
	void UpdateInputProcessor();
	// Creates an input processor if needed
	bool CreateInputProcessor();
	// Deletes the input processor
	bool RemoveInputProcessor();
	// Update the pressed state
	void SetInputChordPressed(bool bNewPressed);
	// Update the hold progress value
	void SetHoldProgress(float NewHoldProgress);
	// Helper function to set the current key padding ratio
	void SetKeyPaddingRatio(float NewRatio);
	// Resets the key down state
	void ResetKeyPressedState();
	// Update the underlying action parameters
	void UpdateAction();
	// To be used after the display info mode has been changed
	void UpdateDisplayInfoMode();
	// Updates the displayed text and icon
	void UpdateDisplayInfo();
	// Manages the visibility of the text and icon
	void UpdateDisplayInfoVisibility();
	// Update the currently active input mapping
	bool UpdateInputMapping();
	// Updates the visibility of the input chord widget
	void UpdateInputChordVisibility();
	// Updates the style of the input chord
	void UpdateInputChordStyleState();
	// Callback for the property system
	void OnAnyBrickPropertyModified(const FBrickPropertyChangedEvent& Event);
};
