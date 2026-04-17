#pragma once

#include "UI/Misc/BrickUIStyleTypes.h"
#include "UI/Input/InputActionTriggerType.h"
#include "CoreMinimal.h"
#include "TooltipContent.generated.h"

USTRUCT(BlueprintType)
struct FTooltipContent
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(BlueprintReadWrite)
	FText TitleText;
	UPROPERTY(BlueprintReadWrite)
	FText ContentText;
	// Optional name of the input action to display
	UPROPERTY(BlueprintReadWrite)
	FName ActionName;
	// Trigger type for the input action
	UPROPERTY(BlueprintReadWrite)
	EInputActionTriggerType TriggerType;
	UPROPERTY(BlueprintReadWrite)
	EBrickUIColorStyle ColorStyle;
	// ~Properties

	// ~Constructor
	FTooltipContent(const FText& InTitleText = FText(), const FText& InContentText = FText(), const FName& InActionName = NAME_None, EInputActionTriggerType InTriggerType = EInputActionTriggerType::Press, EBrickUIColorStyle InColorStyle = EBrickUIColorStyle::Default)
		: TitleText(InTitleText), ContentText(InContentText), ActionName(InActionName), TriggerType(InTriggerType), ColorStyle(InColorStyle)
	{
	}

	bool operator==(const FTooltipContent& Other) const
	{
		return TitleText.EqualTo(Other.TitleText) && ContentText.EqualTo(Other.ContentText) && ActionName == Other.ActionName && TriggerType == Other.TriggerType && ColorStyle == Other.ColorStyle;
	}

	bool operator!=(const FTooltipContent& Other) const
	{
		return !(*this == Other);
	}

	bool IsEmpty() const
	{
		return TitleText.IsEmpty() && ContentText.IsEmpty() && ActionName.IsNone();
	}

	// Adds a new line to the text
	void AddNewLine()
	{
		ContentText = FText::Format(FText::AsCultureInvariant("{0}\n"), ContentText);
	}

	// Adds a new line only if there is already text
	void ConditionalAddNewLine()
	{
		if (!ContentText.IsEmpty())
		{
			AddNewLine();
		}
	}

	// Adds a line to the display text with a bullet point
	void AddBulletPoint(const FText& InText)
	{
		ConditionalAddNewLine();
		ContentText = FText::Format(FText::AsCultureInvariant("{0}{1} {2}"), ContentText, FText::AsCultureInvariant(FString::Printf(TEXT("%c"), 0xB7)), InText);
	}

	// Adds a bullet point in the key: value format
	void AddKeyValueBulletPoint(const FText& InKey, const FText& InValue)
	{
		AddBulletPoint(FText::Format(FText::AsCultureInvariant("{0}: {1}"), InKey, InValue));
	}
};
