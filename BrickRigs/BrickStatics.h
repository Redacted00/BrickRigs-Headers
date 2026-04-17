// Copyright Fluppisoft, 2015

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BrickStatics.generated.h"

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
	UFUNCTION(BlueprintCallable)
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
	// Returns the sub body instance at the given index, this is needed because there is no built in way to do this (sub bodies are mostly addressed by name)
	static FBodyInstance* GetBodyInstanceFromIndex(UPrimitiveComponent* Comp, int32 Index);

	// Returns the bone name associated with the body at the given index
	static FName GetBoneNameFromBodyIndex(UPrimitiveComponent* Comp, int32 Index);

	// Conversion
	static float RPMToAngularSpeed(float Val)
	{
		return Val * PI * 2.f / 60.f;
	}

	static float AngularSpeedToRPM(float Val)
	{
		return Val / (PI * 2.f) * 60.f;
	}

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

	// ~Modding
	// Returns true if the asset is part of a plugin/mod
	UFUNCTION(BlueprintPure)
	static bool IsModdedAsset(const UObject* Asset);
	// ~Modding
};
