// Copyright Fluppisoft 2018

#pragma once

#include "PropertyHandle.h"
#include "CoreMinimal.h"
#include "PropertyEditor/Public/IDetailCustomization.h"

class AStaticMeshProp;
class SWidget;

class FStaticMeshPropDetails : public IDetailCustomization
{
	TArray<TSharedPtr<FName>> MaterialOptionsList;

public:
	static TSharedRef<IDetailCustomization> MakeInstance();
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	// Callbacks for the combo box
	void GetComboBoxStrings(TArray<TSharedPtr<FString>>& OutComboBoxStrings, TArray<TSharedPtr<SToolTip>>& OutToolTips, TArray<bool>& OutRestrictedItems) const;
	FString GetComboBoxValueStr(TSharedPtr<IPropertyHandle> PropertyHandle) const;
	void OnComboBoxSelectionChanged(const FString& Selection, TSharedPtr<IPropertyHandle> PropertyHandle);
};

class FBrickUnitsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;

private:
	// Widget callbacks
	void OnUnitsChanged(const float NewValue, TSharedRef<IPropertyHandle> PropertyHandle);
	void OnUnitsCommitted(const float NewValue, ETextCommit::Type CommitMethod, TSharedRef<IPropertyHandle> PropertyHandle);
	float GetUnits(TSharedRef<IPropertyHandle> PropertyHandle) const;
	FText GetUnitText(TSharedRef<IPropertyHandle> PropertyHandle) const;
};
