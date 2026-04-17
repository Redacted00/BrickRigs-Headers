// Copyright Fluppisoft, 2021

#pragma once

#include "UGC/UGCTypes.h"
#include "UI/Widgets/BrickComboBoxWidget.h"
#include "CoreMinimal.h"
#include "Properties/BrickProperties.h"
#include "UI/Properties/PropertyWidget.h"
#include "UGCTagsPropertyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UUGCTagsPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickComboBoxWidget* TypeComboBox;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickComboBoxWidget* EraComboBox;
	UPROPERTY(BlueprintReadOnly, Meta = (BindWidgetOptional))
	UBrickComboBoxWidget* DepartmentComboBox;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

private:
	// Callbacks for the combo boxes
	UFUNCTION()
	void InitializeTypeItem(int32 Item, FBrickComboBoxItemParams& OutParams);
	UFUNCTION()
	void InitializeEraItem(int32 Item, FBrickComboBoxItemParams& OutParams);
	UFUNCTION()
	void InitializeDepartmentItem(int32 Item, FBrickComboBoxItemParams& OutParams);

	UFUNCTION()
	void OnTypeItemSelected(int32 Item, EValueChangedEventType EventType)
	{
		OnComboBoxItemSelected<EUGCTypeTag, &FUGCTags::Type>(Item, EventType);
	}

	UFUNCTION()
	void OnEraItemSelected(int32 Item, EValueChangedEventType EventType)
	{
		OnComboBoxItemSelected<EUGCEraTag, &FUGCTags::Era>(Item, EventType);
	}

	UFUNCTION()
	void OnDepartmentItemSelected(int32 Item, EValueChangedEventType EventType)
	{
		OnComboBoxItemSelected<EUGCDepartmentTag, &FUGCTags::Department>(Item, EventType);
	}

	// Helper function used for all combo boxes
	void InitializeComboBoxItem(int32 Item, FBrickComboBoxItemParams& OutParams, const FText& TagName, const FText& ItemName);
	// Combined event that is used by all combo boxes
	template <typename EnumType, EnumType FUGCTags::* TagMember>
	void OnComboBoxItemSelected(int32 Item, EValueChangedEventType EventType)
	{
		auto Value = FUGCTags();
		if (GetPropertyValue<FUGCTagsBrickProperty>(Value))
		{
			Value.*TagMember = static_cast<EnumType>(Item);
			SetPropertyValue<FUGCTagsBrickProperty>(EventType, Value);
		}
	}
};
