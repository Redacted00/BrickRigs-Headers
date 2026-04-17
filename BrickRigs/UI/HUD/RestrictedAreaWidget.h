// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestrictedAreaWidget.generated.h"

class UPlayerWidget;
class ABaseCharacter;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API URestrictedAreaWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The owning player widget
	TWeakObjectPtr<UPlayerWidget> PlayerWidget;
	// The character that has entered the restricted area
	TWeakObjectPtr<ABaseCharacter> Character;
	// Currently displayed timer value
	float CurrentTimeRemaining;
	// Whether the widget is in the process of fading out
	bool bIsFadingOut;
	// ~Variables

public:
	// ~Constructor
	URestrictedAreaWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	// ~Super Interface

	// Initializes the owning widget
	void SetPlayerWidget(UPlayerWidget* InWidget);
	// Sets the associated character
	void SetCharacter(ABaseCharacter* InCharacter);
	// Fades in the widget
	void FadeIn();
	// Fade out the widget and hide it afterwards
	void FadeOut();
	// To be called when the fade out animation has finished
	UFUNCTION(BlueprintCallable)
	void OnFadedOut();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimerRemaining(float TimeRemaining);
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeInAnim();
	UFUNCTION(BlueprintImplementableEvent)
	void PlayFadeOutAnim();
	// ~Blueprint Interface
};
