// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Online/SmartUserInfoHandle.h"
#include "UI/Properties/PropertyWidget.h"
#include "UserIdPropertyWidget.generated.h"

class UBrickTextBoxWidget;
/**
 * 
 */
UCLASS()
class BRICKRIGS_API UUserIdPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

	// ~Variables
	FSmartUserInfoHandle UserInfoHandle;
	// The cached user id being displayed
	FUniqueNetIdRepl UserId;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickTextBoxWidget* UserIdTextBox;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UninitializeProperty() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	// Opens the player profile in the browser/overlay
	UFUNCTION(BlueprintCallable)
	void ViewProfileInBrowser();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUserId(const FText& InUserId, const bool bIsValid);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUsername(const FText& InUsername);
	// ~Blueprint Interface

private:
	// Callback for the text box
	UFUNCTION()
	void OnUserIdTextChanged(const FText& Text, EValueChangedEventType EventType);
};
