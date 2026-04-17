// Copyright Fluppisoft, 2017

#pragma once

#include "Bricks/Misc/BrickUnits.h"
#include "Projectiles/AmmoType.h"
#include "Components/FirearmComponent.h"
#include "NumericValueType.h"
#include "Settings/BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIFunctionLibrary.generated.h"

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
	static float ConvertToRealUnits(float Value, ENumericValueType ValueType);
	// Convert back from real units to unreal units
	UFUNCTION(BlueprintPure)
	static float ConvertToUnrealUnits(float Value, ENumericValueType ValueType);
	// Get the desired display text for the given value with units text
	UFUNCTION(BlueprintPure)
	static FText NumberToText(float Value, ENumericValueType ValueType, int32 MaxFractionalDigits = 2, bool bIncludeUnits = true, bool bAlwaysSign = false);
	// Get the unit format text
	UFUNCTION(BlueprintPure)
	static FText GetUnitFormat(ENumericValueType ValueType);
	// Returns the default slider value step to use for any value type
	UFUNCTION(BlueprintPure)
	static float GetDefaultValueStep(ENumericValueType ValueType);

	// Get the scale factor to convert to the given unit (from unreal units)
	static float GetUnitConversionFactor(ENumericValueType ValueType);

private:
	// Returns the current measurement system
	static EMeasurementSystem GetMeasurementSystem();
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
};

FORCEINLINE float UUIFunctionLibrary::ConvertToRealUnits(float Value, ENumericValueType ValueType)
{
	return Value * GetUnitConversionFactor(ValueType);
}

FORCEINLINE float UUIFunctionLibrary::ConvertToUnrealUnits(float Value, ENumericValueType ValueType)
{
	return Value / GetUnitConversionFactor(ValueType);
}

FORCEINLINE FText UUIFunctionLibrary::NumberToText(float Value, ENumericValueType ValueType, int32 MaxFractionalDigits, bool bIncludeUnits, bool bAlwaysSign)
{
	if (ValueType == ENumericValueType::Time)
	{
		const auto TimeFormat = FText::AsCultureInvariant("{0}:{1}");
		const auto RoundedSeconds = FMath::CeilToInt(Value);
		const auto Seconds = RoundedSeconds % 60;
		const auto Minutes = RoundedSeconds / 60 % 60;
		const auto Hours = RoundedSeconds / 3600;
		FNumberFormattingOptions FormattingOptions;
		FormattingOptions.MinimumIntegralDigits = 2;
		// Don't show any fractional digits, important for minutes and hours
		FormattingOptions.MinimumFractionalDigits = 0;

		auto OutText = FText::Format(TimeFormat, FText::AsNumber(Minutes, &FormattingOptions), FText::AsNumber(Seconds, &FormattingOptions));

		// Append the hours
		if (Hours > 0)
		{
			OutText = FText::Format(TimeFormat, FText::AsNumber(Hours, &FormattingOptions), OutText);
		}

		return OutText;
	}
	if (ValueType == ENumericValueType::Gear)
	{
		if (Value == 0.f)
		{
			return FText::AsCultureInvariant("N");
		}
		FNumberFormattingOptions FormattingOptions;
		FormattingOptions.MaximumFractionalDigits = 0;
		auto OutText = FText::AsNumber(FMath::Abs(Value), &FormattingOptions);

		// Append the R prefix for reverse gears
		if (Value < 0.f)
		{
			OutText = FText::Format(FText::AsCultureInvariant("R{0}"), OutText);
		}

		return OutText;
	}
	if (ValueType == ENumericValueType::BrickUnits)
	{
		// Convert to brick units to get the proper display text
		const FBrickUnits BrickUnits(FMath::RoundToInt(Value));
		return BrickUnits.ToText(false);
	}

	// Convert the value
	Value = ConvertToRealUnits(Value, ValueType);

	FNumberFormattingOptions FormattingOptions;
	FormattingOptions.MinimumFractionalDigits = 0;
	FormattingOptions.AlwaysSign = bAlwaysSign;

	if (MaxFractionalDigits >= 0)
	{
		// Explicitly overwrite the fractional digits
		FormattingOptions.MaximumFractionalDigits = MaxFractionalDigits;
	}
	else
	{
		switch (ValueType)
		{
		case ENumericValueType::Money:
			FormattingOptions.MinimumFractionalDigits = 2;
			FormattingOptions.MaximumFractionalDigits = 2;
			break;
		case ENumericValueType::Integer:
		case ENumericValueType::IntZeroAsUnlimited:
		case ENumericValueType::RPM:
		case ENumericValueType::Gear:
		case ENumericValueType::BrickUnits:
		case ENumericValueType::Bricks:
		case ENumericValueType::BricksZeroAsUnlimited:
			FormattingOptions.MinimumFractionalDigits = 0;
			FormattingOptions.MaximumFractionalDigits = 0;
			break;
		default:
			break;
		}
	}

	// Round if no fractional digits are displayed
	if (FormattingOptions.MaximumFractionalDigits == 0)
	{
		Value = FMath::RoundToFloat(Value);
	}

	// Display special types and money as unlimited
	const auto bIsUnlimitedType = ValueType == ENumericValueType::IntZeroAsUnlimited || ValueType == ENumericValueType::BricksZeroAsUnlimited || ValueType == ENumericValueType::DistanceZeroAsUnlimited || ValueType == ENumericValueType::MassZeroAsUnlimited;
	if (bIsUnlimitedType && Value == 0.f || ValueType == ENumericValueType::Money && Value == -1.f)
	{
		return NSLOCTEXT("Misc", "Unlimited", "Unlimited");
	}

	auto OutText = FText::GetEmpty();
	// Make sure the value is valid (might not be from modded save files)
	if (FMath::IsNaN(Value) || !FMath::IsFinite(Value))
	{
		OutText = FText::AsCultureInvariant("Invalid");
	}
	else
	{
		OutText = FText::AsNumber(Value, &FormattingOptions);
	}

	// Append the units if desired
	if (bIncludeUnits)
	{
		const auto UnitsFormat = GetUnitFormat(ValueType);
		OutText = FText::Format(UnitsFormat, OutText);
	}

	return OutText;
}

