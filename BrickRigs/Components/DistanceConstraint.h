#pragma once

#include "DistanceConstraint.generated.h"

struct FBodyInstance;

namespace physx
{
	class PxDistanceJoint;
	class PxRigidActor;
}

UENUM()
enum class EJointFrame : uint8
{
	Frame0,
	Frame1
};

/**
* Thanks to kylawl for the foundation of this code!
*/
USTRUCT()
struct FDistanceConstraint
{
	GENERATED_BODY()

private:
	// ~Variables
	PxDistanceJoint* ConstraintData;
	// ~Variables

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Joint)
	float MinDistance;
	UPROPERTY(EditAnywhere, Category = Joint)
	float MaxDistance;
	UPROPERTY(EditAnywhere, Category = Joint)
	float SpringStiffness;
	UPROPERTY(EditAnywhere, Category = Joint)
	float SpringDampening;
	UPROPERTY(EditAnywhere, Category = Joint)
	float BreakingForce;

	UPROPERTY(EditAnywhere, Category = Joint)
	float InvMassScale0;
	UPROPERTY(EditAnywhere, Category = Joint)
	float InvMassScale1;

	TWeakObjectPtr<UPrimitiveComponent> Component0;
	FName BoneName0;
	FTransform RefFrame0;
	TWeakObjectPtr<UPrimitiveComponent> Component1;
	FName BoneName1;
	FTransform RefFrame1;
	// ~Properties

	// ~Constructor
	FDistanceConstraint();

	// ~Destructor
	~FDistanceConstraint();

	// Delete copy and move operators
	FDistanceConstraint(const FDistanceConstraint&) = delete;
	const FDistanceConstraint& operator=(const FDistanceConstraint&) = delete;
	FDistanceConstraint(FDistanceConstraint&&) = delete;
	const FDistanceConstraint& operator=(FDistanceConstraint&&) = delete;

	// Updates the constrained components
	void SetConstrainedComponents(UPrimitiveComponent* InComp0, const FName& InBoneName0, const FTransform& InRefFrame0, UPrimitiveComponent* InComp1, const FName& InBoneName1, const FTransform& InRefFrame1);
	// Terminates the constraint
	void TermConstraint();
	// Whether the constraint is currently active and valid
	bool IsValidConstraint() const
	{
		return ConstraintData && !IsBroken();
	}

	// Whether the constraint is currently broken
	bool IsBroken() const;
	// Used to update the min and max distance
	void SetMinDistance(float NewMinDistance);
	void SetMaxDistance(float NewMaxDistance);
	// Changes the inverted mass scale of either component
	void SetInvMassScale(EJointFrame Frame, float NewInvScale);
	// Used to update the breaking force
	void SetBreakingForce(float NewForce);
	// Changes the reference frame of either component
	void SetRefFrame(EJointFrame Frame, const FTransform& RefFrame);
	// Wakes up the components involved
	void WakeRigidBodies();

private:
	// Initializes the constraint instance
	void InitializeConstraint();
};

template <>
struct TStructOpsTypeTraits<FDistanceConstraint> : TStructOpsTypeTraitsBase2<FDistanceConstraint>
{
	enum
	{
		// Can't copy the unique ptr
		WithCopy = false
	};
};
