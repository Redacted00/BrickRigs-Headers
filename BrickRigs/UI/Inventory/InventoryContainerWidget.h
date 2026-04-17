// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryContainerWidget.generated.h"

class UInventoryComponent;
class UInventoryWidget;
class UBrickTextBlock;

/**
 * Used to house inventories and sub inventories
 */
UCLASS(Abstract)
class BRICKRIGS_API UInventoryContainerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UInventoryWidget* InventoryWidget;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the associated inventory component
	void SetInventoryComponent(UInventoryComponent* InInventory, bool bInIsSubInventory);
	// Returns the contained inventory widget
	UInventoryWidget* GetInventoryWidget() const;
	// Updates the money of the owning character
	void SetOwnerMoney(float Money);

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateHasInventory(bool bHasInventory);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOwnerMoney(float NewMoney);
};
