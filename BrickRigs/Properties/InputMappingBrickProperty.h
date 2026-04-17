#pragma once

#include "UI/Input/InputActionAndAxisMapping.h"
#include "UI/Input/InputTableRow.h"
#include "CoreMinimal.h"
#include "BrickProperty.h"

class ABasePlayerController;

struct FInputMappingBrickProperty : public FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FInputMappingBrickProperty);

	// ~Variables
	// The input table row associated with this mapping
	const FInputTableRow InputTableRow;
	// Whether this mapping represents the negative axis
	const bool bIsNegativeAxisMapping;
	// ~Variables

	// Used to generate an input mapping property name
	static FString GetInputMappingPropertyName(const FName& ActionName)
	{
		return FString::Printf(TEXT("InputMappings.%s"), *ActionName.ToString());
	}

	// Returns the action name used for all actions
	static FName GetAllInputMappingsActionName()
	{
		return "All";
	}

	// ~Constructor
	FInputMappingBrickProperty(const FInputTableRow& InInputTableRow, bool bInIsNegativeAxis)
		: InputTableRow(InInputTableRow), bIsNegativeAxisMapping(bInIsNegativeAxis)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	// ~Super Interface

	const FInputTableRow& GetInputTableRow() const
	{
		return InputTableRow;
	}

	FName GetActionName() const
	{
		return PropertyName;
	}

	bool IsNegativeAxisMapping() const
	{
		return bIsNegativeAxisMapping;
	}
};
