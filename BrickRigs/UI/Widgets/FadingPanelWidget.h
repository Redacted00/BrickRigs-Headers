// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FadingPanelWidget.generated.h"

class UNamedSlot;

/**
 * A panel that unifies fading functionality and visuals
 */
UCLASS(Abstract)
class BRICKRIGS_API UFadingPanelWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadedOut);

	enum class EFadingState : uint8
	{
		FadedIn,
		FadedOut,
		FadingIn,
		FadingOut
	};

	// ~Variables
	// Current state of the fade animation
	EFadingState FadingState;
	// Current fade amount, 1 means fully faded in
	float CurrentFadeAmount;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UNamedSlot* ContentSlot;
	// ~Widgets

public:
	// ~Properties
	// Time it takes to fully fade in or out
	UPROPERTY(EditAnywhere, Category = Fading)
	float FadeTime;
	// ~Properties

	// ~Delegates
	UPROPERTY(BlueprintAssignable)
	FOnFadedOut OnFadedOutDelegate;
	// ~Delegates

	// ~Constructor
	UFadingPanelWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	// ~Super Interface

	// Starts the fade in animation
	UFUNCTION(BlueprintCallable)
	void FadeIn(bool bImmediate = false);
	// Version that can be called from a timer
	UFUNCTION(BlueprintCallable)
	void FadeInTimer();
	// Starts the fade out animation
	UFUNCTION(BlueprintCallable)
	void FadeOut(bool bImmediate = false);
	// Version that can be called from a timer
	UFUNCTION(BlueprintCallable)
	void FadeOutTimer();
	// Whether the widget is currently fading in
	UFUNCTION(BlueprintPure)
	bool IsFadingIn() const;
	// Whether the widget is fully faded in
	UFUNCTION(BlueprintPure)
	bool IsFadedIn() const;
	// Returns whether the widget is currently in the fading out state
	UFUNCTION(BlueprintPure)
	bool IsFadingOut() const;

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	void UpdateFadeAnimation(float FadeAmount);
	// ~Blueprint Interface

private:
	// Change the fade animation state
	bool SetFadingState(EFadingState NewState);
	// Sets the current fade amount
	void SetFadeAmount(float NewAmount);
	// Called when the panel has faded in our out fully
	void OnFadedIn();
	void OnFadedOut();
};

FORCEINLINE void UFadingPanelWidget::FadeInTimer()
{
	FadeIn();
}

FORCEINLINE void UFadingPanelWidget::FadeOutTimer()
{
	FadeOut();
}

FORCEINLINE bool UFadingPanelWidget::IsFadingIn() const
{
	return FadingState == EFadingState::FadingIn;
}

FORCEINLINE bool UFadingPanelWidget::IsFadedIn() const
{
	return FadingState == EFadingState::FadedIn;
}

FORCEINLINE bool UFadingPanelWidget::IsFadingOut() const
{
	return FadingState == EFadingState::FadingOut;
}
