// Copyright Fluppisoft, 2018

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuPageWidget.generated.h"

class UMenuWidget;

UENUM(BlueprintType)
enum class EMenuPageSize : uint8
{
	Default,
	Large
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UMenuPageWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// The outer menu widget
	TWeakObjectPtr<UMenuWidget> OwningMenu;
	// ~Variables

protected:
	// ~Properties
	// The desired size in the menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EMenuPageSize MenuPageSize;
	// ~Properties

public:
	// Sets the outer menu upon initialization
	void SetOwningMenu(UMenuWidget* InMenu);
	// Return the owning menu
	UFUNCTION(BlueprintPure)
	UMenuWidget* GetMenuWidget() const;

	// ~Blueprint Interface
	// Return the sub widget that should receive user focus by default
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// Allows the menu page to customize the back behavior
	UFUNCTION(BlueprintNativeEvent)
	bool StepBack();
	// ~Blueprint Interface
};

FORCEINLINE UMenuWidget* UMenuPageWidget::GetMenuWidget() const
{
	return OwningMenu.Get();
}
