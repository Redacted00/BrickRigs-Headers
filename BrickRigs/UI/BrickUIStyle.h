// Copyright Fluppisoft, 2019

#pragma once

#include "GenericTeamAgentInterface.h"
#include "Styling/SlateTypes.h"
#include "Properties/ObjectPropertyItemInterface.h"
#include "Misc/BrickUIStyleTypes.h"
#include "CoreMinimal.h"
#include "Misc/FluEnumStatics.h"
#include "BrickUIStyle.generated.h"

/**
 *
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class BRICKRIGS_API UBrickUIStyle : public UObject, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUIStyleChanged, const UBrickUIStyle*);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FOnUIStyleChangedDynamic, const UBrickUIStyle*, Style);

	// ~Delegates
	static FOnUIStyleChanged OnUIStyleChangedDelegate;
	static TWeakObjectPtr<const UClass> CurrentStyle;
	// ~Delegates

public:
	// ~Properties
	// Display name of the UI style
	UPROPERTY(EditDefaultsOnly, Category = Style)
	FText StyleDisplayName;
	// NOTE: Throbber style is declared here, because all BrickImage widgets fall back to this when set to a throbber
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Throbber)
	FSlateBrush ThrobberBrush;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Throbber)
	float ThrobberRotationSpeed;
	UPROPERTY(EditAnywhere, Category = Icons)
	FBrickUIIconAtlas IconAtlas[static_cast<int32>(EBrickUIIconAtlas::Custom)];

	// NOTE: All of these have to be set through functions
	UPROPERTY(VisibleDefaultsOnly, Category = Brushes)
	FBrickUIBrushStyle BrushStyles[static_cast<int32>(EBrickUIBrushStyle::Custom)];
	UPROPERTY(VisibleDefaultsOnly, Category = Color)
	FBrickUIColorStyle ColorStyles[static_cast<int32>(EBrickUIColorStyle::Custom)];
	UPROPERTY(VisibleDefaultsOnly, Category = Text)
	FTextBlockStyle TextStyles[static_cast<int32>(EBrickUITextStyle::Custom)];
	UPROPERTY(VisibleDefaultsOnly, Category = Padding)
	float SpacingStyles[static_cast<int32>(EBrickUISpacingStyle::Custom)];
	UPROPERTY(VisibleDefaultsOnly, Category = Padding)
	FMargin PaddingStyles[static_cast<int32>(EBrickUIPaddingStyle::Custom)];
	// ~Properties

	// This is the main hook used to access the current style, forces the user to register a delegate and listen for changes
	static void GetUIStyle(const FOnUIStyleChanged::FDelegate& Delegate);
	// Templated function pointer version
	template <class UserClass>
	static void GetUIStyle(UserClass* Object, void (UserClass::*Func)(const UBrickUIStyle*))
	{
		GetUIStyle(FOnUIStyleChanged::FDelegate::CreateUObject(Object, Func));
	}

	// Blueprint version
	UFUNCTION(BlueprintCallable)
	static void GetUIStyle(FOnUIStyleChangedDynamic Delegate);
	// Unbinds an object from the UI style delegate
	UFUNCTION(BlueprintCallable)
	static void UnbindUIStyle(UObject* Object);
	// Initializes or updates the current UI style
	static void UpdateUIStyle();
	// Returns the CDO of the current style class, or the CDO of the base class
	static const UBrickUIStyle* GetUIStyleCDOSafe();

	// ~Style Accessors
	// Get the throbber brush
	const FSlateBrush& GetThrobberBrush() const;
	// Returns the desired rotation speed for throbbers
	float GetThrobberRotationSpeed() const;
	// Returns the selected brush
	UFUNCTION(BlueprintPure)
	const FSlateBrush& GetBrush(EBrickUIBrushStyle BrushStyle, EBrickUIStyleState StyleState, const FSlateBrush& Fallback) const;
	// Returns the color for the selected style
	UFUNCTION(BlueprintPure)
	const FLinearColor& GetColor(EBrickUIColorStyle ColorStyle, EBrickUIStyleState StyleState, const FLinearColor& Fallback) const;
	// Returns the associated text style
	UFUNCTION(BlueprintPure)
	const FTextBlockStyle& GetTextStyle(EBrickUITextStyle TextStyle, const FTextBlockStyle& Fallback) const;
	// Templated version used by text blocks, since they don't have a FTextBlockStyle member that could be used as a fallback
	template <typename T, T FTextBlockStyle::* SubVar>
	const T& GetTextStyleMember(EBrickUITextStyle TextStyle, const T& Fallback) const;
	// Returns the associated icon atlas info
	const FBrickUIIconAtlas* GetIconAtlas(EBrickUIIconAtlas InIconAtlas) const;
	// Returns the associated spacing value
	UFUNCTION(BlueprintPure)
	float GetSpacing(EBrickUISpacingStyle SpacingStyle, float Fallback) const;
	// Returns the associated padding value
	UFUNCTION(BlueprintPure)
	FMargin GetPadding(EBrickUIPaddingStyle PaddingStyle, const FMargin& Fallback) const;
	// ~Style Accessors

	// ~Static Helpers
	// Encloses the given text in the given style
	UFUNCTION(BlueprintPure)
	static FText FormatRichTextStyle(EBrickUITextStyle Style, const FText& InText);
	// Converts the style enum to a string
	static FString TextStyleToString(EBrickUITextStyle InStyle);
	// Get the desired layer for the current button state
	static EBrickUIStyleState GetButtonStyleState(bool bIsPressed, bool bIsFocused, bool bIsSelected);
	// Returns the required foreground layer style for the given button state
	static EBrickUIStyleState GetButtonForegroundStyleState(bool bIsPressed, bool bIsFocused, bool bIsSelected);
	// Converts a button style state to a foreground state and vise versa
	UFUNCTION(BlueprintPure)
	static EBrickUIStyleState SwitchButtonStyleState(EBrickUIStyleState StyleState);
	// Get the color style for the given team attitude, for example red for enemies, blue for friendlies
	UFUNCTION(BlueprintPure)
	static EBrickUIColorStyle GetTeamAttitudeColorStyle(TEnumAsByte<ETeamAttitude::Type> TeamAttitude);
	// Inverts positive and negative color style
	UFUNCTION(BlueprintPure)
	static EBrickUIColorStyle InvertTeamAttitudeColorStyle(EBrickUIColorStyle ColorStyle);
	// Get the text style associated with the given team attitude
	UFUNCTION(BlueprintPure)
	static EBrickUITextStyle GetTeamAttitudeTextStyle(TEnumAsByte<ETeamAttitude::Type> TeamAttitude);
	// ~Static Helpers

protected:
	// ~Constructor
	UBrickUIStyle();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
#if WITH_EDITOR
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
#endif
	// ~Super Interface

	UFUNCTION(BlueprintCallable)
	void SetBrush(EBrickUIBrushStyle BrushStyle, EBrickUIStyleState StyleState, const FSlateBrush& InBrush);
	UFUNCTION(BlueprintCallable)
	void SetColor(EBrickUIColorStyle ColorStyle, EBrickUIStyleState StyleState, const FLinearColor& InColor);
	UFUNCTION(BlueprintCallable)
	void SetTextStyle(EBrickUITextStyle TextStyle, const FTextBlockStyle& InTextStyle);
	UFUNCTION(BlueprintCallable)
	void SetIconAtlas(EBrickUIIconAtlas InIconAtlas, FBrickUIIconAtlas InAtlas);
	UFUNCTION(BlueprintCallable)
	void SetSpacingStyle(EBrickUISpacingStyle SpacingStyle, float InSpacing);
	UFUNCTION(BlueprintCallable)
	void SetPaddingStyle(EBrickUIPaddingStyle PaddingStyle, FMargin InPadding);

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void BuildStyle();
	// ~Blueprint Interface

public:
	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(StyleDisplayName);
	}

	// ~IObjectPropertyItemInterface
};

FORCEINLINE const FSlateBrush& UBrickUIStyle::GetThrobberBrush() const
{
	return ThrobberBrush;
}

FORCEINLINE float UBrickUIStyle::GetThrobberRotationSpeed() const
{
	return ThrobberRotationSpeed;
}

FORCEINLINE const FSlateBrush& UBrickUIStyle::GetBrush(EBrickUIBrushStyle BrushStyle, EBrickUIStyleState StyleState, const FSlateBrush& Fallback) const
{
	if (BrushStyle >= EBrickUIBrushStyle::Custom)
	{
		return Fallback;
	}
	return BrushStyles[static_cast<int32>(BrushStyle)].GetBrush(StyleState, Fallback);
}

FORCEINLINE const FLinearColor& UBrickUIStyle::GetColor(EBrickUIColorStyle ColorStyle, EBrickUIStyleState StyleState, const FLinearColor& Fallback) const
{
	if (ColorStyle >= EBrickUIColorStyle::Custom)
	{
		return Fallback;
	}
	return ColorStyles[static_cast<int32>(ColorStyle)].GetColor(StyleState, Fallback);
}

FORCEINLINE const FTextBlockStyle& UBrickUIStyle::GetTextStyle(EBrickUITextStyle TextStyle, const FTextBlockStyle& Fallback) const
{
	if (TextStyle >= EBrickUITextStyle::Custom)
	{
		return Fallback;
	}
	return TextStyles[static_cast<int32>(TextStyle)];
}

FORCEINLINE float UBrickUIStyle::GetSpacing(EBrickUISpacingStyle SpacingStyle, float Fallback) const
{
	if (SpacingStyle >= EBrickUISpacingStyle::Custom)
	{
		return Fallback;
	}
	return SpacingStyles[static_cast<int32>(SpacingStyle)];
}

FORCEINLINE FMargin UBrickUIStyle::GetPadding(EBrickUIPaddingStyle PaddingStyle, const FMargin& Fallback) const
{
	if (PaddingStyle >= EBrickUIPaddingStyle::Custom)
	{
		return Fallback;
	}
	return PaddingStyles[static_cast<int32>(PaddingStyle)];
}

template <typename T, T FTextBlockStyle::* SubVar>
FORCEINLINE const T& UBrickUIStyle::GetTextStyleMember(EBrickUITextStyle TextStyle, const T& Fallback) const
{
	if (TextStyle >= EBrickUITextStyle::Custom)
	{
		return Fallback;
	}
	return TextStyles[static_cast<int32>(TextStyle)].*SubVar;
}

FORCEINLINE const FBrickUIIconAtlas* UBrickUIStyle::GetIconAtlas(EBrickUIIconAtlas InIconAtlas) const
{
	if (InIconAtlas >= EBrickUIIconAtlas::Custom)
	{
		return nullptr;
	}
	return &IconAtlas[static_cast<int32>(InIconAtlas)];
}

FORCEINLINE FText UBrickUIStyle::FormatRichTextStyle(EBrickUITextStyle Style, const FText& InText)
{
	const auto StyleName = TextStyleToString(Style);
	const auto FormatString = FString::Printf(TEXT("<%s>{0}</>"), *StyleName);
	return FText::Format(FText::FromString(FormatString), InText);
}

FORCEINLINE FString UBrickUIStyle::TextStyleToString(EBrickUITextStyle InStyle)
{
	return FFluEnumStatics::ValueToString(InStyle);
}

FORCEINLINE EBrickUIStyleState UBrickUIStyle::GetButtonStyleState(bool bIsPressed, bool bIsFocused, bool bIsSelected)
{
	if (bIsPressed)
	{
		return EBrickUIStyleState::Pressed;
	}
	if (bIsFocused && bIsSelected)
	{
		return EBrickUIStyleState::FocusedSelected;
	}
	if (bIsFocused)
	{
		return EBrickUIStyleState::Focused;
	}
	if (bIsSelected)
	{
		return EBrickUIStyleState::Selected;
	}
	return EBrickUIStyleState::Button;
}

FORCEINLINE EBrickUIStyleState UBrickUIStyle::GetButtonForegroundStyleState(bool bIsPressed, bool bIsFocused, bool bIsSelected)
{
	if (bIsPressed)
	{
		return EBrickUIStyleState::ForegroundPressed;
	}
	if (bIsFocused && bIsSelected)
	{
		return EBrickUIStyleState::ForegroundFocusedSelected;
	}
	if (bIsFocused)
	{
		return EBrickUIStyleState::ForegroundFocused;
	}
	if (bIsSelected)
	{
		return EBrickUIStyleState::ForegroundSelected;
	}
	return EBrickUIStyleState::Foreground;
}

FORCEINLINE EBrickUIStyleState UBrickUIStyle::SwitchButtonStyleState(EBrickUIStyleState StyleState)
{
	switch (StyleState)
	{
	case EBrickUIStyleState::Button:
		return EBrickUIStyleState::Foreground;
	case EBrickUIStyleState::Focused:
		return EBrickUIStyleState::ForegroundFocused;
	case EBrickUIStyleState::Pressed:
		return EBrickUIStyleState::ForegroundPressed;
	case EBrickUIStyleState::Selected:
		return EBrickUIStyleState::ForegroundSelected;
	case EBrickUIStyleState::FocusedSelected:
		return EBrickUIStyleState::ForegroundFocusedSelected;
	case EBrickUIStyleState::Foreground:
		return EBrickUIStyleState::Button;
	case EBrickUIStyleState::ForegroundFocused:
		return EBrickUIStyleState::Focused;
	case EBrickUIStyleState::ForegroundPressed:
		return EBrickUIStyleState::Pressed;
	case EBrickUIStyleState::ForegroundSelected:
		return EBrickUIStyleState::Selected;
	case EBrickUIStyleState::ForegroundFocusedSelected:
		return EBrickUIStyleState::FocusedSelected;
	default:
		return StyleState;
	}
}

FORCEINLINE EBrickUIColorStyle UBrickUIStyle::GetTeamAttitudeColorStyle(TEnumAsByte<ETeamAttitude::Type> TeamAttitude)
{
	switch (TeamAttitude)
	{
	case ETeamAttitude::Friendly:
		return EBrickUIColorStyle::Positive;
	case ETeamAttitude::Hostile:
		return EBrickUIColorStyle::Negative;
	default:
		return EBrickUIColorStyle::Neutral;
	}
}

FORCEINLINE EBrickUIColorStyle UBrickUIStyle::InvertTeamAttitudeColorStyle(EBrickUIColorStyle ColorStyle)
{
	switch (ColorStyle)
	{
	case EBrickUIColorStyle::Positive:
		return EBrickUIColorStyle::Negative;
	case EBrickUIColorStyle::Negative:
		return EBrickUIColorStyle::Positive;
	default:
		return ColorStyle;
	}
}

FORCEINLINE EBrickUITextStyle UBrickUIStyle::GetTeamAttitudeTextStyle(TEnumAsByte<ETeamAttitude::Type> TeamAttitude)
{
	switch (TeamAttitude)
	{
	case ETeamAttitude::Friendly:
		return EBrickUITextStyle::Positive;
	case ETeamAttitude::Hostile:
		return EBrickUITextStyle::Negative;
	default:
		return EBrickUITextStyle::Neutral;
	}
}
