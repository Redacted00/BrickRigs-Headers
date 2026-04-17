// Copyright Fluppisoft, 2015

#pragma once

#include <limits>
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrickStatics.generated.h"

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

	// Conversion
	static float RPMToAngularSpeed(float Val)
	{
		return Val * PI * 2.f / 60.f;
	}

	static float AngularSpeedToRPM(float Val)
	{
		return Val / (PI * 2.f) * 60.f;
	}

	// Calculates angular velocity required to rotate from start to end in the given time
	static FVector CalcAngularVelocity(const FQuat& Start, const FQuat& End, float DeltaTime);

	// Used to calculate the impulse required to accelerate an object to the given speed
	static float CalcStoppingImpulse(float TargetSpeed, const FVector& Normal, const FQuat& Rotation, const FVector& VelocityAtPoint, float Mass, const FVector& MomentOfInertia, const FVector& COMOffset)
	{
		// p = (vp - v - r * w) / (1 / m + r * r / O)
		const auto Speed = VelocityAtPoint | Normal;

		// Make sure to ignore the center of mass offset along the normal
		const auto RotationRadiusSquared = FVector::VectorPlaneProject(COMOffset, Normal).SizeSquared();

		// The axis of rotation we care about
		const auto RotationAxis = (Normal ^ COMOffset).GetSafeNormal();
		float InertiaVar;
		if (RotationAxis.IsZero())
		{
			InertiaVar = 0.f;
		}
		else
		{
			const auto InertiaTensor = Rotation.UnrotateVector(RotationAxis).GetAbs() | MomentOfInertia;
			InertiaVar = RotationRadiusSquared / InertiaTensor;
		}

		return (TargetSpeed - Speed) / (InertiaVar + 1.f / Mass);
	}

	// ~Physics

	// ~Math
	// Converts an FQuat to an FRotator while trying to keep the result as simple as possible for display purposes
	UFUNCTION(BlueprintPure)
	static FRotator QuatToNiceRotator(const FQuat& Quat);

	// Returns the angle of a 2D vector against the unit axis in degrees, input has to be normalized
	UFUNCTION(BlueprintPure)
	static float Vector2DAngle(const FVector2D& Vector);

	// Returns the signed angle between two normalized vectors
	UFUNCTION(BlueprintPure)
	static float Vector2DAngle2(const FVector2D& A, const FVector2D& B);

	// Creates an integer with N leading ones
	template <typename T>
	static constexpr T GenerateIntWithLeadingOnes(T N)
	{
		// Special case when int would overflow from shifting too far left
		if (N >= std::numeric_limits<T>::digits)
		{
			return std::numeric_limits<T>::max();
		}

		return (static_cast<T>(1) << N) - static_cast<T>(1);
	}

	// ~Math

	// ~Arrays
	// Wraps an index to the given number of entries
	UFUNCTION(BlueprintPure)
	static int32 WrapIndex(int32 Value, int32 Num)
	{
		while (Value < 0)
		{
			Value += Num;
		}
		while (Value >= Num)
		{
			Value -= Num;
		}

		return Value;
	}

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
};
