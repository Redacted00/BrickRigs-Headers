// Copyright Fluppisoft, 2020

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AdminLoginWidget.generated.h"

class UBrickTextBoxWidget;

/**
 * Players can use this widget to log in as an admin on a server
 */
UCLASS(Abstract)
class BRICKRIGS_API UAdminLoginWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBoxWidget* TextBox;
	// ~Widgets

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = AdminLogin)
	FText EnterPasswordText;
	UPROPERTY(EditDefaultsOnly, Category = AdminLogin)
	FText LoggedInText;
	// ~Properties

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

protected:
	UFUNCTION(BlueprintNativeEvent)
	void UpdateCanEverBeAdmin(bool bCanBeAdmin);
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIsAdmin(bool bNewIsAdmin);

private:
	// Callback for the player
	void OnIsAdminChanged(bool bNewIsAdmin);
	// Callback for the text box
	UFUNCTION()
	void OnTextChanged(const FText& Text, EValueChangedEventType EventType);
};
