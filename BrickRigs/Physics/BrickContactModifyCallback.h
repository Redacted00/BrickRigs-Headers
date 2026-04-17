#pragma once

#include "Physics/PhysScene_PhysX.h"
#include "PhysXPublicCore.h"
#include "CoreMinimal.h"
#include "BrickEditor/BrickEditorObjectID.h"
#include "Bricks/WheelBrick.h"

class UBrick;
class UWheelBrick;

class FBrickContactModifyCallback : public FContactModifyCallback
{
	struct FPairComponentInfo
	{
		const FBodyInstance* BodyInstance;
		UPrimitiveComponent* Comp;
	};

	struct FSimulatedComponentInfo
	{
		const FTransform COMTransform;
		const FVector LinVel;
		const FVector AngVel;
		const float InvMass;
		const FVector InvInertiaTensor;
	};

	struct FBrickInitialCollisionPair
	{
		FBrickEditorObjectID Bricks[2];
	};

	// ~Variables
	FCriticalSection Mutex;
	FDelegateHandle DelegateHandle_OnPhysDispatch;

	TArray<FContactModifyParams> PendingContactNotifyInfos;
	// Stores pairs of brick IDs that should currently not collide
	TArray<FBrickInitialCollisionPair> CollisionIgnoreMap;
	TArray<FBrickInitialCollisionPair> PrevCollisionIgnoreMap;
	// ~Variables

public:
	// ~Constructor
	FBrickContactModifyCallback();
	// ~Destructor
	virtual ~FBrickContactModifyCallback() override;

	virtual void onContactModify(PxContactModifyPair* const pairs, PxU32 count) override;

private:
	void OnPhysDispatch(FPhysScene* PhysicsScene);
	// Calculates collision damage for all contacts
	void CalcCollisionDamage(PxContactModifyPair& Pair, TArray<FContactModifyParams>& NewContactNotifyInfos, const FPairComponentInfo CompInfos[]) const;
	// Called to modify contacts when both bricks are on the same vehicle, returns true if the contacts are ignored
	bool ModifyBrickSameVehicleContacts(PxContactModifyPair& Pair, TArray<FBrickInitialCollisionPair>& NewCollisionIgnoreMap, const UBrick* Bricks[], const UWheelBrick* Wheels[]) const;
	// Modifies contacts for wheel bricks
	void ModifyWheelContacts(const UWheelBrick* Wheel, PxContactModifyPair& Pair, uint32 ActorIdx) const;
};

class FBrickContactModifyCallbackFactory : public IContactModifyCallbackFactory
{
public:
	// ~Destructor
	virtual ~FBrickContactModifyCallbackFactory() = default;

	virtual FContactModifyCallback* Create(FPhysScene_PhysX* PhysScene) override
	{
		return new FBrickContactModifyCallback();
	}

	virtual void Destroy(FContactModifyCallback* Callback) override
	{
		delete Callback;
	}
};
