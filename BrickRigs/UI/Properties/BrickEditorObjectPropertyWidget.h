// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/BrickEditorObjectBrickProperty.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "BrickEditorObjectPropertyWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickEditorObjectPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void InitializeProperty() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	// Enters the object picker tool
	UFUNCTION(BlueprintCallable)
	void OpenObjectPicker();
	// Selects the picked object in the editor
	UFUNCTION(BlueprintCallable)
	void SelectObjects();
	// Clears all selected objects
	UFUNCTION(BlueprintCallable)
	void ClearSelection();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateSelectedObjects(const FText& FirstObjectName, int32 NumSelectedObjects, bool bDefaultObjects);
	// ~Blueprint Interface

private:
	// Callback for the object picker mode
	void OnObjectsSelected(const TArray<FBrickEditorObjectPtr>& InObjects);
};
