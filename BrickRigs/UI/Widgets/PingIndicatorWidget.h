// Copyright Fluppisoft, 2018

#pragma once

#include  "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PingIndicatorWidget.generated.h"

class ABrickPlayerState;
class UBrickTextBlock;

/**
 * Used to display ping and frame rate values in the server browser, scoreboard or in game
 */
UCLASS(Abstract)
class BRICKRIGS_API UPingIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The player state the ping should be accessed from
	UPROPERTY(Transient)
	ABrickPlayerState* PlayerState;
	// The currently displayed ping
	int32 DisplayedPing;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* PingTextBlock;
	// ~Widgets

	// ~Properties
	// Show the ping indicator only above a certain threshold, <= 0 to disable, inverted for frame rate display
	UPROPERTY(EditAnywhere, Category = PingIndicator)
	int32 MinDisplayPing;
	// Whether to display the FPS instead of ping
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = PingIndicator)
	bool bDisplayFrameRate;
	// ~Properties

public:
	// ~Constructor
	UPingIndicatorWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Link this indicator to a specific player state
	UFUNCTION(BlueprintCallable)
	void SetPlayerState(ABrickPlayerState* InPlayerState);
	// Set the minimum value at which the ping should be displayed
	UFUNCTION(BlueprintCallable)
	void SetMinDisplayPing(int32 InPing);
	// Change the currently displayed ping value, only call this directly if no associated player state is set!
	UFUNCTION(BlueprintCallable)
	void SetPing(int32 InPing);
	// Sets the color style of the text block
	UFUNCTION(BlueprintCallable)
	void SetColorStyle(EBrickUIColorStyle NewStyle);
	// Sets the style state of the text block
	UFUNCTION(BlueprintCallable)
	void SetStyleState(EBrickUIStyleState NewState);
	// Sets the text block style
	UFUNCTION(BlueprintCallable)
	void SetTextStyle(EBrickUITextStyle NewStyle);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	void UpdatePing(int32 InPing);
	// ~Blueprint Interface

private:
	// Update the ping displayed and visibility
	void UpdateDisplayedPing();
	void BindPingDelegate();
	void UnbindPingDelegate();
};
