#pragma once

#include "BrickEditor/BrickEditorObjectID.h"
#include "UObject/Object.h"
#include "BrickConnection.generated.h"

class ABrickVehicle;
class UBrick;
class UPhysicsConstraintComponent;

#define BRICK_SIZE 30.f
#define BRICK_HALF_SIZE 15.f
#define BRICK_SUB_SIZE 10.f

UENUM(BlueprintType)
enum class EConnectorDirection : uint8
{
	X,
	Y,
	Z,
	XNeg,
	YNeg,
	ZNeg,
	Max
};

UENUM(BlueprintType)
enum class EConnectorType : uint8
{
	None,
	Default,
	Wheel,
	// NOTE: See IsOmnidirectionalConnectorType
	SphereCoupling,
	Hinge,
	Rotor,
	Muzzle,
	LauncherMuzzle,
	FlatRadialHinge,
	BigHinge,
	Piston,
	LinearCoupling,
	Turntable,
	FlatLinearActuator,
	SmallHinge,
	SmallMuzzle,
	VerticalHinge,
	Hinge3x2,
	SmallTurntable,
	LargeTurntable,
	LargePiston,
	SmallFlatLinearActuator,
	MediumHinge,
	TinyTurntable,
	TinyHinge,
	TinyLinearActuator,
	// NOTE: See IsOmnidirectionalConnectorType
	TinySphereCoupling,
	TinyPiston,
	FlareMuzzle
};

UENUM(BlueprintType)
enum class EConnectorGender : uint8
{
	Male,
	Female,
	Neutral
};

UENUM(BlueprintType)
enum class EConnectorSpacing : uint8
{
	None,
	Default,
	Half,
	Third,
	Double,
	Triple,
	Quadruple,
	Max
};

UENUM(BlueprintType)
enum class EConnectorShape : uint8
{
	Rectangle,
	Triangle,
	FlippedTriangle,
	IsoscelesTriangle,
	Circle,
	HalfCircle,
	QuarterCircle,
	FlippedQuarterCircle,
	InvertedQuarterCircle,
	FlippedInvertedQuarterCircle
};

USTRUCT(BlueprintType)
struct FConnectorField
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	EConnectorType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	EConnectorGender Gender;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	EConnectorDirection Direction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	FVector Origin;
	// Additional offset to direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	int32 NumX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	int32 NumY;
	// Can be used to adjust the default connector spacing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	EConnectorSpacing Spacing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	EConnectorShape Shape;
	// The size of the connector relative to the shape that encloses it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Connector)
	FVector2D ShapeRelativeSize;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	bool bFemale_DEPRECATED = false;
