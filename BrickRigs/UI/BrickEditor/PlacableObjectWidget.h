// Copyright Fluppisoft, 2020

#pragma once

#include "Input/EditorInputComponent.h"
#include "BrickEditor/BrickEditorObjectFilter.h"
#include "CoreMinimal.h"
#include "UI/Widgets/BrickUserWidget.h"
#include "PlacableObjectWidget.generated.h"

class UBrickEditorWidget;
class UBrickButtonWidget;
class UBrickImage;
class UBrickTextBlock;
class ABrickEditor;

// Input component used for the context menu
UCLASS()
class BRICKRIGS_API UPlacableObjectInputComponent : public UBaseEditorInputComponent
{
	GENERATED_BODY()

private:
	// ~Variables
	// Class of the associated placable
	TWeakObjectPtr<UClass> PlacableClass;
	// The associated filter
	FBrickEditorObjectFilterRef PlacableFilter;
	// ~Variables

public:
	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	virtual void SetContextMenuOwner(UObject* InOwner) override;
	// ~Super Interface

private:
	UFUNCTION()
	void OnPressedSelectByPlacableType();
};

/**
 * This widget is used to display a placable class, for example a brick
 */
UCLASS(Abstract)
class BRICKRIGS_API UPlacableObjectWidget : public UBrickUserWidget
{
	GENERATED_BODY()

private:
	// ~Variabels
	// The owning editor widget
	TWeakObjectPtr<UBrickEditorWidget> BrickEditorWidget;
	// Class of the associated placable
	TWeakObjectPtr<UClass> PlacableClass;
	// The associated filter
	FBrickEditorObjectFilterRef PlacableFilter;
	// Whether this widget is currently displayed as a filter
	uint8 bIsFilterWidget : 1;
	// ~Variabels

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickButtonWidget* Button;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* ThumbnailImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* NameTextBlock;
	// ~Widgets

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual bool NativeGetTooltipContent(const FWeakWidgetPath& EventPath, FTooltipContent& OutContent) override;
	// ~Super Interface

	// Initializes the outer editor
	void SetBrickEditorWidget(UBrickEditorWidget* InWidget);
	// Initializes the widget as a placable or filter
	void InitializePlacable(const FSoftClassPath& InPlacableClass, const FBrickEditorObjectFilterRef& InFilter);
	// Public accessors
	auto* GetPlacableClass() const
	{
		return PlacableClass.Get();
	}

	const auto& GetPlacableFilter() const
	{
		return PlacableFilter;
	}

	// Places the represented object in the editor
	UFUNCTION(BlueprintCallable)
	void OnButtonClicked();
	// Opens the placable context menu
	UFUNCTION(BlueprintCallable)
	void OpenContextMenu();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateIsFilterWidget(bool bNewIsFilter);
	// ~Blueprint Interface
};
