#pragma once

#include "BrickEditor/BrickEditorObjectID.h"
#include "BrickEditor/Components/BrickEditorAudioComponent.h"
#include "BrickEditor/Components/BrickEditorParticleComponent.h"
#include "BrickCollisionEffect.h"
#include "FluidDynamicElement.h"
#include "CoreMinimal.h"

class UBrick;
class UPhysicsConstraintConnection;

#define TEST_BRICKS_WITH_FUEL WITH_EDITOR

// Brick part: A set of bricks that are attached rigidly, i.e. without physics constraints
// Brick cluster: Multiple brick parts that are connected by physics constraints

// This struct is used to store information on cluster roots that are currently on fire
struct FBrickClusterRootFireParams
{
	// Whether the cluster is currently actively on fire
	bool bIsClusterOnFire = false;
	// A list of all cluster bricks currently on fire
	TArray<FBrickEditorObjectID> BricksOnFire;
	// Particle system used for the fire effect
	TBrickEditorComponentPtr<UBrickEditorParticleComponent> FirePSC;
	// Audio component used for the fire effect
	TBrickEditorComponentPtr<UBrickEditorAudioComponent> FireAC;
};

struct FBrickClusterRootParams
{
	// ~Variables
	// All parts inside the brick cluster (except the root part)
	TArray<UBrick*> ChildPartRoots;
	// Instantiated when there are any child bricks on fire
	TUniquePtr<FBrickClusterRootFireParams> FireParams;
	// All previously spawned collision effects
	TArray<FBrickCollisionEffect> CollisionEffects;
	// Maps all currently active sliding effects to their surface type
	TMap<EPhysicalSurface, FBrickSlidingEffect> SlidingEffects;
	// Handle used for the OnPhysDispatchNotifications delegate
	FDelegateHandle DelegateHandle_PhysDispatchNotifications;
	// The total amount of fuel on the cluster, safe to use on the server only
	float FuelLevel = 0.f;
	// The fuel level of the cluster at the time of the last update
	float LastFuelLevel = 0.f;
	// The fuel capacity of all tanks connected to the cluster
	float FuelCapacity = 0.f;
	// Number of bricks in the cluster that have any fuel
	int32 NumBricksWithFuel = 0;
#if TEST_BRICKS_WITH_FUEL
	// List of fuel bricks used to debug errors
	TArray<UBrick*> BricksWithFuel;
#endif
	// Last cluster root this part was attached to
	FBrickEditorObjectID LastClusterRootID = nullptr;
	// Frame when this part was detached
	uint64 LastClusterRootDetachFrame = 0;
	// ~Variables
};

struct FBrickPartRootParams
{
	// ~Variables
	// Struct used on cluster roots to store additional data
	TUniquePtr<FBrickClusterRootParams> ClusterRootParams;
	// All bricks that are attached to the part root
	TArray<UBrick*> Children;
	// List of physics constraint connections that connect to other parts
	TArray<UPhysicsConstraintConnection*> ExternalPartConnections;
	// Stores all fluid dynamic elements of this part
	FFluidDynamicElements FluidDynamicElements;
	// The higher this value the more like this part is to be replicated, 0 means it doesn't need to
	uint16 MovementReplicationPriority = 0;
	// ~Variables
};