#endif

	// ~Constructor
	FConnectorField(EConnectorType InType = EConnectorType::Default, EConnectorGender InGender = EConnectorGender::Male, EConnectorDirection InDirection = EConnectorDirection::Z)
		: Type(InType)
		  , Gender(InGender)
		  , Direction(InDirection)
		  , Origin(FVector::ZeroVector)
		  , Rotation(FRotator::ZeroRotator)
		  , NumX(1)
		  , NumY(1)
		  , Spacing(EConnectorSpacing::Default)
		  , Shape(EConnectorShape::Rectangle)
		  , ShapeRelativeSize(FVector2D(1.f, 1.f))
	{
	}

	// Useful for calculating the maximum distance of any connector to the origin
	static float CalcMaxConnectorDist(const TArray<FConnectorField>& InConnectors)
	{
		auto MaxConnectorDistSq = 0.f;
		for (const auto& ConnectorField : InConnectors)
		{
			auto CheckExtrema = [&](int32 X, int32 Y)
			{
				const auto ConnectorFieldTM = ConnectorField.GetConnectorTransform();
				const auto ExtremaLocation = ConnectorField.GetNobLocation(ConnectorFieldTM, X, Y);
				MaxConnectorDistSq = FMath::Max(MaxConnectorDistSq, ExtremaLocation.SizeSquared());
			};

			// NOTE: We have to check all corners of the connector, since any one of them could be the farthest away
			CheckExtrema(0, 0);
			if (ConnectorField.NumX > 1)
			{
				CheckExtrema(ConnectorField.NumX - 1, 0);
			}
			if (ConnectorField.NumY > 1)
			{
				CheckExtrema(0, ConnectorField.NumY - 1);
			}
			if (ConnectorField.NumX > 1 && ConnectorField.NumY > 1)
			{
				CheckExtrema(ConnectorField.NumX - 1, ConnectorField.NumY - 1);
			}
		}

		return FMath::Sqrt(MaxConnectorDistSq);
	}

	// Whether the given connector type is a wheel connector
	static bool IsWheelConnectorType(EConnectorType InType)
	{
		return InType == EConnectorType::Wheel || InType == EConnectorType::Rotor;
	}

	// Whether the given connector type is omnidirection
	static bool IsOmnidirectionalConnectorType(EConnectorType InType)
	{
		return InType == EConnectorType::SphereCoupling || InType == EConnectorType::TinySphereCoupling;
	}

	// Returns the strength per connector type
	static float GetConnectorTypeStrength(EConnectorType InType)
	{
		switch (InType)
		{
		case EConnectorType::Default:
			return 0.15f;
		case EConnectorType::Wheel:
			return 1.2f;
		case EConnectorType::SphereCoupling:
			return 0.5f;
		case EConnectorType::Hinge:
			return 0.25f;
		case EConnectorType::Rotor:
			return 4.f;
		case EConnectorType::Muzzle:
			return 0.1f;
		case EConnectorType::FlatRadialHinge:
			return 2.5f;
		case EConnectorType::BigHinge:
			return 0.5f;
		case EConnectorType::Piston:
			return 1.f;
		case EConnectorType::LargePiston:
			return 5.f;
		case EConnectorType::LinearCoupling:
			return 1.f;
		case EConnectorType::Turntable:
			return 2.f;
		case EConnectorType::LargeTurntable:
			return 5.f;
		case EConnectorType::FlatLinearActuator:
			return 4.f;
		case EConnectorType::SmallFlatLinearActuator:
			return 2.f;
		case EConnectorType::SmallHinge:
			return 0.3f;
		case EConnectorType::MediumHinge:
			return 0.5f;
		case EConnectorType::SmallMuzzle:
			return 0.2f;
		case EConnectorType::VerticalHinge:
			return 0.3f;
		case EConnectorType::Hinge3x2:
			return 0.3f;
		case EConnectorType::SmallTurntable:
			return 0.4f;
		case EConnectorType::TinyTurntable:
		case EConnectorType::TinyHinge:
		case EConnectorType::TinyLinearActuator:
		case EConnectorType::TinySphereCoupling:
		case EConnectorType::TinyPiston:
			return 0.2f;
		default:
			return 0.15f;
		}
	}

	// Get the connector spacing in cm
	float GetSpacing() const
	{
		switch (Spacing)
		{
		case EConnectorSpacing::Default:
			return BRICK_SIZE;
		case EConnectorSpacing::Half:
			return BRICK_HALF_SIZE;
		case EConnectorSpacing::Third:
			return BRICK_SUB_SIZE;
		case EConnectorSpacing::Double:
			return BRICK_SIZE * 2;
		case EConnectorSpacing::Triple:
			return BRICK_SIZE * 3;
		case EConnectorSpacing::Quadruple:
			return BRICK_SIZE * 4;
		default:
			return 0.f;
		}
	}

	// Converts the connector direction to an axis
	EAxis::Type GetAxis() const
	{
		switch (Direction)
		{
		case EConnectorDirection::X:
		case EConnectorDirection::XNeg:
			return EAxis::X;
		case EConnectorDirection::Y:
		case EConnectorDirection::YNeg:
			return EAxis::Y;
		case EConnectorDirection::Z:
		case EConnectorDirection::ZNeg:
			return EAxis::Z;
		default:
			return EAxis::None;
		}
	}

	// Returns the axis that should be used for the normal direction
	EAxis::Type GetNormalAxis() const
	{
		switch (Type)
		{
		case EConnectorType::Piston:
		case EConnectorType::LargePiston:
		case EConnectorType::TinyPiston:
			return EAxis::X;
		default:
			return EAxis::Z;
		}
	}

