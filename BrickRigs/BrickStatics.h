// Copyright Fluppisoft, 2015

#pragma once

#include "CoreMinimal.h"
#include "BrickEditor/BrickEditorSaveVersion.h"
#include "Bricks/Misc/ScalableBrickConnectorSpacing.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Vehicle/BrickConnection.h"
#include "BrickStatics.generated.h"

enum class EFluAxisSigned : uint8;
class UExplosiveMaterial;
class AExplosion;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UBrickStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Returns the project version string
	UFUNCTION(BlueprintPure)
	static FString GetProjectVersion();

	// ~Explosions and Fire
	// Spawns an explosion in the world or merges it into an existing one, server only
	UFUNCTION(BlueprintCallable, Meta = (WorldContext="WorldContextObject"))
	static void SpawnExplosion(UObject* WorldContextObject, TSubclassOf<UExplosiveMaterial> ExplosiveMaterial, float Volume, const FVector& Location, const FRotator& Rotation, APawn* Instigator, AActor* DamageCauser, const TArray<UPrimitiveComponent*>& ComponentsToIgnore);

	// Tries to set fire to an actor, component or instance
	UFUNCTION(BlueprintCallable)
	static bool SetActorOnFire(const FHitResult& Hit, APawn* Instigator, AActor* DamageCauser);
	// Puts out the fire on an object
	UFUNCTION(BlueprintCallable)
	static bool ExtinguishActor(const FHitResult& Hit);
	// Allows objects on fire to spread it to nearby objects
	UFUNCTION(BlueprintCallable)
	static bool SpreadFire(UObject* WorldContextObject, const FTransform& Transform, const FVector& BoundsMin, const FVector& BoundsMax, float SpreadDistance, float SpreadProbability);
	// ~Explosions and Fire

	// ~Physics
	// Returns the sub body instance associated with the item index of a hit or overlap result
	static FBodyInstance* GetBodyInstanceFromItemIndex(UPrimitiveComponent* Comp, int32 Index);

	// Returns the bone name associated with the item index of a hit or overlap result
	static FName GetBoneNameFromItemIndex(UPrimitiveComponent* Comp, int32 Index);

	// Returns the instance index corresponding to the given body index
	static int32 GetInstanceIndexFromBodyIndex(const UInstancedStaticMeshComponent* Comp, const int32 Index);

	// ~Physics

	// ~Arrays
	// Clamps the array to the given maximum size, returns the number of elements removed
	template <typename T>
	static int32 ClampArraySize(TArray<T>& Array, int32 MaxSize, bool bAllowShrinking = true, bool bRemoveFromStart = false)
	{
		const auto NumToRemove = FMath::Clamp(Array.Num() - MaxSize, 0, Array.Num());
		if (NumToRemove > 0)
		{
			Array.RemoveAt(bRemoveFromStart ? 0 : Array.Num() - NumToRemove, NumToRemove, bAllowShrinking);
		}

		return NumToRemove;
	}

	// ~Arrays

	// ~Online
	// Converts a unique net it to string
	UFUNCTION(BlueprintPure)
	static FString UniqueNetIdToString(const FUniqueNetIdRepl& UniqueNetId);

	// Creates a unique net id from string
	UFUNCTION(BlueprintCallable)
	static FUniqueNetIdRepl CreateUniqueNetId(const FName& OSSName, const FString& Str);

	// Returns the unique net id for a player
	UFUNCTION(BlueprintPure)
	static FUniqueNetIdRepl GetPlayerUniqueNetId(const APlayerController* PC);

	// Opens the player profile in the game overlay
	UFUNCTION(BlueprintCallable)
	static bool ShowPlayerProfileUI(const APlayerController* OwnPC, const FUniqueNetIdRepl& PlayerId);
	// ~Online

	// ~Modding
	// Returns true if the asset is part of a plugin/mod
	UFUNCTION(BlueprintPure)
	static bool IsModdedAsset(const UObject* Asset);
	// ~Modding

	// ~Color
	UFUNCTION(BlueprintPure)
	static FString ColorToHex(const FLinearColor& Color, const bool bIncludeAlpha = true)
	{
		auto Hex = Color.ToFColor(true).ToHex();
		// Remove the alpha channel if needed
		if (!bIncludeAlpha)
		{
			Hex = Hex.LeftChop(2);
		}
		return Hex;
	}

	UFUNCTION(BlueprintPure)
	static FLinearColor HexToColor(const FString& Hex)
	{
		return FLinearColor(FColor::FromHex(Hex));
	}

	// ~Color

	// ~Brick Units
	static float BrickUnitsToUnrealUnits(const float Units)
	{
		return Units * 30.f;
	}

	static float UnrealUnitsToBrickUnits(const float Units)
	{
		return Units / 30.f;
	}

	static float SubUnitsToUnrealUnits(const float Units)
	{
		return Units * 10.f;
	}

	static float UnrealUnitsToSubUnits(const float Units)
	{
		return Units / 10.f;
	}

	// Reads units from the legacy 1s notation
	static float BrickUnitsFromLegacyString(FString Str);

	// Reads a brick size vector from the legacy 1x1x1s notation
	static FVector BrickSizeFromLegacyString(const FString& Str);

	// Loads the deprecated brick units type
	static float LoadDeprecatedBrickUnits(const FBrickRigsSaveVersion& Version, FArchive& Ar);

	// Loads the deprecated brick size type
	static FVector LoadDeprecatedBrickSize(const FBrickRigsSaveVersion& Version, FArchive& Ar);
	// ~Brick Units

	// ~Connector Spacing
	UFUNCTION(BlueprintPure)
	static EConnectorSpacing GetScalableBrickConnectorSpacingAxis(const FScalableBrickConnectorSpacing& ConnectorSpacing, const EFluAxisSigned Axis);

	UFUNCTION(BlueprintCallable)
	static void SetScalableBrickConnectorSpacingAxis(UPARAM(Ref) FScalableBrickConnectorSpacing& ConnectorSpacing, const EFluAxisSigned Axis, const EConnectorSpacing NewSpacing);
	// ~Connector Spacing
};
