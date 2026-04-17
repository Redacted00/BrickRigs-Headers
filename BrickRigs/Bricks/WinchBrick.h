// Copyright Fluppisoft, 2016

#pragma once

#include "Components/DistanceConstraint.h"
#include "Bricks/Brick.h"
#include "WinchBrick.generated.h"

class USplineMeshComponent;
class IWinchBrickInterface;

USTRUCT()
struct FWinchAttachTarget
{
	GENERATED_BODY()
	;

	// ~Constructor
	FWinchAttachTarget(UPrimitiveComponent* InComponent = nullptr, int32 InItem = INDEX_NONE, const FVector& InLocation = FVector::ZeroVector)
		: bIsAttached(InComponent != nullptr), Component(InComponent), Item(InItem + 1), RelativeLocation(InLocation)
	{
	}

	bool operator==(const FWinchAttachTarget& Other) const
	{
		return Component == Other.Component
			&& bIsAttached == Other.bIsAttached
			&& Item == Other.Item
			&& RelativeLocation == Other.RelativeLocation;
	}

	bool operator!=(const FWinchAttachTarget& Other) const
	{
		return !(*this == Other);
	}

	bool IsAttached() const
	{
		return bIsAttached;
	}

	auto* GetComponent() const
	{
		return Component.Get();
	}

	auto* GetInterface() const;

	const auto& GetRelativeLocation() const
	{
		return RelativeLocation;
	}

	FName GetBoneName() const;

	TOptional<FVector> GetWorldLocation() const;

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
	{
		Ar << bIsAttached;

		// Only replicate while attached
		if (bIsAttached)
		{
			Ar << Component;
			Ar << Item;
			RelativeLocation.NetSerialize(Ar, Map, bOutSuccess);
		}
		else if (Ar.IsLoading())
		{
			Component.Reset();
		}

		return !Ar.IsError();
	}

private:
	// Whether a component has been set
	bool bIsAttached;
	// The component to attach to
	TWeakObjectPtr<UPrimitiveComponent> Component;
	// The item i.e. instance index
	uint16 Item;
	// If a component is set this is the relative location, otherwise absolute
	FVector_NetQuantize100 RelativeLocation;
};

template <>
struct TStructOpsTypeTraits<FWinchAttachTarget> : public TStructOpsTypeTraitsBase2<FWinchAttachTarget>
{
	enum
	{
		// NOTE: Can't be shared because of the component pointer
		WithIdenticalViaEquality = true,
		WithNetSerializer = true
	};
};

UCLASS(Abstract)
class BRICKRIGS_API UWinchBrickStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Winch)
	UStaticMesh* HookMesh;
	UPROPERTY(EditDefaultsOnly, Category = Winch)
	UStaticMesh* RopeMesh;
	UPROPERTY(EditDefaultsOnly, Category = Winch)
	float MinRopeLength;
	UPROPERTY(EditDefaultsOnly, Category = Winch)
	float MaxRopeLength;
	UPROPERTY(EditDefaultsOnly, Category = Winch)
	float WinchSpeed;
	// Cached transform of the hook socket
	UPROPERTY(VisibleDefaultsOnly, Category = Flap)
	FTransform HookSocketTransform;
	// ~Properties

	// ~Constructor
	UWinchBrickStaticInfo();

#if WITH_EDITOR
	virtual void BuildCachedData() override;
#endif
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UWinchBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// The target we are currently attached to
	UPROPERTY(Transient, ReplicatedUsing = OnRep_AttachTarget)
	FWinchAttachTarget AttachTarget;
	UFUNCTION()
	void OnRep_AttachTarget(const FWinchAttachTarget& PrevAttachTarget);
	// The replicated and compressed rope length
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepRopeLength)
	float RepRopeLength;
	UFUNCTION()
	void OnRep_RepRopeLength();
	// The current rope length
	float RopeLength;
	// The rope constraint instance
	TUniquePtr<FDistanceConstraint> RopeConstraint;

	// Current input channel value
	float InputChannelValue;
	// Used to keep track of replicated changes
	int32 ReplicationKey;
	// ~Variables

	// ~Components
	TBrickEditorComponentPtr<UBrickEditorStaticMeshComponent> HookMeshComponent;
	TBrickEditorComponentPtr<USplineMeshComponent> CableComponent;
	TBrickEditorComponentPtr<UInteractionComponent> HookInteractionComponent;
	// ~Components

