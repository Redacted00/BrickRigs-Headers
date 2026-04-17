// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Settings/MatchSettings.h"
#include "MenuSequenceProperties.generated.h"

USTRUCT(BlueprintType)
struct FMenuSequenceProperties
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0))
	float ProbabilityWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 31))
	int32 PlayOnDayOfMonth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 12))
	int32 PlayDuringMonth;
	// 0 disables the option, 1-7 maps to week days Monday-Sunday
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ClampMin = 0, ClampMax = 7))
	int32 PlayOnDayOfWeek;
	// Optional list from which to randomly chose an override for the world setup params
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWorldSetupParams> WorldSetupParamsOverrides;
	// ~Properties

	// ~Constructor
	FMenuSequenceProperties()
		: ProbabilityWeight(1.f), PlayOnDayOfMonth(0), PlayDuringMonth(0), PlayOnDayOfWeek(0)
	{
	}

	bool CanBePlayed() const
	{
		if (PlayOnDayOfMonth != 0 && PlayOnDayOfMonth != FDateTime::Today().GetDay())
		{
			return false;
		}

		if (PlayDuringMonth != 0 && PlayDuringMonth != FDateTime::Today().GetMonth())
		{
			return false;
		}

		if (PlayOnDayOfWeek != 0 && PlayOnDayOfWeek - 1 != static_cast<int32>(FDateTime::Today().GetDayOfWeek()))
		{
			return false;
		}

		return true;
	}
};

FORCEINLINE void LexFromString(FMenuSequenceProperties& OutConditions, const TCHAR* const String)
{
	FMenuSequenceProperties::StaticStruct()->ImportText(String, &OutConditions, nullptr, 0, nullptr, "");
}