private:
	// Helper used for all kinds of circles
	float EvalCircle(float Val) const
	{
		return FMath::Sqrt(1.f - FMath::Square(1.f - Val * 2.f));
	}

	// Evalulates the shape functiton
	void EvalShape(float Val, FFloatInterval& OutRange) const
	{
		switch (Shape)
		{
		case EConnectorShape::Triangle:
			OutRange.Min = 0.f;
			OutRange.Max = Val;
			break;
		case EConnectorShape::FlippedTriangle:
			OutRange.Min = 0.f;
			OutRange.Max = 1.f - Val;
			break;
		case EConnectorShape::IsoscelesTriangle:
			OutRange.Min = 0.f;
			OutRange.Max = 1.f - FMath::Abs(Val - 0.5f) * 2.f;
			break;
		case EConnectorShape::Circle:
			{
				const auto CircleVal = EvalCircle(Val) * 0.5f;
				OutRange.Min = FMath::Max(0.5f - CircleVal, 0.f);
				OutRange.Max = 0.5f + CircleVal;
				break;
			}
		case EConnectorShape::HalfCircle:
			// NOTE: This is to make sure we always round up and don't have a connector on the negative side, some with other circles
			OutRange.Min = 0.5f + KINDA_SMALL_NUMBER;
			OutRange.Max = EvalCircle(Val) * 0.5f + 0.5f;
			break;
		case EConnectorShape::QuarterCircle:
			OutRange.Min = 0.5f + KINDA_SMALL_NUMBER;
			OutRange.Max = Val >= 0.5f ? EvalCircle(Val) * 0.5f + 0.5 : 0.5f;
			break;
		case EConnectorShape::FlippedQuarterCircle:
			OutRange.Min = 0.5f + KINDA_SMALL_NUMBER;
			OutRange.Max = Val <= 0.5f ? EvalCircle(Val) * 0.5f + 0.5 : 0.5f;
			break;
		case EConnectorShape::InvertedQuarterCircle:
			OutRange.Min = 0.f;
			OutRange.Max = 1.f - FMath::Sqrt(1.f - FMath::Square(Val));
			break;
		case EConnectorShape::FlippedInvertedQuarterCircle:
			OutRange.Min = 0.f;
			OutRange.Max = 1.f - FMath::Sqrt(1.f - FMath::Square(1.f - Val));
			break;
		default:
			OutRange.Min = 0.f;
			OutRange.Max = 1.f;
			break;
		}

		OutRange.Max = FMath::Clamp(OutRange.Max, OutRange.Min, 1.f);
	}

public:
	// Get the min and max X direction nobs at the given Y coordinate
	void GetShapeRange(int32 Y, int32& OutMin, int32& OutMax) const;

	// Get the local space connector transform
	FTransform GetConnectorTransform() const
	{
		return FTransform(GetConnectorRotation(), Origin);
	}

	// Get the relative space rotation of the connector
	FQuat GetConnectorRotation() const
	{
		FVector Normal, RightVector;
		switch (Direction)
		{
		case EConnectorDirection::X:
			Normal = FVector::ForwardVector;
			RightVector = -FVector::RightVector;
			break;
		case EConnectorDirection::Y:
			Normal = FVector::RightVector;
			RightVector = -FVector::ForwardVector;
			break;
		case EConnectorDirection::Z:
			Normal = FVector::UpVector;
			RightVector = FVector::RightVector;
			break;
		case EConnectorDirection::XNeg:
			Normal = -FVector::ForwardVector;
			RightVector = FVector::RightVector;
			break;
		case EConnectorDirection::YNeg:
			Normal = -FVector::RightVector;
			RightVector = FVector::ForwardVector;
			break;
		case EConnectorDirection::ZNeg:
			Normal = -FVector::UpVector;
			RightVector = -FVector::RightVector;
			break;
		}

		// Apply the custom rotation
		if (!Rotation.IsZero())
		{
			Normal = Rotation.RotateVector(Normal);
			RightVector = Rotation.RotateVector(RightVector);
		}

		return FRotationMatrix::MakeFromZY(Normal, RightVector).ToQuat();
	}

	int32 GetNumConnectors() const
	{
		return NumX * NumY;
	}

	// Get the relative position of a nob
	FVector2D GetNobOffset(int32 X, int32 Y) const
	{
		const auto SpacingDist = GetSpacing();
		return FVector2D((X - (NumX - 1) * 0.5f) * SpacingDist, (Y - (NumY - 1) * 0.5f) * SpacingDist);
	}

	// Get the location of a specific nob
	FVector GetNobLocation(const FTransform& Transform, int32 X, int32 Y) const
	{
		const auto Offset = GetNobOffset(X, Y);
		return Transform.GetLocation() + Transform.GetUnitAxis(EAxis::X) * Offset.X + Transform.GetUnitAxis(EAxis::Y) * Offset.Y;
	}

	// Get the transform of a specific nob
	FTransform GetNobTransform(const FTransform& Transform, int32 X, int32 Y) const
	{
		return FTransform(Transform.GetRotation(), GetNobLocation(Transform, X, Y));
	}

	// Gets the planar radius of the connector
	float GetConnectorFieldRadius2D() const
	{
		return GetNobOffset(0, 0).Size();
	}

	// Whether the connector type can connect wheels
	bool IsWheelConnector() const
	{
		return IsWheelConnectorType(Type);
	}

	// Convenience functions to get and set the size by index
	int32 GetNum(int32 Index) const
	{
		return Index == 0 ? NumX : NumY;
	}

	void SetNum(int32 Index, int32 NewNum)
	{
		(Index == 0 ? NumX : NumY) = NewNum;
	}

