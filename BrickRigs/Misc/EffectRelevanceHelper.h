#pragma once

#include "Sound/SoundBase.h"
#include "CoreMinimal.h"

namespace FEffectRelevanceHelper
{
	FORCEINLINE float GetDistanceToViewerSq(UWorld* World, const FVector& Location)
	{
		float MinDistSquared = MAX_flt;
		for (const FVector& ViewLocation : World->ViewLocationsRenderedLastFrame)
		{
			const float ViewDistSquared = (ViewLocation - Location).SizeSquared();
			MinDistSquared = FMath::Min(ViewDistSquared, MinDistSquared);
		}

		return MinDistSquared;
	}

	FORCEINLINE bool ShouldSpawnVisualEffect(UWorld* World, const FVector& Location, float MaxDistance)
	{
		check(World);

		return GetDistanceToViewerSq(World, Location) < FMath::Square(MaxDistance);
	}

	FORCEINLINE bool GetAudioListenerLocation(UWorld* World, FVector& OutLocation)
	{
		// Only have this execute once a frame to be a bit more performant
		static TFrameValue<FVector> ListenerLocation;
		if (!ListenerLocation.IsSet())
		{
			UGameInstance* GameInstance = World->GetGameInstance();
			if (GameInstance)
			{
				APlayerController* LocalPC = GameInstance->GetFirstLocalPlayerController();
				if (LocalPC)
				{
					FVector NewLocation, FrontDir, RightDir;
					LocalPC->GetAudioListenerPosition(NewLocation, FrontDir, RightDir);
					ListenerLocation = NewLocation;
				}
			}
		}

		if (ListenerLocation.IsSet())
		{
			OutLocation = ListenerLocation.GetValue();
			return true;
		}

		return false;
	}

	FORCEINLINE float GetDistanceToListenerSq(UWorld* World, const FVector& Location)
	{
		FVector ListenerLocation;
		if (GetAudioListenerLocation(World, ListenerLocation))
		{
			return (Location - ListenerLocation).SizeSquared();
		}

		return MAX_flt;
	}

	// Returns whether a sound with a given attenuation radius could be heard at the given location
	FORCEINLINE bool ShouldSpawnSound(UWorld* World, float AttenuationRadius, const FVector& Location)
	{
		const float MaxDistanceSquared = FMath::Square(AttenuationRadius);
		const float DistanceSquared = GetDistanceToListenerSq(World, Location);
		return DistanceSquared < MaxDistanceSquared;
	}

	// Internal version that takes the attenuation settings to apply
	FORCEINLINE bool ShouldSpawnSound(UWorld* World, const FSoundAttenuationSettings& AttenuationSettings, const FVector& Location)
	{
		return ShouldSpawnSound(World, AttenuationSettings.GetMaxDimension(), Location);
	}

	// Version that takes an attenuation settings asset
	FORCEINLINE bool ShouldSpawnSound(UWorld* World, USoundAttenuation* AttenuationSettings, const FVector& Location)
	{
		check(AttenuationSettings);

		return ShouldSpawnSound(World, AttenuationSettings->Attenuation, Location);
	}

	// Returns whether the given sound should be spawned, returns false if it is too far away to be audible
	// Version that takes a sound asset
	FORCEINLINE bool ShouldSpawnSound(UWorld* World, USoundBase* Sound, const FVector& Location)
	{
		if (LIKELY(Sound))
		{
			const auto* AttenuationSettings = Sound->GetAttenuationSettingsToApply();
			if (LIKELY(AttenuationSettings))
			{
				return ShouldSpawnSound(World, *AttenuationSettings, Location);
			}
			UE_LOG(LogTemp, Error, TEXT("FEffectRelevanceHelper::ShouldSpawnSound: Sound %s does not have attenuation settings and won't be spawned!"), *GetNameSafe(Sound));
		}

		return false;
	}
}
