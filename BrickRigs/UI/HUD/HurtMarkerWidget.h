// Copyright Fluppisoft, 2020

#pragma once

#include "Character/CharacterDamageInfo.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HurtMarkerWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UHurtMarkerWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE_OneParam(FOnHurtMarkerFadedOut, UHurtMarkerWidget *);

public:
	// ~Delegates
	FOnHurtMarkerFadedOut OnHurtMarkerFadedOutDelegate;
	// ~Delegates

	// ~Constructor
	UHurtMarkerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	// ~Super Interface

	// Initializes the widget
	void InitializeHurtMarker(const FCharacterDamageInfo& DamageInfo);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void PlayHurtMarkerAnimation();
};