#if WITH_EDITORONLY_DATA
	void PostSerialize(const FArchive& Ar)
	{
		if (bFemale_DEPRECATED)
		{
			Gender = EConnectorGender::Female;
			bFemale_DEPRECATED = false;
		}
	}
#endif
};

#if WITH_EDITORONLY_DATA
template <>
struct TStructOpsTypeTraits<FConnectorField> : public TStructOpsTypeTraitsBase2<FConnectorField>
{
	enum
	{
		WithPostSerialize = true
	};
};
#endif

// This struct is used to pass along connection parameters more easily
USTRUCT()
struct FBrickConnectionParams
{
	GENERATED_BODY()

	// ~Variables
	// The type of this connection
	EConnectorType Type;
	// Number of sub connectors between the two bricks
	// Each default connector would count as 9 sub connectors for example
	uint16 NumSubConnectors;
	// The connector field indices used by the bricks
	uint16 Brick0ConnectorIndex;
	uint16 Brick1ConnectorIndex;
	// ~Variables

	// ~Constructor
	FBrickConnectionParams()
		: Type(EConnectorType::None), NumSubConnectors(0), Brick0ConnectorIndex(0), Brick1ConnectorIndex(0)
	{
	}

	// Returns a copy of the params with flipped bricks
	FBrickConnectionParams GetFlipped()
	{
		auto OutParams = *this;
		Exchange(OutParams.Brick0ConnectorIndex, OutParams.Brick1ConnectorIndex);
		return OutParams;
	}

	// Resets the params for a new iteration
	void Reset()
	{
		Type = EConnectorType::None;
		NumSubConnectors = 0;
	}

	bool operator==(const FBrickConnectionParams& Other) const
	{
		return Type == Other.Type && NumSubConnectors == Other.NumSubConnectors && Brick0ConnectorIndex == Other.Brick0ConnectorIndex && Brick1ConnectorIndex == Other.Brick1ConnectorIndex;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << Type;
		Ar << NumSubConnectors;
		Ar << Brick0ConnectorIndex;
		Ar << Brick1ConnectorIndex;
		return !Ar.IsError();
	}
};

template <>
struct TStructOpsTypeTraits<FBrickConnectionParams> : public TStructOpsTypeTraitsBase2<FBrickConnectionParams>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

struct FBrickConnectionTollerance
{
private:
	// Maximum distance
	float DistanceTollerance;
	// Distance tollerance but as the size of a vector
	float DistanceTollerance3D;
	// Maximum angle difference in radians
	float AngleTollerance;
	// Factor applied to angles to be comparable to distances
	float AngleScoreWeight;

public:
	// ~Constructor
	// For parameters below 0 the default values will be used instead
	FBrickConnectionTollerance(float InDistance = -1.f, float InAngle = -1.f)
	{
		InDistance = InDistance < 0.f ? 4.f : InDistance;
		InAngle = InAngle < 0.f ? FMath::DegreesToRadians(1.f) : InAngle;

		SetDistanceTollerance(InDistance);
		SetAngleTollerance(InAngle);
		AngleScoreWeight = InAngle > 0.f ? InDistance / InAngle : 0.f;
	}

	void UpdateScore(float NewScore, bool bIgnoreAngle)
	{
		SetDistanceTollerance(NewScore);
		if (!bIgnoreAngle)
		{
			SetAngleTollerance(NewScore / AngleScoreWeight);
		}
	}

	float GetDistanceTollerance() const
	{
		return DistanceTollerance;
	}

	float GetDistanceTollerance3D() const
	{
		return DistanceTollerance3D;
	}

