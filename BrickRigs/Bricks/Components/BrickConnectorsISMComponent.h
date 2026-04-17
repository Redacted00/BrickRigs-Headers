// Copyright Fluppisoft, 2021

#pragma once

#include "Vehicle/BrickConnection.h"
#include "UI/BrickUIStyle.h"
#include "CoreMinimal.h"
#include "BrickEditor/Components/BrickEditorComponentInterface.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "BrickConnectorsISMComponent.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickConnectorsISMComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

	// ~Variables
	// Cached pointer to the current UI style
	FBrickUIStyleSafePtr UIStyle;
	// The MID used to reflect the UI style color
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickUIColorStyle ColorStyle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickUIColorStyle FocusedColorStyle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickUIStyleState StyleState;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Arrow)
	EBrickUIStyleState FocusedStyleState;
	// ~Properties

	// ~Constructor
	UBrickConnectorsISMComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	// ~Super Interface

	// Changes the color style and style state
	UFUNCTION(BlueprintCallable)
	void SetStyle(EBrickUIColorStyle NewColorStyle, EBrickUIColorStyle NewFocusedColorStyle, EBrickUIStyleState NewStyleState, EBrickUIStyleState NewFocusedStyleState);

	// Adds a list of new connector fields
	void AddConnectorFields(const TArray<FConnectorField>& InConnectorFields, const uint8 FocusedAxisFlags, const FVector& BoundsSize);
	// Removes all connector field instances
	void ClearConnectorFields();

private:
	// Updates the MID parameters
	void UpdateMID();
	// Callback for the UI style
	void UpdateUIStyle(const UBrickUIStyle* NewStyle);
};

// NOTE: We deliberately don't inherit from the brick editor ISM component, since we don't want the mesh and other settings to be overridden
struct FBrickConnectorsISMComponentParams : FBrickEditorPrimitiveComponentParams
{
	EBrickUIColorStyle ColorStyle;
	EBrickUIColorStyle FocusedColorStyle;
	EBrickUIStyleState StyleState;
	EBrickUIStyleState FocusedStyleState;

	FBrickConnectorsISMComponentParams(const UBrickConnectorsISMComponent* Default = GetDefault<UBrickConnectorsISMComponent>())
		: FBrickEditorPrimitiveComponentParams(Default),
		  ColorStyle(Default->ColorStyle),
		  FocusedColorStyle(Default->FocusedColorStyle),
		  StyleState(Default->StyleState),
		  FocusedStyleState(Default->FocusedStyleState)
	{
	}

	void InitializeComponent(UBrickConnectorsISMComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		Comp->ColorStyle = ColorStyle;
		Comp->FocusedColorStyle = FocusedColorStyle;
		Comp->StyleState = StyleState;
		Comp->FocusedStyleState = FocusedStyleState;
		Comp->ClearInstances();
	}
};
