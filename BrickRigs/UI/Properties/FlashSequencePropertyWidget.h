// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Widgets/BrickComboBoxItemWidget.h"
#include "CoreMinimal.h"
#include "UI/Properties/ObjectPropertyWidget.h"
#include "FlashSequencePropertyWidget.generated.h"

class USirenSequence;

UCLASS(Abstract)
class BRICKRIGS_API UFlashSequenceItemWidget : public UBrickComboBoxItemWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	UPROPERTY(Transient)
	TSubclassOf<USirenSequence> FlashSequenceClass;
	// Whether the icon is currently lit
	bool bIsLit;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& InGeometry, float InDeltaTime) override;
	virtual void InitializeItem_Implementation(int32 InItem, const FBrickComboBoxItemParams& Params) override;
	// ~Super Interface

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsLit(bool bNewLit);
	// ~Blueprint Interface

private:
	// Updates the lit state
	void SetIsLit(bool bNewLit);
};

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UFlashSequencePropertyWidget : public UObjectPropertyWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual void InitializeItem(int32 Item, FBrickComboBoxItemParams& OutParams) override;
	// ~Super Interface
};
