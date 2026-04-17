// Copyright Fluppisoft, 2020

#pragma once

#include "GameModeInfo.h"
#include "Properties/ObjectPropertyItemInterface.h"
#include "CoreMinimal.h"
#include "BrickThumbnailSubsystem.h"
#include "LevelInfo.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API ULevelInfo : public UPrimaryDataAsset, public IObjectPropertyItemInterface
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Level, meta = (AllowedClasses = "World"))
	FSoftObjectPath Level;
	// Runtime generated list of (sub-) levels used for specific game modes
	TArray<FSoftObjectPath> GameModeSpecificLevels;
	// The name to display on the UI
	UPROPERTY(EditDefaultsOnly, Category = Level)
	FText DisplayName;
	// The icon to display on the UI
	UPROPERTY(EditDefaultsOnly, Category = Level)
	TSoftObjectPtr<UTexture2D> DisplayIcon;
	// Higher numbers are displayed first
	UPROPERTY(EditDefaultsOnly, Category = GameMode)
	int32 SortPriority;
	// Whether the level uses world composition, will load game mode specific levels as sub levels in that case
	UPROPERTY(EditDefaultsOnly, Category = Level)
	bool bUsesWorldComposition;
	// Set to true if weather isn't allowed in this level
	UPROPERTY(EditDefaultsOnly, Category = Level)
	bool bNoWeather;
	// Whether AI trains are set up for this level
	UPROPERTY(EditDefaultsOnly, Category = Level)
	bool bHasAITrains;
	// ~Properties

	// Used for sorting
	bool operator<(const ULevelInfo& Other) const
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

	static bool IsGameModeSpecificLevel(const FSoftObjectPath& InLevelPath, const UGameModeInfo* InGameModeInfo)
	{
		const auto LevelName = InLevelPath.GetAssetName();
		const auto GameModePrefix = FString::Printf(TEXT("%s_"), *InGameModeInfo->Alias);
		return LevelName.StartsWith(GameModePrefix);
	}

	FSoftObjectPath FindGameModeSpecificLevel(UGameModeInfo* InGameModeInfo) const
	{
		const auto FoundLevel = GameModeSpecificLevels.FindByPredicate([InGameModeInfo](const auto& OtherLevel) { return IsGameModeSpecificLevel(OtherLevel, InGameModeInfo); });
		return FoundLevel ? *FoundLevel : nullptr;
	}

	bool IsGameModeSupported(UGameModeInfo* InGameModeInfo) const
	{
		// Only compatible if there is at least one game mode specific (sub) level
		return InGameModeInfo && (!InGameModeInfo->bRequiresPrefixedSubLevel || !FindGameModeSpecificLevel(InGameModeInfo).IsNull());
	}

	// ~IObjectPropertyItemInterface
	virtual FDisplayInfo GetDisplayInfo() const override
	{
		return FDisplayInfo(GetDisplayName());
	}

	virtual void GetDisplayThumbnail(FBrickThumbnailRequest& OutRequest) const override
	{
		OutRequest.Texture = DisplayIcon;
		OutRequest.RoundedEdgeRadius = 0.15f;
	}

	virtual bool GetTooltipContent(FTooltipContent& OutContent) const override;
	// ~IObjectPropertyItemInterface
};
