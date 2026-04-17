// Copyright Fluppisoft, 2016

#pragma once

#include "BrickRigsMacros.h"
#include "Components/DestructibleISMComponentInterface.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Physics/ContactModifyInterface.h"
#include "DestructibleISMComponent.generated.h"

class UDestructibleInstancesComponent;

/**
 * The DestructibleISMComponent and DestructibleHISMComponent are used by the DestructibleInstancesComponent class
 */

// Macro used to mirror the overrides for both classes
#define DESTRUCTIBLE_ISM_BODY(...) \
	virtual void ReceiveComponentDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override { \
		ReceiveComponentDamageInternal(DamageAmount, DamageEvent, EventInstigator, DamageCauser); } \
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx) const override { \
		OnContactModifyInternal(Params, ActorIdx); } \
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx) { \
		PostContactModifyInternal(Params, ActorIdx); }

UCLASS()
class BRICKRIGS_API UDestructibleISMComponent : public UInstancedStaticMeshComponent, public IDestructibleISMComponentInterface, public IContactModifyInterface
{
	GENERATED_BODY()

public:
	DESTRUCTIBLE_ISM_BODY()
};

UCLASS()
class BRICKRIGS_API UDestructibleHierarchicalISMComponent : public UHierarchicalInstancedStaticMeshComponent, public IDestructibleISMComponentInterface, public IContactModifyInterface
{
	GENERATED_BODY()

public:
	// ~Constructor
	UDestructibleHierarchicalISMComponent()
	{
		// We want to rebuild the tree manually for better performance
		bAutoRebuildTreeOnInstanceChanges = false;
	}

	DESTRUCTIBLE_ISM_BODY()

#if !BR_BUILD_VANILLA
	// Make sure not too many instances are being added per leaf since that affects culling and LOD accuracy quite a lot
	virtual int32 DesiredInstancesPerLeaf() override
	{
		return FMath::Min(Super::DesiredInstancesPerLeaf(), 4);
	}
#endif
};

#undef DESTRUCTIBLE_ISM_SUPER_INTERFACE
