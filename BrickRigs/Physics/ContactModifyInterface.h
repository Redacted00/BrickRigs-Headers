// Copyright Fluppisoft, 2022

#pragma once

#include "CoreMinimal.h"
#include "Damage/DamageType_Collision.h"
#include "ContactModifyInterface.generated.h"

struct FContactModifyActor
{
	TWeakObjectPtr<UPrimitiveComponent> Comp;
	TWeakObjectPtr<UPhysicalMaterial> PhysMaterial;
	// Index of the body instance
	int32 BodyIndex;
	// Whether PostContactModify should be called for this component
	mutable bool bNotify = false;
};

struct FContactModifyParams
{
	FContactModifyActor Actors[2];
	mutable float DamageAmount[2];
	mutable TOptional<float> AbsorbedDamage;
	FVector HitLocation;
	FVector HitNormal;
	float ImpactSpeed;
	float SlideSpeedSquared;

	FHitResult ToHitResult(int32 ActorIdx) const
	{
		const auto& OtherActor = Actors[1 - ActorIdx];
		auto* OtherComp = OtherActor.Comp.Get();
		auto Hit = FHitResult();
		Hit.Location = HitLocation;
		Hit.Normal = HitNormal;
		Hit.Actor = OtherComp ? OtherComp->GetOwner() : nullptr;
		Hit.Component = OtherComp;
		Hit.Item = OtherActor.BodyIndex;
		Hit.PhysMaterial = OtherActor.PhysMaterial;
		Hit.bBlockingHit = true;
		return Hit;
	}

	FPointDamageEvent ToDamageEvent(int32 ActorIdx) const
	{
		return FPointDamageEvent(DamageAmount[ActorIdx], ToHitResult(ActorIdx), HitNormal, UDamageType_Collision::StaticClass());
	}
};

using FPostContactModifyParams = FContactModifyParams;

UINTERFACE()
class BRICKRIGS_API UContactModifyInterface : public UInterface
{
	GENERATED_BODY()
};

class IContactModifyInterface
{
	GENERATED_IINTERFACE_BODY()
	// Can be implemented to customize contact modification
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx) const = 0;

	// Allows the actor to apply collision damage
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx)
	{
	}
};
