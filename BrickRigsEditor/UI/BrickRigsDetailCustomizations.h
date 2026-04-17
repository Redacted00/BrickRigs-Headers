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
