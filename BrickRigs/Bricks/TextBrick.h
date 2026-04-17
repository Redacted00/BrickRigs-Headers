// Copyright Fluppisoft, 2016

#pragma once

#include "BrickEditor/Components/BrickEditorTextRenderComponent.h"
#include "ScalableBrick.h"
#include "TextBrick.generated.h"

class UTextRenderComponent;
class UBrickFont;

UCLASS(Abstract)
class BRICKRIGS_API UTextBrickStaticInfo : public UScalableBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Text)
	UBrickFont* DefaultFont;
	// ~Properties

	// ~Constructor
	UTextBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UTextBrick : public UScalableBrick
{
	GENERATED_BODY()

	// ~Components
	// The component used to display the text
	TBrickEditorComponentPtr<UBrickEditorTextRenderComponent> TextRenderComponent;
	// ~Components

protected:
	// ~Brick properties
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FString Text;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	UBrickFont* Font;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	float FontSize;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	FBrickColor TextColor;
	UPROPERTY(EditDefaultsOnly, Category = Text)
	float OutlineThickness;
	// ~Brick properties

public:
	// ~Constructor
	UTextBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	// ~Super Interface

private:
	// Updates the displayed text
	void UpdateText();
	// Returns the max allowed font size
	float GetMaxFontSize() const;

	// Property callbacks
	static FFloatInterval GetFontSizeValueRange(const FBrickPropertyContainer& Container);
};
