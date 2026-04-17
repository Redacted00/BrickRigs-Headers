#pragma once

#include "Misc/ValueChangedEventType.h"
#include "CoreMinimal.h"

class ABasePlayerController;
struct FBrickPropertyReflection;

// Struct used to pass on changed event params more easily
struct FBrickPropertyChangedEvent
{
private:
	// ~Variables
	// The player who instigated the edit
	TWeakObjectPtr<ABasePlayerController> Player;
	// Full name of the property, including parent properties
	FString FullPropertyName;
	// The chain of property and parent property names, useful for faster comparisons
	TArray<FName> PropertyChain;
	// The current parent property chain depth we are in while calling post modify events for struct properties
	int32 PropertyChainDepth;
	// Objects that have been modified
	TArray<TWeakObjectPtr<>> Objects;
	// The actively selected object
	TWeakObjectPtr<> ActiveObject;
	// Indicates if the value has been committed, is still pending etc
	EValueChangedEventType EventType;
	// Whether the property has been modified externally
	bool bExternalChange;
	// Whether all properties should be updated after the event
	bool bUpdateAllProperties = false;
	// ~Variables

public:
	// Version for normal, internal events
	FBrickPropertyChangedEvent(ABasePlayerController* InPlayer, const FName& InPropertyName, const FString& InFullPropertyName, const TArray<FStructProperty*>& InParentPropertyChain, UObject* InActiveObject, EValueChangedEventType InEventType);

	// Version for external events
	FBrickPropertyChangedEvent(const FName& InPropertyName, UObject* InObject, const bool bUpdateAllProperties)
		: Player(nullptr), PropertyChainDepth(0), EventType(EValueChangedEventType::Commit), bExternalChange(true), bUpdateAllProperties(bUpdateAllProperties)
	{
		Objects.Add(InObject);

		FullPropertyName = InPropertyName.ToString();
		// Build the property chain from the full property name
		TArray<FString> PropertyNameStrings;
		InPropertyName.ToString().ParseIntoArray(PropertyNameStrings, TEXT("."));
		check(PropertyNameStrings.Num());

		PropertyChain.Reserve(PropertyNameStrings.Num());
		for (auto i = 0; i < PropertyNameStrings.Num(); ++i)
		{
			PropertyChain.Add(*PropertyNameStrings[i]);
		}
		ensure(PropertyChain.Num());
	}

	void ReserveObjects(int32 Num)
	{
		Objects.Reserve(Num);
	}

	void AddObject(UObject* Object)
	{
		// IMPORTANT: Need to use AddUnique, since this is potentially called multiple times with the same object on pending values
		Objects.AddUnique(Object);
	}

	int32 GetNumObjects() const
	{
		return Objects.Num();
	}

	UObject* GetObject(int32 Index) const
	{
		return Objects.IsValidIndex(Index) ? Objects[Index].Get() : nullptr;
	}

	UObject* GetActiveObject() const
	{
		return ActiveObject.Get();
	}

	template <class T = ABasePlayerController>
	T* GetPlayerController() const
	{
		return Cast<T>(Player.Get());
	}

	FName GetPropertyName() const
	{
		if (PropertyChain.Num())
		{
			return PropertyChain.Last();
		}

		return NAME_None;
	}

	const TArray<FName>& GetPropertyChain() const
	{
		return PropertyChain;
	}

	void SetPropertyChainDepth(int32 NewDepth) const
	{
		// NOTE: This is a bit hacky, but needed because in FBrickProperty::OnPropertyModified the event is passed as const
		auto& MutableThis = (FBrickPropertyChangedEvent&)*this;
		MutableThis.PropertyChainDepth = NewDepth;
	}

	// Macros used to simplify the syntax
#define IsBrickProp(PropertyName) IsBrickPropInternal(PropertyName, TEXT(#PropertyName))
#define IsExternalBrickProp(PropertyType) IsExternalBrickPropInternal<PropertyType>()

	// Take the property as a parameter to make sure the name is valid
	template <typename PropertyType>
	bool IsBrickPropInternal(const PropertyType& InProperty, const FName& InPropertyName) const
	{
		return IsBrickPropInternal(InPropertyName);
	}

	// Version for external properties
	template <typename PropertyType>
	//typename TEnableIf<TIsDerivedFrom<PropertyType, FBrickProperty>::IsDerived, bool>::Type IsExternalBrickPropInternal() const
	bool IsExternalBrickPropInternal() const
	{
		return IsBrickPropInternal(PropertyType::StaticType());
	}

	bool IsBrickPropInternal(const FName& InPropertyName) const
	{
		// This could legitimatelly fail, so don't assert just return false
		if (PropertyChain.IsValidIndex(PropertyChainDepth))
		{
			return PropertyChain[PropertyChainDepth] == InPropertyName;
		}

		return false;
	}

	// Return whether the modified property is the given action
	bool IsInputMapping(const FName& ActionName) const;

	// Returns true if the modified property is part of a property reflection list
	bool IsPartOfReflection(const FBrickPropertyReflection& Reflection) const;

	// Used to override the event type
	void SetEventType(EValueChangedEventType InEventType)
	{
		EventType = InEventType;
	}

	bool IsValueCommitted() const
	{
		return EventType != EValueChangedEventType::Pending;
	}

	bool IsExternalEvent() const
	{
		return bExternalChange;
	}

	bool ShouldUpdateAllProperties() const
	{
		return bUpdateAllProperties;
	}
};
