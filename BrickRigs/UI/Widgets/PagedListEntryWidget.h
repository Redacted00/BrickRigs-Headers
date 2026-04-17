// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/BrickPropertyInterface.h"
#include "UI/Misc/PagedListEntry.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PagedListEntryWidget.generated.h"

class UBrickButtonWidget;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPagedListEntryWidget : public UUserWidget, public IBrickPropertyInterface
{
	GENERATED_BODY()

protected:
	DECLARE_DELEGATE_TwoParams(FOnEntryClicked, UPagedListEntryWidget*, bool);

	// ~Variables
	// Index this entry represents
	int32 EntryIndex;
	// Whether this entry is currently selected
	bool bIsSelected;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	// ~Widgets

public:
	// ~Delegates
	FOnEntryClicked OnEntryClickedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Assigns a new entry to the widget
	void InitializeEntry(const FPagedListEntry& InEntry);
	// Returns the associated entry
	const FPagedListEntry* GetEntry() const
	{
		const FStructProperty* EntryProperty = GetEntryProperty();
		return EntryProperty && EntryProperty->Struct->IsChildOf(FPagedListEntry::StaticStruct()) ? EntryProperty->ContainerPtrToValuePtr<FPagedListEntry>(this) : nullptr;
	}

	// Called from the list to update the selected appearance
	void SetIsSelected(bool bNewSelected);

protected:
	// Callbacks for the button
	UFUNCTION(BlueprintCallable)
	void OnClickedEntry();
	UFUNCTION(BlueprintCallable)
	void OnDoubleClickedEntry();

	// ~Blueprint Interface
	UFUNCTION(BlueprintNativeEvent)
	void InitializeEntry();
	// Updates the selected appearance
	UFUNCTION(BlueprintNativeEvent)
	void UpdateButtonSelected(bool bNewSelected);
	// ~Blueprint Interface

private:
	// Tries to find the shared item property
	FStructProperty* GetEntryProperty() const
	{
		return CastField<FStructProperty>(GetClass()->FindPropertyByName("Entry"));
	}

	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~IBrickPropertyInterface
};