	float GetAngleTollerance() const
	{
		return AngleTollerance;
	}

	// Returns a value that can be compared against other possible connections, lower is closer
	float GetScore(bool bIgnoreAngle)
	{
		return GetScore(DistanceTollerance, AngleTollerance, bIgnoreAngle);
	}

	// Version that allows using external values
	float GetScore(float Dist, float Angle, bool bIgnoreAngle)
	{
		return bIgnoreAngle ? Dist : Dist + Angle * AngleScoreWeight;
	}

private:
	void SetDistanceTollerance(float NewTollerance)
	{
		DistanceTollerance = NewTollerance;
		DistanceTollerance3D = FVector(NewTollerance).Size();
	}

	void SetAngleTollerance(float NewTollerance)
	{
		AngleTollerance = NewTollerance;
	}
};

// Defines a physical connection between two bricks
UCLASS(Within = BrickVehicle)
class UBrickConnection : public UObject
{
	GENERATED_BODY()

protected:
	// ~Variables
	uint16 ConnectionIndex;
	// First brick of this connection
	UPROPERTY(Transient)
	UBrick* Brick0;
	// Second brick of this connection
	UPROPERTY(Transient)
	UBrick* Brick1;
	// These variables contain the IDs of the two bricks, needed for replication
	UPROPERTY(Transient, Replicated)
	FBrickEditorObjectID BrickID0;
	UPROPERTY(Transient, Replicated)
	FBrickEditorObjectID BrickID1;
	// The outer vehicle to the other brick (the vehicle of brick 0 is always the outer of the connection)
	UPROPERTY(Transient, Replicated)
	ABrickVehicle* OtherVehicle;
	// The connection parameters
	UPROPERTY(Transient, Replicated)
	FBrickConnectionParams Params;
	// Damage needed to break the connection
	float BreakingDamage;

	// Whether the connection has been initialized yet
	uint8 bIsInitialized : 1;
	// Whether the connection is a physics constraint connection, used to avoid casts
	uint8 bIsPhysicsConstraint : 1;
	// Whether the connection has been activated
	uint8 bIsActive : 1;
	// Whether this connection is broken yet
	uint8 bIsBroken : 1;
	// Whether the connection has been created after spawning, for example for a coupling
	uint8 bIsDynamic : 1;
	// Used while receiving damage
	uint8 bIsPendingBreak : 1;
	// ~Variables

public:
	// ~Super Interface
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	virtual void PostNetReceive() override;
	virtual void PreDestroyFromReplication() override;
	// ~Super Interface

	// Initializes this connection
	void InitializeConnection(UBrick* InBrick0, UBrick* InBrick1, const FBrickConnectionParams& InParams, const int32 ConnectionIdx, const bool bInIsDynamic);

	// Returns the static connection index
	int32 GetConnectionIndex() const
	{
		return ConnectionIndex;
	}

	// Whether this connection uses a physics constraint
	bool IsPhysicsConstraintConnection() const
	{
		return bIsPhysicsConstraint;
	}

	// Called to activate new or broken connections
	virtual void ActivateConnection();

	// Called when a connection is broken or removed
	virtual void DeactivateConnection();

	// Resets the damage received by the connection
	virtual void RepairConnection();

	// Gets the damage required to break a single connector
	float GetBreakingDamage() const
	{
		return BreakingDamage;
	}

	// Returns the owning vehicle
	ABrickVehicle* GetVehicle() const;

	// Returns the other vehicle
	ABrickVehicle* GetOtherVehicle() const
	{
		return OtherVehicle;
	}

	// Get the first brick involved in the connection
	UBrick* GetBrick0() const
	{
		return Brick0;
	}

	// Get the second brick involved in the connection
	UBrick* GetBrick1() const
	{
		return Brick1;
	}

	// Get a brick by index
	UBrick* GetBrick(bool bBrick1) const
	{
		return bBrick1 ? Brick1 : Brick0;
	}

	// Gets the part root for the given brick
	UBrick* GetBrickPartRoot(bool bBrick1) const;

	// Whether this connection involves the given brick
	bool ContainsBrick(const UBrick* Brick) const
	{
		return Brick == Brick0 || Brick == Brick1;
	}

	// Whether this connection involves the given part root brick
	bool ContainsPartRoot(const UBrick* PartRoot) const
	{
		return GetBrickPartRoot(false) == PartRoot || GetBrickPartRoot(true) == PartRoot;
	}

