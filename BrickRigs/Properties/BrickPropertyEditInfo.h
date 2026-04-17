#pragma once

#include "BrickPropertyChangedEvent.h"
#include "BrickProperty.h"
#include "CoreMinimal.h"

class ABasePlayerController;

struct FBrickPropertyEditInfo : FBrickPropertyInstance, TSharedFromThis<FBrickPropertyEditInfo>
{
private:
	// ~Variables
	// The property name displayed
	FText DisplayName;
	// Long property description
	FText DescriptionText;
	// List of objects being edited
	const TArray<TWeakObjectPtr<UObject>> ContainerObjects;
	// Whether the property should appear as enabled
	bool bIsEnabled;
	// Whether the property is shown as read only
	bool bIsReadOnly;
	// Used for combo box properties, max number of items to display in a combo box
	int32 MaxComboBoxListItems;
	// Max amount of items to display per row
	int32 MaxComboBoxItemsPerRow;
	// Changed event that has yet to be committed
	TSharedPtr<FBrickPropertyChangedEvent> PendingChangedEvent;
	// ~Variables

public:
	// ~Constructor
	FBrickPropertyEditInfo(const FBrickPropertyInstance& InPropertyInstance, const FText& InDisplayName, const TArray<TWeakObjectPtr<UObject>>& InContainerObjects)
		: FBrickPropertyInstance(InPropertyInstance), DisplayName(InDisplayName), ContainerObjects(InContainerObjects), bIsEnabled(true), bIsReadOnly(false), MaxComboBoxListItems(INDEX_NONE), MaxComboBoxItemsPerRow(0)
	{
	}

	// ~Destructor
	~FBrickPropertyEditInfo()
	{
		// If we still have a pending changed event, force commit and broadcast it
		if (PendingChangedEvent.IsValid())
		{
			PendingChangedEvent->SetEventType(EValueChangedEventType::Commit);
			FBrickProperty::OnPropertyModified(*PendingChangedEvent.Get());
			PendingChangedEvent.Reset();
		}
	}

	bool HasPendingChangedEvent() const
	{
		return PendingChangedEvent.IsValid();
	}

	void CopyPendingChangedEvent(FBrickPropertyEditInfo& Other)
	{
		PendingChangedEvent = Other.PendingChangedEvent;
		Other.PendingChangedEvent.Reset();
	}

	const FText& GetDisplayName() const
	{
		return DisplayName;
	}

	FBrickPropertyEditInfo& SetDescription(const FText& Desc)
	{
		DescriptionText = Desc;
		return *this;
	}

	const auto& GetDescription() const
	{
		return DescriptionText;
	}

	// Can be used to draw the property as disabled
	FBrickPropertyEditInfo& SetIsEnabled(bool bNewEnabled)
	{
		bIsEnabled = bNewEnabled;
		return *this;
	}

	bool GetIsEnabled() const
	{
		return bIsEnabled;
	}

	FBrickPropertyEditInfo& SetIsReadOnly(bool bInReadOnly)
	{
		bIsReadOnly = bInReadOnly;
		return *this;
	}

	bool GetIsReadOnly() const
	{
		return bIsReadOnly;
	}

	int32 GetMaxComboBoxListItems() const
	{
		return MaxComboBoxListItems;
	}

	FBrickPropertyEditInfo& SetMaxComboBoxListItems(int32 InMaxComboBoxListItems)
	{
		MaxComboBoxListItems = InMaxComboBoxListItems;
		return *this;
	}

	int32 GetMaxComboBoxItemsPerRow() const
	{
		return MaxComboBoxItemsPerRow;
	}

	FBrickPropertyEditInfo& SetMaxComboBoxItemsPerRow(int32 InMaxComboBoxItemsPerRow)
	{
		MaxComboBoxItemsPerRow = InMaxComboBoxItemsPerRow;
		return *this;
	}

	// Get the selected/active object
	UObject* GetSelectedObject() const
	{
		if (ContainerObjects.Num())
		{
			return ContainerObjects[0].Get();
		}
		return nullptr;
	}

	const auto& GetContainerObjects() const
	{
		return ContainerObjects;
	}

	FBrickPropertyContainer GetSelectedPropertyContainer() const
	{
		const auto* SelectedObject = GetSelectedObject();
		return SelectedObject && IsInObject(SelectedObject) ? GetPropertyContainer(SelectedObject) : FBrickPropertyContainer();
	}

	// Whether the property can be modified for the given container
	bool CanModifyProperty(const FBrickPropertyContainer& Container) const;

	// This function should be used by property widgets to get the displayed value
	template <typename PropertyType, typename ValueType>
	bool GetPropertyValue(ValueType& OutValue) const
	{
		if (const auto SelectedContainer = GetSelectedPropertyContainer(); SelectedContainer.IsValid())
		{
			return GetPropertyValueForContainer<PropertyType, ValueType>(SelectedContainer, OutValue);
		}

		return false;
	}

	// Version that uses the FGetBrickPropertyValueParams struct
	template <typename PropertyType, typename ValueType>
	bool GetPropertyDefaultValue(ValueType& OutValue, FDisplayInfo& OutDisplayInfo) const
	{
		if (const auto SelectedContainer = GetSelectedPropertyContainer(); SelectedContainer.IsValid())
		{
			return GetPropertyDefaultValueForContainer<PropertyType, ValueType>(SelectedContainer, OutValue, OutDisplayInfo);
		}

		return false;
	}

