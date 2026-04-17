#pragma once

#include "BrickEditorObjectBrickProperty.h"
#include "UserIdBrickProperty.h"
#include "StructBrickProperty.h"
#include "ColorBrickProperty.h"
#include "DateTimeBrickProperty.h"
#include "UGCBrickProperty.h"
#include "ObjectBrickProperty.h"
#include "TextBrickProperty.h"
#include "EnumBrickProperty.h"
#include "NumericBrickProperty.h"
#include "BoolBrickProperty.h"
#include "TimespanBrickProperty.h"
#include "BrickPropertyReflection.h"
#include "BrickPropertyChangedEvent.h"
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BrickPropertyInterface.generated.h"

class UBrickEditorObject;
struct FBrickStructPropertyInterface;
enum class EBrickValidityState : uint8;

struct FBrickPropertyFocusEvent : FBrickPropertyInstance
{
private:
	// ~Variables
	const FName SubPropertyName;
	const bool bIsFocused;
	mutable TMap<UBrickEditorObject*, EBrickValidityState> ObjectsToHighlight;
	// ~Variables

public:
	// ~Constructor
	FBrickPropertyFocusEvent(const FBrickPropertyInstance& InPropertyInstance, const FName& InSubPropertyName, bool bInIsFocused)
		: FBrickPropertyInstance(InPropertyInstance), SubPropertyName(InSubPropertyName), bIsFocused(bInIsFocused)
	{
	}

	template <typename PropertyType>
	bool IsBrickPropInternal(const PropertyType& InProperty, const FString& InPropertyName) const
	{
		return IsBrickPropInternal(InPropertyName);
	}

	bool IsBrickPropInternal(const FString& InPropertyName) const
	{
		// Check if the property chain starts with the given property name
		if (FullPropertyName.StartsWith(InPropertyName))
		{
			if (FullPropertyName.Len() == InPropertyName.Len())
			{
				return true;
			}
			if (FullPropertyName.Len() > InPropertyName.Len())
			{
				// Also a match if the next character is a dot
				return FullPropertyName[InPropertyName.Len()] == '.';
			}
		}

		return false;
	}

	const auto& GetSubPropertyName() const
	{
		return SubPropertyName;
	}

	auto IsFocused() const
	{
		return bIsFocused;
	}

	const auto& GetObjectsToHighlight() const
	{
		return ObjectsToHighlight;
	}

	void ReserveObjectsToHighlight(int32 Num) const
	{
		ObjectsToHighlight.Reserve(ObjectsToHighlight.Num() + Num);
	}

	void AddObjectToHighlight(UBrickEditorObject* Object, EBrickValidityState State) const
	{
		ObjectsToHighlight.Add(Object, State);
	}
};

// Interface version for structs, useful to share certain properties between different classes
USTRUCT()
struct FBrickStructPropertyInterface
{
	GENERATED_BODY()

	virtual ~FBrickStructPropertyInterface() = default;

	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const
	{
	}

	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event)
	{
	}
};

UINTERFACE()
class BRICKRIGS_API UBrickPropertyInterface : public UInterface
{
	GENERATED_BODY()
};

class BRICKRIGS_API IBrickPropertyInterface
{
	GENERATED_IINTERFACE_BODY()

public:
	// This function just returns GetClass for the UObject representation, needed so the macros for structs and UObjects can be the same
	UStruct* StaticStruct() const;
	// Return a list of all properties that should be displayed for this object
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const
	{
	}

	// Whether properties can currently be modified
	virtual bool CanModifyBrickProperty() const { return true; }
	// Called when a property has been modified
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event)
	{
	}

	// Called when a property was focused or unfocused
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event)
	{
	}
};