	// Gets the other brick in this connection
	UBrick* GetOtherBrick(const UBrick* Brick) const
	{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
		ensure(Brick == Brick0 || Brick == Brick1);
#endif

		return Brick == Brick0 ? Brick1 : Brick0;
	}

	// Gets the other part root in this connection
	UBrick* GetOtherBrickPartRoot(const UBrick* PartRoot) const
	{
		auto* PartRoot0 = GetBrickPartRoot(false);
		return PartRoot == PartRoot0 ? GetBrickPartRoot(true) : PartRoot0;
	}

	// Get the associated params
	const FBrickConnectionParams& GetConnectionParams() const
	{
		return Params;
	}

	// Get the connection field index for the given brick
	int32 GetConnectorIndex(const UBrick* Brick) const
	{
		return Brick == Brick0 ? Params.Brick0ConnectorIndex : Params.Brick1ConnectorIndex;
	}

	// Whether the connection is currently active (not broken)
	bool IsActive() const
	{
		return bIsActive;
	}

	// Whether the connection is broken
	bool IsBroken() const
	{
		return bIsBroken;
	}

	// Marks the connection as broken
	void SetIsBroken(bool bNewBroken)
	{
		bIsBroken = bNewBroken;
	}

	// Whether the connection is dynamic
	bool IsDynamic() const
	{
		return bIsDynamic;
	}

	// Get the connector type this connection uses
	EConnectorType GetConnectorType() const
	{
		return Params.Type;
	}

	auto IsPendingBreak() const
	{
		return bIsPendingBreak;
	}

	void SetIsPendingBreak(bool bPending)
	{
		bIsPendingBreak = bPending;
	}

protected:
	// Activates the connection when both bricks are available
	void ActivateReplicatedConnectionIfReady();

	// Callback for the own vehicle
	void OnVehicleConstructed();

	// Callback for the other vehicle
	void OnOtherVehicleConstructed();
};

UCLASS()
class UPhysicsConstraintConnection : public UBrickConnection
{
	GENERATED_BODY()

protected:
	// ~Variables
	// The optional physics constraint used for this connection
	TWeakObjectPtr<UPhysicsConstraintComponent> PhysicsConstraint;
	// Constraint reference transforms saved upon creation
	FTransform Brick0RefTransform;
	FTransform Brick1RefTransform;
	// ~Variables

public:
	// ~Constructor
	UPhysicsConstraintConnection()
		: Super()
	{
		bIsPhysicsConstraint = true;
	}

	// ~Super Interface
	virtual void BeginDestroy() override;
	virtual void ActivateConnection() override;
	virtual void DeactivateConnection() override;
	// ~Super Interface

	// Whether two parts connected by this constraint can replicate their movement as one
	virtual bool CanJoinMovementReplication() const
	{
		return true;
	}

protected:
	// Called after spawning a vehicle if a physics constraint is required
	virtual void CreatePhysicsConstraint();

	// Deactivates and resets the constraint
	void DeactivatePhysicsConstraint();

	// Get the desired relative offset of the constraint, i.e. the pivot point
	virtual void GetConstraintRelativeTransform(FVector& OutLocation, FRotator& OutRotation) const;

	// Calculates the desired reference transform for the constraint
	virtual FTransform CalcConstraintRefFrame(bool bBrick1) const;

	// Calculates the initial ref frame between the brick and the constraint
	FTransform CalcBrickRefTransform(bool bBrick1) const;

public:
	// Updates the ref transform of either brick
	void UpdateBrickRefTransform(bool bBrick1);

	// Called after all connections and parts have been updated to update the constrained components
	void PostUpdateBrickConnections();

	// Called when either part root involved in the connection has started or stopped simulating physics
	void OnAnyPartRootSimulatePhysicsChanged();

	// Updates the current reference frame
	void UpdateConstraintRefFrame(bool bBrick1, bool bTeleportOther = false);

protected:
	// Updates the components involved on the physics constraint
	void UpdateConstrainedComponents(bool bForceUpdate = false);

	// Allows connections to have the other brick be component 1
	virtual bool ShouldFlipConstrainedComponents() const
	{
		return false;
	}

	// Whether the ref transform should be recalculated upon reactivating the constraint
	virtual bool ShouldRecalculateRefTransform() const
	{
		return false;
	}

	// Updates the relative transform of the constraint
	void UpdateConstraintTransform();

