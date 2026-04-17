// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Menu/Popups/PopupParams.h"
#include "UI/Input/InputActionConflict.h"
#include "UI/Input/InputActionAndAxisMapping.h"
#include "GameFramework/PlayerInput.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "InputMappingPropertyWidget.generated.h"

class UBrickButtonWidget;
class UInputChordWidget;
class UBrickButtonWidget;
class UPopupWidget;
struct FInputTableRow;

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputMappingKeyWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	DECLARE_DELEGATE_OneParam(FOnClickedRebind, UInputMappingKeyWidget*);

	// ~Variables
	// The mapping associated with this widget
	FInputActionAndAxisMapping InputMapping;
	// Whether the key is currently conflicted
	bool bIsConflicted;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UInputChordWidget* InputChordWidget;
	// ~Widgets

public:
	// ~Delegates
	FOnClickedRebind OnClickedRebindDelegate;
	// ~Delegates

	// ~Super Interface
	virtual bool Initialize() override;
	// ~Super Interface

	// Sets the key and initializes the widget
	void InitializeMapping(const FInputActionAndAxisMapping& InMapping);
	// Returns the associated mapping
	const FInputActionAndAxisMapping& GetInputMapping() const;
	// Updates the key conflicted state
	void SetIsConflicted(bool bNewConflicted);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateConflictedState(bool bNewConflicted);
	// ~Blueprint Interface

private:
	// Calls the blueprint version with correct parameters
	void UpdateConflictedState();
	UFUNCTION()
	void OnClickedButton();
};

/**
 *
 */
UCLASS(Abstract)
class BRICKRIGS_API UInputMappingPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

	// ~Variables
	FPopupHandle PopupHandle_BindKey;
	// Whether a new mapping can currently be added
	bool bCanAddMapping;
	UPROPERTY(Transient)
	TArray<UInputMappingKeyWidget*> KeyWidgets;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	TSubclassOf<UInputMappingKeyWidget> KeyWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	TSoftClassPtr<UPopupWidget> BindKeyPopupClass;
	UPROPERTY(EditDefaultsOnly, Category = InputMapping)
	int32 MaxInputMappings;
	// ~Properties

public:
	// ~Constructor
	UInputMappingPropertyWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual bool Initialize() override;
	virtual void InitializeProperty() override;
	virtual void UninitializeProperty() override;
	virtual void UpdateProperty(const TSharedRef<FBrickPropertyEditInfo>& InPropertyInfo, const FBrickPropertyChangedEvent* ChangedEvent) override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	// ~Super Interface

	// Get the name of the associated action
	FName GetActionName() const;
	// Whether this mapping belongs to a negative axis
	bool IsNegativeAxisMapping() const;

	// Opens the bind key popup to bind a new mapping
	UFUNCTION(BlueprintCallable)
	void AddMapping();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void AddKeyWidget(UInputMappingKeyWidget* Widget, int32 Index);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateCanAddMapping(bool bCanAdd);
	// ~Blueprint Interface

private:
	// Get the input table row from the property handle
	const FInputTableRow* GetInputTableRow() const;

	// Updates the conflicted appearance
	void UpdateConflictedState(const FInputActionConflicts* NewConflicts);
	// Callback for the key widgets
	void OnClickedRebind(UInputMappingKeyWidget* Widget);
	// Opens the bind key popup
	void StartBindKey(UInputMappingKeyWidget* InWidget);
};
