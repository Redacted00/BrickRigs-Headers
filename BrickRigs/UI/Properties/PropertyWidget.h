// Copyright Fluppisoft, 2019

#pragma once

#include "Misc/ValueChangedEventType.h"
#include "Properties/BrickPropertyInterface.h"
#include "Properties/BrickPropertyEditInfo.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PropertyWidget.generated.h"

class ABrickEditor;

/**
 * This is the base class for all specialized properties
 */
UCLASS(Abstract)
class BRICKRIGS_API UPropertyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// ~Variables
	// Our brick property handle
	TSharedPtr<FBrickPropertyEditInfo> PropertyInfo;
	// Whether this widget is currently being initialized
	bool bIsInitializingProperty;
	// Whether the property widget is in read only mode
	bool bIsPropertyReadOnly;
	// ~Variables

	// ~Properties
	// Whether this widget can be displayed in read only mode, otherwise the generic property widget will be displayed
	UPROPERTY(EditDefaultsOnly)
	bool bSupportsReadOnly;
	// ~Properties

public:
	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& OutTags) const override;
	// ~Super Interface

	// Initialize this widget with the given container and property
	void InitializeProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo);
	// Updates the property state
	virtual void UpdateProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo, const FBrickPropertyChangedEvent* ChangedEvent);
	// Uninitialize the property, to stop listening for delegates etc.
	void UninitializeProperty(bool bDummy);
	// Can be implemented to return a custom focused property name
	virtual FName GetFocusedSubProperty(const FWidgetPath& FocusedWidgetPath) const
	{
		return NAME_None;
	}

private:
	// Versions that can be overridden
	virtual void InitializeProperty();
	virtual void UninitializeProperty();
	// Implement to update the displayed value
	virtual void UpdateValue();

protected:
	// Updates the read only state of the property
	UFUNCTION(BlueprintNativeEvent)
	void UpdateIsReadOnly(bool bNewReadOnly);
	// Implement to return the supported property types
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const;

	// Get the selected/active object
	const UObject* GetSelectedObject() const;
	// Get the container of the property
	FBrickPropertyContainer GetPropertyContainer() const;

	// Helper function to get the brick property
	template <typename PropertyType>
	const PropertyType* GetProperty() const
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->GetBrickProperty<PropertyType>();
		}

		return nullptr;
	}

	// Helper function to get the current value of the property
	template <typename PropertyType, typename ValueType>
	bool GetPropertyValue(ValueType& OutValue) const
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->GetPropertyValue<PropertyType, ValueType>(OutValue);
		}

		return false;
	}

	// Advanced version
	template <typename PropertyType, typename ValueType>
	bool GetPropertyDefaultValue(ValueType& OutValue, FDisplayInfo& OutDisplayInfo) const
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->GetPropertyDefaultValue<PropertyType, ValueType>(OutValue, OutDisplayInfo);
		}

		return false;
	}

	// Get the current text value of the property
	bool GetPropertyValueAsText(FText& OutText) const
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->GetPropertyValueAsText(OutText);
		}

		return false;
	}

	// Sets the property value via the brick property system
	template <typename PropertyType, typename... P>
	bool SetPropertyValue(EValueChangedEventType EventType, const P&... Params)
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->SetPropertyValue<PropertyType>(GetBasePlayerController(), EventType, Params...);
		}

		return false;
	}

	// Sets the property value as a text
	bool SetPropertyValueAsText(EValueChangedEventType EventType, const FText& InText)
	{
		if (PropertyInfo.IsValid())
		{
			return PropertyInfo->SetPropertyValueAsText(GetBasePlayerController(), EventType, InText);
		}

		return false;
	}

	// Get the editor associated with the edit info
	ABrickEditor* GetBrickEditor() const;
	// Gets the owning player casted to our custom class, needed since we can't cast in the .h inline function
	ABasePlayerController* GetBasePlayerController() const;
};