	// Callback for the constraint
	UFUNCTION()
	void OnConstraintBroken(int32 ConstraintIndex);
};

UCLASS()
class UWheelConnection : public UPhysicsConstraintConnection
{
	GENERATED_BODY()

	// ~Variables
	float DriveSpeed;
	float ClutchRatio;
	float BrakeRatioOverride;
	float TankDriveBrakeRatio;
	float SteeringRatio;
	// Constants
	float SteeringSpeed;
	// Whether the connected wheel is a left wheel
	uint8 bIsLeftWheel : 1;
	// ~Variables

public:
	// ~Constructor
	UWheelConnection()
	{
		// Start this as not overridden
		BrakeRatioOverride = -1.f;
	}

	// ~Super Interface
	virtual void ActivateConnection() override;
	virtual void RepairConnection() override;
	virtual void CreatePhysicsConstraint() override;
	virtual void GetConstraintRelativeTransform(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual FTransform CalcConstraintRefFrame(bool bBrick1) const override;

	virtual bool ShouldFlipConstrainedComponents() const override
	{
		// NOTE: We have to make the wheel frame 0 and the axle frame 1, so the suspension limits remain relative to the axle rather than the wheel
		return true;
	}

	// ~Super Interface

	// Return true if the wheel drive direction should be inverted
	bool IsLeftWheel() const
	{
		return bIsLeftWheel;
	}

	// Get the current brake ratio applied by the tank drive
	float GetTankDriveBrakeRatio() const
	{
		return TankDriveBrakeRatio;
	}

	// Used by axles to update the wheel connection
	void UpdateWheel(float DeltaTime, float InSteeringRatio, float InBrakeRatio, bool bInHasBrake, bool bInHandBrake, float BrakeStrength, const FVector& WheelVelocity, const TOptional<float>& TrackSpeed);

	// Called from the motor to update the drive params
	void UpdateDrive(float DeltaTime, float InDriveSpeed, float InBrakeRatioOverride, float InClutchRatio, float InTankDriveBrakeRatio);

	// Called from the axle when it became uncontrollable
	void OnBecameUncontrollable();

private:
	// Resets drive variables to zero
	void ResetDriveParams();

	// Sets the initial angular twist limit of the constraint
	void InitAngularTwistLimit();
};

UCLASS()
class UActuatorConnection : public UPhysicsConstraintConnection
{
	GENERATED_BODY()

private:
	// ~Variables
	// Current actuation
	float Actuation;
	// Min and max actuation for this connection
	FFloatInterval ActuationRange;
	// ~Variables

public:
	// ~Super Interface
	virtual void CreatePhysicsConstraint() override;
	virtual FTransform CalcConstraintRefFrame(bool bBrick1) const override;
	virtual bool CanJoinMovementReplication() const override;
	// ~Super Interface

	// Sets the current actuation and moves the actuator
	void SetActuation(float NewActuation);
	// Initializes the actuation range
	void SetActuationRange(const FFloatInterval& NewRange)
	{
		ActuationRange = NewRange;
	}
};

UCLASS()
class UCouplingConnection : public UPhysicsConstraintConnection
{
	GENERATED_BODY()

private:
	// ~Variables
	// The current transition ratio of the coupling animation, range 0-1
	TOptional<float> CouplingTransitionRatio;
	// Offset between the connectors at the time of coupling
	FVector CouplingOffset;
	// ~Variables

public:
	// ~Super Interface
	virtual void ActivateConnection() override;
	virtual void GetConstraintRelativeTransform(FVector& OutLocation, FRotator& OutRotation) const override;

	virtual bool ShouldRecalculateRefTransform() const override
	{
		return true;
	}

	virtual bool CanJoinMovementReplication() const override
	{
		return false;
	}

	// ~Super Interface

	// Return whether the transition animation is still in progress
	bool IsCouplingTransitionInProgress() const
	{
		// NOTE: Don't check for IsActive here, since it's also called during initialization before activation
		return CouplingTransitionRatio.IsSet();
	}

	// Updates the transition animation, returns true when the transition has been finished
	bool UpdateCouplingTransition(float DeltaTime);
};

UCLASS()
class UTurbineConnection : public UPhysicsConstraintConnection
{
	GENERATED_BODY()

public:
	// ~Super Interface
	virtual void CreatePhysicsConstraint() override;
	// ~Super Interface
};
