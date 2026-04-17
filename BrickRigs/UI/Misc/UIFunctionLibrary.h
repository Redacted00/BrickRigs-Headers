// Copyright Fluppisoft, 2017

#pragma once

#include "Projectiles/AmmoType.h"
#include "Components/FirearmComponent.h"
#include "NumericValueType.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/Widgets/BrickSliderWidget.h"
#include "UIFunctionLibrary.generated.h"

// Used to expose the FWidgetPath type to Blueprints
USTRUCT(BlueprintType)
struct FWidgetPathWrapper
{
	GENERATED_BODY()

	const FWidgetPath* WidgetPath;

	FWidgetPathWrapper()
		: WidgetPath(nullptr)
	{
	}

	FWidgetPathWrapper(const FWidgetPath& WidgetPath)
		: WidgetPath(&WidgetPath)
	{
	}
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// ~Online
	// Opens an online link in an external web browser
	UFUNCTION(BlueprintCallable)
	static void OpenURLInExternalBrowser(const FString& URL);
	// ~Online

	// ~Text
	// Returns whether the given string is valid, optionally returning a validated string
	static bool ValidateStringLength(FString& InOutString, int32 MaxLength, bool bMultiLine, bool bAddDots = false);
	// Version that takes a text
	static bool ValidateTextLength(FText& InOutText, int32 MaxLength, bool bMultiLine, bool bAddDots = false);
	// Used to get a true/false text for a bool property
	UFUNCTION(BlueprintPure)
	static FText BoolAsText(bool bValue);
	// Converts a timespan to a more user friendly text
	UFUNCTION(BlueprintPure)
	static FText TimespanAsText(const FTimespan& Timespan);
	// ~Text

	// ~Enum Display
	UFUNCTION(BlueprintPure)
	static FText GetFireModeDisplayText(EFireMode InFireMode);
	UFUNCTION(BlueprintPure)
	static FText GetAmmoTypeDisplayText(EAmmoType InAmmoType, bool bLongName);
	UFUNCTION(BlueprintPure)
	static FText GetHUDVisibilityDisplayText(EHUDVisibility InHUDVisibility);
	UFUNCTION(BlueprintPure)
	static FText GetCameraModeDisplayText(ECameraMode InCameraMode);
	// ~Enum Display

	// ~Numeric Values
	// Convert from unreal units to real units
	UFUNCTION(BlueprintPure)
	static float ConvertToRealUnits(float Value, ENumericValueType ValueType)
	{
		return Value * GetUnitConversionFactor(ValueType);
	}

	// Convert back from real units to unreal units
	UFUNCTION(BlueprintPure)
	static float ConvertToUnrealUnits(float Value, ENumericValueType ValueType)
	{
		return Value / GetUnitConversionFactor(ValueType);
	}

	// Get the desired display text for the given value with units text
	UFUNCTION(BlueprintPure)
	static FText NumberToText(float Value, ENumericValueType ValueType, int32 MaxFractionalDigits = 2, bool bIncludeUnits = true, bool bAlwaysSign = false);
	// Get the unit format text
	UFUNCTION(BlueprintPure)
	static FText GetUnitFormat(ENumericValueType ValueType);
	// Returns the delimiter string to use between multidimensional numbers
	UFUNCTION(BlueprintPure)
	static FText GetMultidimensionalNumberDelimiter(const ENumericValueType ValueType);

	// Returns the default slider value step to use for any value type
	static float GetDefaultValueStep(ENumericValueType ValueType);
	// Returns the default slider value range for any value type
	static FFloatInterval GetDefaultValueRange(ENumericValueType ValueType, const FFloatInterval& ValueRange);
	// Returns the default slider grid snap mode to use for any value type
	static EBrickSliderGridSnapMode GetDefaultGridSnapMode(ENumericValueType ValueType);
	// Whether the fast slider mode should be used for the given value type
	static bool ShouldUseFastSliderMode(const ENumericValueType ValueType);

	// Get the scale factor to convert to the given unit (from unreal units)
	static float GetUnitConversionFactor(ENumericValueType ValueType);

	// Converts a brick size to a display text
	UFUNCTION(BlueprintPure)
	static FText BrickSizeToText(const FVector& Size, const bool bZeroAsUndetermined = false);
	// Converts brick units to a display text
	UFUNCTION(BlueprintPure)
	static FText BrickUnitsToText(const float UnrealUnits, const bool bZeroAsUndetermined = false);

private:
	// Resolve the value type to a concrete unit, given the current measurement system
	static ENumericValueType ResolveNumericValueType(ENumericValueType ValueType);
	// ~Numeric Values

public:
	// ~Text Filtering
	// Replaces inappropriate words in the supplied text, returns true if the text was already valid
	UFUNCTION(BlueprintPure)
	static bool FilterTextForProfanity(const FText& InText, FText& OutText);
	// Removes any URLs in the given text
	UFUNCTION(BlueprintPure)
	static bool FilterTextForURLs(const FText& InText, FText& OutText);
	// ~Text Filtering

	// ~Widgets
	// Helper function to traverse the children of a widget and return the first one that supports keyboard focus, or none
	static TSharedPtr<SWidget> GetFirstFocusableDescendant(const TSharedRef<SWidget>& Widget, bool bOnlyEnabled = true);
	// ~Widgets

	// ~FWidgetPathWrapper
	// Whether the given widget is part of the given widget path
	UFUNCTION(BlueprintCallable)
	static bool IsWidgetInWidgetPath(const UWidget* Widget, const FWidgetPathWrapper& WidgetPath);
	// Returns the first widget from the array that is contained in the widget path
	UFUNCTION(BlueprintCallable)
	static UWidget* FindFirstWidgetInWidgetPath(const TArray<UWidget*> Widgets, const FWidgetPathWrapper& WidgetPath);
	// ~FWidgetPathWrapper
};
