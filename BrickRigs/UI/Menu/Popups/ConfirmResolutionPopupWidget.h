// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "ConfirmResolutionPopupWidget.generated.h"

UCLASS()
class BRICKRIGS_API UConfirmResolutionPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void NativeOnPopupClosed(const EPopupResult Result) override;
	// ~Super Interface
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UConfirmResolutionPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

	// ~Variables
	// Timer used to update the countdown
	FFluUITimer Timer_UpdateCountdown;
	// Number of seconds remaining until the resolution is reverted
	int32 SecondsRemaining;
	// ~Variables

protected:
	// ~Properties
	// Time until the resolution is automatically reverted
	UPROPERTY(EditDefaultsOnly, Category = Popup)
	float RevertTime;
	// ~Properties

public:
	// ~Constructor
	UConfirmResolutionPopupWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual FText GetPopupTitle_Implementation() const override;
	// ~Super Interface

private:
	// Timer callback
	void UpdateCountdown();
	// Starts the update timer
	void SetupCountdownTimer();
};