	// Calls the GetValueAsText function on the property
	bool GetPropertyValueAsText(FText& OutText) const
	{
		return BrickProperty->GetValueAsText(GetSelectedPropertyContainer(), OutText);
	}

	// This function should be used by property widgets to set the value instead of calling SetValue directly
	template <typename PropertyType, typename... P>
	bool SetPropertyValue(ABasePlayerController* PC, EValueChangedEventType EventType, const P&... Params)
	{
		return SetPropertyValueInternal<PropertyType>(PC, EventType, [&](const PropertyType* CastedProperty, const FBrickPropertyContainer& PropertyContainer)
		{
			return CastedProperty->SetValue(PropertyContainer, Params...);
		});
	}

	// Sets the property value via a text input
	bool SetPropertyValueAsText(ABasePlayerController* PC, EValueChangedEventType EventType, const FText& InText)
	{
		return SetPropertyValueInternal<FBrickProperty>(PC, EventType, [&](const FBrickProperty* CastedProperty, const FBrickPropertyContainer& PropertyContainer)
		{
			return CastedProperty->SetValueAsText(PropertyContainer, InText);
		});
	}

	// Copies the property value from one object to all others
	bool SetPropertyValueByCopy(ABasePlayerController* PC, EValueChangedEventType EventType, const UBrickEditorObject* FromObject);

	// Copies the active object's property value to the clipboard
	bool CopyPropertyToClipboard() const
	{
		return GetBrickProperty()->CopyPropertyToClipboard(GetSelectedPropertyContainer());
	}

	// Whether the property value can be copied
	bool CanCopyPropertyToClipboard() const
	{
		return GetBrickProperty()->CanCopyPropertyToClipboard(GetSelectedPropertyContainer());
	}

	// Pastes the active object's property value from the clipboard
	bool PastePropertyFromClipboard(ABasePlayerController* PC)
	{
		return SetPropertyValueInternal<FBrickProperty>(PC, EValueChangedEventType::Commit, [&](const FBrickProperty* CastedProperty, const FBrickPropertyContainer& PropertyContainer)
		{
			return CastedProperty->PastePropertyFromClipboard(PropertyContainer);
		});
	}

	// Whether pasting is currently possible
	bool CanPastePropertyFromClipboard() const
	{
		return !GetIsReadOnly() && GetBrickProperty()->CanPastePropertyFromClipboard(GetSelectedPropertyContainer());
	}

private:
	// Internal version that allows a template to process setting the value
	template <typename PropertyType, typename SetterType>
	bool SetPropertyValueInternal(ABasePlayerController* PC, EValueChangedEventType EventType, SetterType SetterFunction);
};

template <typename PropertyType, typename SetterType>
bool FBrickPropertyEditInfo::SetPropertyValueInternal(ABasePlayerController* PC, EValueChangedEventType EventType, SetterType SetterFunction)
{
	if (bIsReadOnly)
	{
		return false;
	}

	// Make sure this property is actually of the correct type
	const PropertyType* CastedProperty = GetBrickProperty<PropertyType>();
	if (!CastedProperty)
	{
		return false;
	}

	// IMPORTANT: We have to remember all objects that had their value changed in any form, and broadcast a changed event for them as well when the value is committed
	// This has to be done to avoid cases with combo boxes for example, where a pending value is applied but a changed event with IsValueCommitted is never broadcast
	// because the final value isn't supported by the container

	// Build the changed event
	if (!PendingChangedEvent.IsValid())
	{
		PendingChangedEvent = MakeShared<FBrickPropertyChangedEvent>(PC, GetPropertyName(), GetFullPropertyName(), ParentPropertyChain, GetSelectedObject(), EventType);
	}
	else
	{
		// Overrite the event type
		PendingChangedEvent->SetEventType(EventType);
	}

	const int32 NumContainers = ContainerObjects.Num();
	PendingChangedEvent->ReserveObjects(NumContainers);

	// NOTE: Iterate the containers in reverse, so the active object gets to set the value last
	// This is needed for the driver seat property for example
	for (int32 i = NumContainers - 1; i >= 0; --i)
	{
		UObject* ContainerObject = ContainerObjects[i].Get();

		// Make sure the container actually supports the property, and that it can be modified
		if (!ContainerObject || !IsInObject(ContainerObject) || !CanModifyProperty(GetPropertyContainer(ContainerObject)))
		{
			continue;
		}

		// Set the actual value
		if (SetterFunction(CastedProperty, GetPropertyContainer(ContainerObject)))
		{
			// Add the object to the changed event
			PendingChangedEvent->AddObject(ContainerObject);
		}
	}

	if (PendingChangedEvent->GetNumObjects() > 0)
	{
		// Make a copy of the changed event, since we might delete it before broadcasting it
		const FBrickPropertyChangedEvent ChangedEvent = *PendingChangedEvent.Get();
		// Reset the pending event if the value is being committed
		if (PendingChangedEvent->IsValueCommitted())
		{
			PendingChangedEvent.Reset();
		}
		FBrickProperty::OnPropertyModified(ChangedEvent);
		return true;
	}
	PendingChangedEvent.Reset();

	return false;
}
