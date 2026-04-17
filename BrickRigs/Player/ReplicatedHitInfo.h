#pragma once

#include "BrickStatics.h"
#include "Projectiles/AmmoType.h"
#include "CoreMinimal.h"
#include "ReplicatedHitInfo.generated.h"

class UFirearmComponent;

/**
* This struct is responsible for replicating crucial hit information from clients to the server.
* A sub struct is used for projectile hits, this base class can be used for more simple hit events such as punching
*/
USTRUCT()
struct FRepHitInfo
{
	GENERATED_BODY()

public:
	// ~Variables
	TWeakObjectPtr<AActor> HitActor;
	TWeakObjectPtr<UPrimitiveComponent> HitComponent;
	FVector_NetQuantizeNormal ShotDirection;
	FVector_NetQuantize ImpactPoint;
	FVector_NetQuantizeNormal ImpactNormal;

private:
	// This is stored as the item index + 1, so invalid items are 0
	uint16 HitItem;
	// ~Variables

public:
	// ~Constructor
	FRepHitInfo(const FHitResult& Hit = FHitResult(1.f), const FVector& InShotDirection = FVector::ZeroVector)
	{
		ensure(Hit.Item < MAX_uint16 + 1);
		HitActor = Hit.GetActor();
		HitComponent = Hit.GetComponent();
		HitItem = Hit.Item + 1;
		ShotDirection = InShotDirection;
		ImpactPoint = FRepMovement::RebaseOntoZeroOrigin(Hit.ImpactPoint, HitActor.Get());
		ImpactNormal = Hit.ImpactNormal;
	}

	FHitResult ToHitResult() const
	{
		FHitResult Hit;
		Hit.Actor = HitActor;
		Hit.Component = HitComponent;
		Hit.Item = GetHitItem();
		Hit.ImpactPoint = FRepMovement::RebaseOntoLocalOrigin(ImpactPoint, HitActor.Get());
		Hit.Normal = ImpactNormal;
		Hit.Location = Hit.ImpactPoint;
		Hit.BoneName = UBrickStatics::GetBoneNameFromBodyIndex(HitComponent.Get(), Hit.Item);
		return Hit;
	}

	int32 GetHitItem() const
	{
		return HitItem - 1;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << HitActor;
		Ar << HitComponent;
		Ar << HitItem;
		ImpactPoint.NetSerialize(Ar, Map, bOutSuccess);
		ImpactNormal.NetSerialize(Ar, Map, bOutSuccess);
		ShotDirection.NetSerialize(Ar, Map, bOutSuccess);

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FRepHitInfo> : public TStructOpsTypeTraitsBase2<FRepHitInfo>
{
	enum
	{
		WithNetSerializer = true
		// NOTE: This can't have WithNetSharedSerialization, since object pointers are being replicated
	};
};


/**
* This struct is used to replicate projectile hits to the server
*/
USTRUCT()
struct FProjectileHitInfo : public FRepHitInfo
{
	GENERATED_BODY()
	;

public:
	// The firearm that spawned the projectile
	TWeakObjectPtr<UFirearmComponent> FirearmComponent;
	// The ammo type that has been fired
	EAmmoType AmmoType;
	// Number of projectiles merged
	uint8 NumMergedProjectiles;
	// The distance the projectile has travelled
	float DistanceTravelled;

	FProjectileHitInfo(UFirearmComponent* InFirearmComponent = nullptr, EAmmoType InAmmoType = EAmmoType::Default, uint8 InNumMerged = 0, float InDistanceTravelled = 0.f, const FHitResult& InHit = FHitResult(1.f), const FVector& InShotDirection = FVector::ZeroVector)
		: Super(InHit, InShotDirection)
	{
		FirearmComponent = InFirearmComponent;
		AmmoType = InAmmoType;
		NumMergedProjectiles = InNumMerged;
		DistanceTravelled = InDistanceTravelled;
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		Super::NetSerialize(Ar, Map, bOutSuccess);
		Ar << FirearmComponent;
		Ar << AmmoType;
		Ar << NumMergedProjectiles;
		Ar << DistanceTravelled;

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FProjectileHitInfo> : public TStructOpsTypeTraitsBase2<FProjectileHitInfo>
{
	enum
	{
		WithNetSerializer = true,
		// NOTE: This can't have WithNetSharedSerialization, since object pointers are being replicated
	};
};
