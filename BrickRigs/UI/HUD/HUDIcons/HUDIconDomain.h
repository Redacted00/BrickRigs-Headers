#pragma once

#include "Settings/BrickUserSettingsTypes.h"
#include "CoreMinimal.h"
#include "HUDIconDomain.generated.h"

UENUM(BlueprintType)
enum class EHUDIconDomain : uint8
{
	Map,
	// In game map
	Spawn,
	// Map on the spawn screen
	SpawnPoints,
	// Spawn points panel on the spawn screen
	HUD // 3D HUD screen
};

// Used to wrap the domain enum as well as other parameters
USTRUCT(BlueprintType)
struct FHUDIconContext
{
	GENERATED_BODY()

protected:
	// ~Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHUDIconDomain Domain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHUDVisibility HUDVisibility;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShowInteraction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrawDistanceScale;
	// ~Variables

public:
	// ~Constructor
	FHUDIconContext(EHUDIconDomain InDomain = EHUDIconDomain::Map, EHUDVisibility InHUDVisibility = EHUDVisibility::Full, bool bInShowInteraction = true, float InDrawDistanceScale = 1.f)
		: Domain(InDomain), HUDVisibility(InHUDVisibility), bShowInteraction(bInShowInteraction), DrawDistanceScale(InDrawDistanceScale)
	{
	}

	bool IsHUD() const
	{
		return Domain == EHUDIconDomain::HUD;
	}

	bool IsMap() const
	{
		return Domain == EHUDIconDomain::Map || Domain == EHUDIconDomain::Spawn;
	}

	bool IsSpawnPoints() const
	{
		return Domain == EHUDIconDomain::SpawnPoints;
	}

	EHUDVisibility GetHUDVisibility() const
	{
		return HUDVisibility;
	}

	bool ShouldShowInteraction() const
	{
		return bShowInteraction;
	}

	float GetDrawDistanceScale() const
	{
		return DrawDistanceScale;
	}
};
