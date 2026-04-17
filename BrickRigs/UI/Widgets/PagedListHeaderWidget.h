// Copyright Fluppisoft, 2021

#pragma once

#include "UI/Misc/PagedListEntry.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PagedListHeaderWidget.generated.h"

/**
 * This widget is used to display shared UGC file information for the editor and UGC browser
 */
UCLASS(Abstract)
class BRICKRIGS_API UPagedListHeaderWidget : public UUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// Cached variables from the entry
	bool bHasEntry;
	FText TitleText;
	FVector Dimensions;
	float Mass;
	int32 NumObjects;
	float Price;
	FVector MaxDimensions;
	float MaxMass;
	int32 MaxNumObjects;
	// Cached editor related parameters
	bool bHasUnsavedChanges;
	int32 NumHiddenObjects;
	int32 NumObjectsWithAerodynamics;
	// Cached player parameters
	float Money;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Updates the displayed entry
	void SetPagedListEntry(const FPagedListEntry* InEntry);
	// Updates the unsaved appearance
	void SetHasUnsavedChanges(bool bNewHasUnsaved);
	// Updates the number of objects hidden
	void SetNumHiddenObjects(int32 NewNum);
	// Updates the number of objects with aerodynamics
	void SetNumObjectsWithAerodynamics(int32 NewNum);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateEntry(const FText& InTitleText, bool bInHasEntry, bool bInHasUnsavedChanges);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateNumObjects(int32 InNumObjects, int32 InMaxNumObjects, int32 InNumHiddenObjects, int32 InNumObjectsWithAerodynamics);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateDimensions(const FVector& InDimensions, const FVector& InMaxDimensions);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateMass(float InMass, float InMaxMass);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdatePrice(float InPrice, float InMoney);
	// ~Blueprint Interface

private:
	// Updates the displayed entry
	void UpdateEntry();
	// Updates the object counter
	void UpdateNumObjects();
	// Updates the displayed dimensions
	void UpdateDimensions();
	// Updates the displayed mass
	void UpdateMass();
	// Updates the displayed price
	void UpdatePrice();
	// Callback for the player state
	void OnMoneyChanged(float NewMoney, float PrevMoney);
	void SetMoney(float NewMoney);
	// Callback for the user settings
	UFUNCTION()
	void OnMeasurementSystemChanged(EMeasurementSystem NewSystem);
};
