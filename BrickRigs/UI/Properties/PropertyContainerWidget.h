// Copyright Fluppisoft, 2019

#pragma once

#include "Input/BaseInputComponent.h"
#include "Properties/BrickPropertyInterface.h"
#include "Properties/BrickPropertyEditInfo.h"
#include "CoreMinimal.h"
#include "Misc/BrickAssetManager.h"
#include "UI/Widgets/BrickUserWidget.h"
#include "PropertyContainerWidget.generated.h"

struct FBrickProperty;
struct FBrickPropertyEditInfo;
class UBrickTextBlock;
class UBrickImage;
class UBrickBorder;
class UPropertiesPanelWidget;
class UPropertyCategoryWidget;
class ABrickEditor;

// Input component used for the context menu
UCLASS()
class BRICKRIGS_API UPropertyContainerInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

	// ~Variables
	// The property being edited
	TSharedPtr<FBrickPropertyEditInfo> PropertyInfo;
	// ~Variables

public:
	// ~Super Interface
	virtual void DestroyComponent(bool bDestroyHierarchy = false) override;
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetContextMenuOwner(UObject* InOwner) override;
	// ~Super Interface

private:
	// Returns the editor associated with the property
	ABrickEditor* GetBrickEditor() const;

	UFUNCTION()
	void OnPressedPickValue();
	UFUNCTION()
	bool GetPickValueEnabled() const;
	UFUNCTION()
	void OnPressedSelectByValue();
	UFUNCTION()
	bool GetSelectByValueEnabled() const;
	UFUNCTION()
	void OnPressedCopyValue();
	UFUNCTION()
	bool GetCopyValueEnabled() const;
	UFUNCTION()
	void OnPressedPasteValue();
	UFUNCTION()
	bool GetPasteValueEnabled() const;
};

/**
* This class is used as a container for all property specific widgets
 */
UCLASS(Abstract)
class BRICKRIGS_API UPropertyContainerWidget : public UBrickUserWidget
{
	GENERATED_BODY()

	// ~Variables
	// Streamable handle used during async loading of the property widget class
	FSmartStreamableHandle StreamableHandle_PropertyWidgetClass;
	// Property list object containing this property
	TWeakObjectPtr<UWidget> PropertyList;
	// Slot index the widget was added to
	int32 PropertyListSlotIndex = -1;
	// Last updated orientation
	TEnumAsByte<EOrientation> Orientation = static_cast<EOrientation>(MAX_uint8);
	// The property details associated with this container
	TSharedPtr<FBrickPropertyEditInfo> PropertyInfo;
	// The displayed property value widget
	UPROPERTY(Transient)
	UPropertyWidget* PropertyWidget;
	// Set when the property is focused, optionally with a sub property name
	TOptional<FName> FocusedSubProperty;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	// ~Widgets

public:
	// ~Constructor
	UPropertyContainerWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void NativeDestruct() override;
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

	auto* GetPropertyList() const
	{
		return PropertyList.Get();
	}

	// Sets the outer parent widget
	void AddToPropertyList(UWidget* InPropertyList, int32 InNumPerRow);
	// Initialize this property, will create a property widget or use one from the pool
	void InitializeProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo, EOrientation InOrientation);
	// Uninitializes the property references and delegates and pools the property widget
	void UninitializeProperty();
	// Update the displayed value and metadata (name, enabled etc)
	void UpdateProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo, const FBrickPropertyChangedEvent* ChangedEvent);
	// Return whether this container is already displaying the given property
	bool IsDisplayingProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo) const;
	// Public accessor to the property info
	const auto& GetPropertyInfo() const
	{
		return PropertyInfo;
	}

	UPropertyWidget* GetPropertyWidget() const
	{
		return PropertyWidget;
	}

protected:
	// Opens the property context menu
	UFUNCTION(BlueprintCallable)
	bool OpenContextMenu();

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddPropertyWidget(UPropertyWidget* Widget, EOrientation InOrientation);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateOrientation(EOrientation InOrientation);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsReadOnly(bool bInReadOnly);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateColorStyle(EBrickUIColorStyle InColorStyle);

public:
	UFUNCTION(BlueprintNativeEvent)
	UWidget* GetWidgetToFocus() const;
	// ~Blueprint Interface

private:
	// Tries to get the brick editor housing the property
	ABrickEditor* GetBrickEditor() const;
	// Updates the focused sub property
	void SetFocusedSubProperty(const TOptional<FName>& NewFocusedSubProp);
	// Updates the focused state of the selected objects
	void UpdateFocusedSubProperty();
};
