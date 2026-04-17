// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "Input/BaseInputComponent.h"
#include "UObject/Object.h"
#include "BrickProjectileTypes.generated.h"

class ABrickProjectile;

UINTERFACE()
class BRICKRIGS_API UProjectileSeekingInterface : public UInterface
{
	GENERATED_BODY()
};

class IProjectileSeekingInterface
{
	GENERATED_IINTERFACE_BODY()

private:
	friend ABrickProjectile;
	
	// ~Variables
	// Projectiles currently locking on to this target
	mutable TArray<TWeakObjectPtr<ABrickProjectile>> SeekingProjectiles;
	// ~Variables

	// Removes invalid seeking projectiles
	void RemoveInvalidProjectiles() const;

public:
	// Returns the number of projectiles currently targeting this object
	int32 GetNumSeekingProjectiles() const;

	// Returns the distance of the closest seeking projectile to the target location, or MAX_flt if no projectile is seeking
	float GetClosestSeekingProjectileDistance() const;

	// Can be overridden to customize the heat seeking target location
	virtual FVector GetProjectileSeekingTargetLocation() const;
};


USTRUCT(BlueprintType)
struct FProjectileParams
{
	GENERATED_BODY()

	// The launch speed
	UPROPERTY(EditAnywhere)
	float InitialSpeed = 80000.f;
	// Maximum damage applied before the dropoff starts
	UPROPERTY(EditAnywhere)
	float MaxDamage = 0.2f;
	// Minimum damage applied after the dropoff end
	UPROPERTY(EditAnywhere)
	float MinDamage = 0.1f;
	// Distance at which the damage starts to drop
	UPROPERTY(EditAnywhere)
	float DropoffStart = 2000.f;
	// Distance at which the minimum damage is reached
	UPROPERTY(EditAnywhere)
	float DropoffEnd = 10000.f;
	// Maximum range the projectile can travel
	UPROPERTY(EditAnywhere)
	float MaxRange = 100000.f;

	// Get the damage at the given distance travelled
	float GetDamage(float InDistance) const;
	// Get the distance at which the given damage would be applied
	float GetDistance(float InDamage) const;
};

UCLASS()
class BRICKRIGS_API UProjectileInputComponent : public UBaseInputComponent
{
	GENERATED_BODY()

public:
	// ~Constructor
	UProjectileInputComponent()
	{
		bForceConsumeInput = true;
	}

	// ~Super Interface
	virtual void GetBindInputActionsParams(FBindInputActionsParams& OutParams) const override;
	// ~Super Interface
};