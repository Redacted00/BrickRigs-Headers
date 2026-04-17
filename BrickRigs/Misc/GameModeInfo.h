// Copyright Fluppisoft, 2020

#pragma once

#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "GameModeInfo.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UGameModeInfo : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	// The game mode reference
	UPROPERTY(EditDefaultsOnly, Category = GameMode, meta = (MetaClass = "GameModeBase"))
	FSoftClassPath GameModeClass;
	// Name of the game mode displayed in the UI
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	FText DisplayName;
	// Higher numbers are displayed first
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	int32 SortPriority;
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	FText DescriptionText;
	// The short alias and prefix used for game mode specific levels
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	FString Alias;
	// Whether this game mode requires levels to have a sub level with the alias as a prefix to be supported
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bRequiresPrefixedSubLevel = true;
	// Whether the round duration can be used for this mode
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bUsesRoundDuration = true;
	// Whether the mode is round based
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bIsRoundBased;
	// Whether infinite money is enabled for the mode
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bUsesInfiniteMoney;
	// Whether sandbox settings can be used for this mode
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bUsesSandboxSettings;
	// Whether this mode is meant for singleplayer
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bSupportsSingleplayer = true;
	// Whether this mode is meant for multiplayer
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	bool bSupportsMultiplayer = true;
	// ~Properties

	// Used for sorting
	bool operator<(const UGameModeInfo& Other) const
	{
		if (SortPriority != Other.SortPriority)
		{
			return SortPriority > Other.SortPriority;
		}

		return GetName() < Other.GetName();
	}

	// Returns the user friendly name
	FText GetDisplayName() const
	{
		return DisplayName;
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

#define LOCTEXT_NAMESPACE "GameMode"

	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override
	{
		OutContent.TitleText = GetDisplayName();
		OutContent.AddBulletPoint(DescriptionText);
		if (bSupportsSingleplayer)
		{
			OutContent.AddBulletPoint(LOCTEXT("Singleplayer", "Singleplayer"));
		}
		if (bSupportsMultiplayer)
		{
			OutContent.AddBulletPoint(LOCTEXT("Multiplayer", "Multiplayer"));
		}
		if (bIsRoundBased)
		{
			OutContent.AddBulletPoint(LOCTEXT("RoundBased", "Round Based"));
		}
		return true;
	}
#undef LOCTEXT_NAMESPACE

	// ~IObjectPropertyItemInterface
};
