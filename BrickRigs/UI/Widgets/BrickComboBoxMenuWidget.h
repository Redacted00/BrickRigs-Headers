// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "MenuAnchorWidget.h"
#include "BrickComboBoxMenuWidget.generated.h"

class UBrickComboBoxWidget;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickComboBoxMenuWidget : public UMenuAnchorWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The currently owning combo box
	TWeakObjectPtr<UBrickComboBoxWidget> ParentComboBox;
	// ~Variables

public:
	// ~Widgets
	UPROPERTY(BlueprintReadWrite, Meta = (BindWidgetOptional))
	UBrickComboBoxWidget* ComboBox;
	// ~Widgets

	// ~Super Interface
	virtual void NativeDestruct() override;
	virtual UWidget* GetWidgetToFocus_Implementation() const override;
	// ~Super Interface

	// Sets up the combo box menu
	void InitializeComboBoxMenu(UBrickComboBoxWidget* InComboBox);
};
