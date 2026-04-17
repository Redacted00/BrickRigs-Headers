// Copyright Fluppisoft, 2016

#pragma once

#include "BrickEditor/Components/BrickEditorTextRenderComponent.h"
#include "Bricks/Brick.h"
#include "NameplateBrick.generated.h"

class UTextRenderComponent;

UCLASS(Abstract)
class BRICKRIGS_API UNameplateBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Text)
	UMaterialInterface* FontMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	UFont* Font;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FVector TextLocation;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FRotator TextRotation;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FVector2D MaxTextSize;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	float DefaultFontSize;
	// ~Properties

	// ~Constructor
	UNameplateBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UNameplateBrick : public UBrick
{
	GENERATED_BODY()

private:
	// ~Components
	// The component used to display the text
	TBrickEditorComponentPtr<UBrickEditorTextRenderComponent> TextRenderComponent;
	// ~Components

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FString Text;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FBrickColor TextColor;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	float FontSize;
	// ~Brick properties

public:
	// ~Constructor
	UNameplateBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults() override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	// ~Super Interface

private:
	// Whether the text should currently be visible
	bool ShouldTextBeVisible() const;
	// Updates text visibility if needed
	void UpdateTextVisibility() const;
	// Updates the displayed text
	void UpdateText();
	// Updates the color of the text
	void UpdateTextColor();

	// Property callbacks
	static FFloatInterval GetFontSizeValueRange(const FBrickPropertyContainer& Container);
};
