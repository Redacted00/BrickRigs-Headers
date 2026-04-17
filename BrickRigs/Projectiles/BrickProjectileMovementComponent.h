// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BrickProjectileMovementComponent.generated.h"

class ABrickProjectile;

/**
 *
 */
UCLASS()
class BRICKRIGS_API UBrickProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

	// ~Variables
	// Total distance the projectile has travelled, NOT affected by penetration
	float DistanceTravelled;
	// Distance at which the projectile should be out of penetration again
	float OutOfPenetrationDistance;
	// Amount of penetration that has already been consumed
	float AbsorbedPenetration;
	// The amount of times the projectile has bounced
	int32 NumBounces;
	// Set to true if the projectile has reached the max distance
	uint8 bExceededMaxDistance : 1;
	// Whether the projectile has ticked at least once
	uint8 bHadInitialTick : 1;
	// ~Variables

public:
	// ~Constructor
	UBrickProjectileMovementComponent();

	// ~Super Interface
	virtual void Activate(bool bReset = false) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual bool MoveUpdatedComponentImpl(const FVector& Delta, const FQuat& NewRotation, bool bSweep, FHitResult* OutHit, ETeleportType Teleport) override;
	virtual bool ShouldUseSubStepping() const override;
	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;
	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;
	virtual bool HandleDeflection(FHitResult& Hit, const FVector& OldVelocity, const uint32 NumBounces, float& SubTickTimeRemaining) override;
	virtual void StopSimulating(const FHitResult& HitResult) override;
	virtual FVector ComputeBounceResult(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;
	virtual FVector ComputeAcceleration(const FVector& InVelocity, float DeltaTime) const override;
	virtual FVector ComputeHomingAcceleration(const FVector& InVelocity, float DeltaTime) const override;
	virtual bool CheckStillInWorld() override;
	// ~Super Interface

	// Get the owning projectile
	ABrickProjectile* GetProjectile() const;

	auto GetDistanceTravelled() const
	{
		return DistanceTravelled;
	}
};
