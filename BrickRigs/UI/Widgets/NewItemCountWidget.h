// Copyright Fluppisoft, 2019

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NewItemCountWidget.generated.h"

/**
 * This widget is used to display the number of new/unread/unseen items, like for unread chat messages for example
 */
UCLASS(Abstract)
class BRICKRIGS_API UNewItemCountWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// The count that is currently displayed
	int32 NumItemsDisplayed;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the displayed number of items
	UFUNCTION(BlueprintCallable)
	void SetNumNewItems(int32 NewNum);

protected:
	// Updates the item count
	void UpdateNumItems();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateNumItems(int32 NewNum);
	// ~Blueprint Interface
};