protected:
	// ~Brick Properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Winch)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Winch)
	float WinchSpeed;
	// ~Brick Properties

public:
	// ~Constructor
	UWinchBrick();

	// ~Super Interface
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void PostConstructVehicle() override;
	virtual void UpdateCustomDepth(bool bEnable, uint8 Stencil) override;

	virtual bool ShouldReplicate() const override
	{
		return true;
	}

	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;
	virtual void RepairBrick() override;

	virtual bool HasAnyInputChannel() const override
	{
		return true;
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual void OnBrickPartRootChanged() override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	// ~Super Interface

	// Whether the winch is currently attached to anything
	bool IsWinchAttached() const
	{
		return AttachTarget.IsAttached();
	}

	// Whether the winch can be attached to the given target
	bool CanAttachWinch(const FWinchAttachTarget& NewTarget, float& OutRopeLength) const;
	// Try to attach the hook to a certain location, server only
	bool AttachWinch(FWinchAttachTarget NewTarget);
	// Detaches the winch from whatever it is attached to
	bool DetachWinch(bool bIsBeingDestroyed = false);
	// Called from the attach target interface
	void OnAttachTargetAttachmentChanged();

private:
	// Get the current world transform of the hook socket
	FTransform GetHookSocketTransform() const;
	// Get the world location of the rope socket
	FVector GetRopeSocketLocation() const;
	// Changes the attach component and location
	void SetAttachTarget(const FWinchAttachTarget& Target, bool bIsBeingDestroyed = false);
	// Called whenever the attach target has changed
	void OnAttachTargetChanged(const FWinchAttachTarget& PrevAttachTarget, bool bIsBeingDestroyed = false);
	// Sets up the initial rope length
	void InitializeRopeLength();
	// Helper function to set the rope length and update the cable etc.
	void SetRopeLength(float NewLength);
	// To be called whenever the rope length has changed
	void OnRopeLengthChanged();
	// Updates the constraint
	void UpdateConstrainedComponents();
	// Updates the maximum distance of the constraint
	void UpdateConstraintLimit();
	// Get the offset of the hook socket to its origin
	FVector GetHookAttachmentOffset() const;
	// Whether the hook should currently be visible
	bool ShouldHookBeVisible() const;
	// Updates hook visibility if needed
	void UpdateHookVisibility() const;

	// Callbacks for the hook interaction component
	void GetHookInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetHookInteractionLocation() const;
	// Interaction callbacks
	UFUNCTION()
	void Interact_AttachWinch(ABrickPlayerController* OtherPC);
	UFUNCTION()
	void Interact_DetachWinch(ABrickPlayerController* OtherPC);
	// Get the closest target we can attach to
	FWinchAttachTarget GetAttachWinchTarget() const;
};

// This interface can be implemented on actors to allow attaching winches
UINTERFACE()
class BRICKRIGS_API UWinchBrickInterface : public UInterface
{
	GENERATED_BODY()
};

class IWinchBrickInterface
{
	GENERATED_IINTERFACE_BODY()

private:
	// NOTE: We need this extra struct since we can't use the interface destructor
	struct FAttachedWinches
	{
		TArray<TWeakObjectPtr<UWinchBrick>> Array;

		// ~Destructor
		~FAttachedWinches()
		{
			DetachAllWinches();
		}

		void DetachAllWinches();
	};

	// ~Variables
	// List of all currently attached winches
	FAttachedWinches AttachedWinches;
	// ~Variables

public:
	// Returns whether a winch can be attached to the given hit
	virtual bool CanAttachWinch(UPrimitiveComponent* Comp) const
	{
		return true;
	}

	// Adds a new winch to the list
	void AddWinchBrick(UWinchBrick* Winch)
	{
		AttachedWinches.Array.AddUnique(Winch);
	}

	// Removes a winch from the list
	void RemoveWinchBrick(UWinchBrick* Winch)
	{
		AttachedWinches.Array.RemoveSingleSwap(Winch);
	}

	// Detaches all winches from the actor, server only
	void DetachAllWinches();
	// Updates the attachment of all attached winches
	void UpdateAllWinchAttachments();
};