#define LOCTEXT_NAMESPACE "Units"
FORCEINLINE FText UUIFunctionLibrary::GetUnitFormat(ENumericValueType ValueType)
{
	ValueType = ResolveNumericValueType(ValueType);
	switch (ValueType)
	{
	case ENumericValueType::Percent:
		return FText::AsCultureInvariant("{0}%");
	case ENumericValueType::Angle:
		return FText::AsCultureInvariant(FString::Printf(TEXT("{0}%c"), 0xB0));
	case ENumericValueType::RPM:
		return LOCTEXT("RPM", "{0}RPM");
	case ENumericValueType::Seconds:
		return LOCTEXT("Seconds", "{0}s");
	case ENumericValueType::Minutes:
		return LOCTEXT("Minutes", "{0}m");
	case ENumericValueType::Hours:
		return LOCTEXT("Hours", "{0}h");
	case ENumericValueType::Days:
		return LOCTEXT("Days", "{0}d");
	case ENumericValueType::Money:
		return FText::AsCultureInvariant("${0}");
	case ENumericValueType::SpeedMetric:
		return LOCTEXT("SpeedMetric", "{0}kph");
	case ENumericValueType::SpeedImperial:
		return LOCTEXT("SpeedImperial", "{0}mph");
	case ENumericValueType::DistanceMetric:
		return LOCTEXT("DistanceMetric", "{0}m");
	case ENumericValueType::DistanceImperial:
		return LOCTEXT("DistanceImperial", "{0}ft");
	case ENumericValueType::VolumeMetric:
		return LOCTEXT("VolumeMetric", "{0}L");
	case ENumericValueType::VolumeImperial:
		return LOCTEXT("VolumeImperial", "{0}gal");
	case ENumericValueType::Bricks:
	case ENumericValueType::BricksZeroAsUnlimited:
		return LOCTEXT("Bricks", "{0} Bricks");
	case ENumericValueType::MassMetric:
		return LOCTEXT("MassMetric", "{0}kg");
	case ENumericValueType::MassImperial:
		return LOCTEXT("MassImperial", "{0}lb");
	default:
		return FText::AsCultureInvariant("{0}");
	}
}
#undef LOCTEXT_NAMESPACE

FORCEINLINE float UUIFunctionLibrary::GetDefaultValueStep(ENumericValueType ValueType)
{
	switch (ValueType)
	{
	case ENumericValueType::Percent:
		return 0.05f;
	case ENumericValueType::Angle:
		return 10.f;
	case ENumericValueType::DistanceAuto:
	case ENumericValueType::DistanceMetric:
	case ENumericValueType::DistanceImperial:
		return 30.f;
	default:
		return 1.f;
	}
}

FORCEINLINE float UUIFunctionLibrary::GetUnitConversionFactor(ENumericValueType ValueType)
{
	ValueType = ResolveNumericValueType(ValueType);

	switch (ValueType)
	{
	case ENumericValueType::Percent:
		return 100.f;
	case ENumericValueType::SpeedMetric:
		return 0.036f;
	case ENumericValueType::SpeedImperial:
		return 0.02236936f;
	case ENumericValueType::DistanceMetric:
		return 0.01f;
	case ENumericValueType::DistanceImperial:
		return 0.0328084f;
	case ENumericValueType::VolumeImperial:
		return 0.264172f;
	case ENumericValueType::MassImperial:
		return 2.20462f;
	default:
		return 1.f;
	}
}

FORCEINLINE EMeasurementSystem UUIFunctionLibrary::GetMeasurementSystem()
{
	return UBrickUserSettings::GetUserSettings()->GetMeasurementSystem();
}

FORCEINLINE ENumericValueType UUIFunctionLibrary::ResolveNumericValueType(ENumericValueType ValueType)
{
	// Resolve unlimited types
	switch (ValueType)
	{
	case ENumericValueType::DistanceZeroAsUnlimited:
		ValueType = ENumericValueType::DistanceAuto;
		break;
	case ENumericValueType::MassZeroAsUnlimited:
		ValueType = ENumericValueType::MassAuto;
		break;
	}

	static auto* ValueTypeEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("ENumericValueType"));
	auto EnumName = ValueTypeEnum->GetNameStringByValue(static_cast<int32>(ValueType));

	// Resolve the auto suffix
	const auto AutoSuffix = "Auto";
	if (EnumName.EndsWith(AutoSuffix))
	{
		const auto OutValueType = static_cast<ENumericValueType>(static_cast<int32>(ValueType) + 1 + static_cast<int32>(GetMeasurementSystem()));
#if !UE_BUILD_SHIPPING
		// Do some validation to make sure the correct enum valie exosts
		static auto* MeasurementSystemEnum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EMeasurementSystem"));
		EnumName.RemoveFromEnd(AutoSuffix);
		EnumName += MeasurementSystemEnum->GetNameStringByValue(static_cast<int32>(GetMeasurementSystem()));
		ensure(EnumName == ValueTypeEnum->GetNameStringByValue(static_cast<int32>(OutValueType)));
#endif
		return OutValueType;
	}

	return ValueType;
}
