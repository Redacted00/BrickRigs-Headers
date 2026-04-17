// Copyright Fluppisoft, 2019

#pragma once

#include "PropertyCategoryWidget.h"
#include "PropertyContainerWidget.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PropertiesPanelWidget.generated.h"

class UPropertyContainerWidget;
class UPropertyCategoryWidget;
class UPropertyWidget;
class UStructPropertyWidget;
class UBrickScrollBox;
class ABrickEditor;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UPropertiesPanelWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Handle for the global brick property delegate
	FDelegateHandle DelegateHandle_AnyPropertyModified;

	// Cached property reflection list used for gathering the properties
	TArray<FBrickPropertyReflection> PropertyReflectionList;

	// All category widgets in use
	UPROPERTY(Transient)
	TArray<UPropertyCategoryWidget*> PropertyCategoryWidgets;
	// All currently used container widgets
	UPROPERTY(Transient)
	TArray<UPropertyContainerWidget*> PropertyContainerWidgets;
	// Set to true during the properties update function
	bool bIsUpdatingProperties = false;

	// The property that is currently focused
	TSharedPtr<FBrickPropertyEditInfo> FocusedPropertyInfo;
	// Custom sub property name, for example the X axis of a vector property
	FName FocusedSubPropertyName;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Properties)
	TSubclassOf<UPropertyCategoryWidget> CategoryWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = Properties)
	TSubclassOf<UPropertyContainerWidget> ContainerWidgetClass;
	UPROPERTY(EditAnywhere, Category = Properties)
	int32 NumPropertiesPerRow;
	UPROPERTY(EditAnywhere, Category = Properties)
	TEnumAsByte<EOrientation> Orientation;
	// ~Properties

public:
	// ~Constructor
	UPropertiesPanelWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Add all brick properties for the given object, optionally with a context list
	UFUNCTION(BlueprintCallable)
	void AddProperties(const TArray<UObject*>& InContainers, const FBrickPropertyReflectionFilter& InFilter);
	// Update the list of properties and their values
	UFUNCTION(BlueprintCallable)
	void UpdateProperties();
	// Remove all properties from the panel
	UFUNCTION(BlueprintCallable)
	void ClearProperties();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddCategoryWidget(UPropertyCategoryWidget* Widget, int32 Index);
	// ~Blueprint Interface

private:
	// Internal version that takes an optional changed event
	void UpdateProperties(const FBrickPropertyChangedEvent* ChangedEvent);

	struct FAddPropertyParams
	{
		const FBrickPropertyReflection& Reflection;
		const FBrickPropertyChangedEvent* ChangedEvent;
		const TSharedRef<FBrickPropertyEditInfo>& PropertyInfo;
		const int32 CategoryIndex;
		const int32 StartCategoryIndex;
		int32& NumContainersUsed;
		int32& NumCategoriesUsed;
	};

	// Adds a single property
	void AddProperty(const FAddPropertyParams& Params);
	// Searches for the associated category and adds it if needed
	UPropertyCategoryWidget* FindOrAddCategory(const FAddPropertyParams& Params);
	// Returns the struct property widget that is the parent of the given property
	UStructPropertyWidget* FindParentPropertyWidget(const FAddPropertyParams& Params) const;
	// Global property interface callback
	void OnAnyPropertyModified(const FBrickPropertyChangedEvent& Event);
};
