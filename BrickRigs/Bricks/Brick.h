// Copyright Fluppisoft, 2015

#pragma once

#include "Physics/ContactModifyInterface.h"
#include "PhysXPublicCore.h"
#include "Inventory/InventoryComponent.h"
#include "Interaction/InteractionComponent.h"
#include "Misc/FluidDynamicElement.h"
#include "Misc/BrickPartRootParams.h"
#include "Components/BrickStaticMeshComponent.h"
#include "Misc/BrickTickFunction.h"
#include "Misc/BrickDamage.h"
#include "BrickEditor/BrickEditorSnappingOffset.h"
#include "Vehicle/VehicleInputChannel.h"
#include "Misc/BrickPattern.h"
#include "Misc/BrickMaterial.h"
#include "Vehicle/BrickConnection.h"
#include "BrickEditor/BrickEditorObject.h"
#include "Misc/CustomCollisionChannels.h"
#include "Misc/FuelTankParams.h"
#include "UObject/Object.h"
#include "Brick.generated.h"

class UBrickEditorArrowComponent;
struct FFuelTankParams;
// Actors
class ABrickVehicle;
class ABrickCharacter;
class ABrickEditor;
class ABrickWorldSettings;
// Components
class UBrickVehicleComponent;
class UBrickConnectorsISMComponent;
// Bricks
class UMotorBrick;
class USeatBrick;
// Misc
class UBrickConnection;
class UBrickVehicleStaticInfo;
class UBrickMaterial;
class UBrickPhysicalMaterial;

#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
static auto GVisualizeFluidDynamics = 0;
static auto GDisableDetachedBrickCollision = 0;
#endif

USTRUCT()
struct FBrickEditorParams : public FBrickEditorObjectEditorParams
{
	GENERATED_BODY()

	// Used to visualize connector fields in the editor
	TBrickEditorComponentPtr<UBrickConnectorsISMComponent> ConnectorsISMComponent;
	// Whether a property is currently being modified
	bool bIsPropertyChangePending = false;
};

UCLASS(Abstract)
class BRICKRIGS_API UBrickStaticInfo : public UBrickEditorObjectStaticInfo
{
	GENERATED_BODY()

public:
	// ~Variables
	// Radius of the lift surface, if there is any
	UPROPERTY()
	float LiftSurfaceRadius;
	// Local center position of the lift surface
	UPROPERTY()
	FVector LiftSurfaceLocation;
	// Direction of the lift surface
	UPROPERTY()
	FVector LiftSurfaceNormal;
	// ~Variables

	// ~Properties
	// The static mesh to use as the bricks root component
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMesh* StaticMesh;
	// The brick material used by default
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UBrickMaterial* DefaultBrickMaterial;
	// Default opaque material
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UMaterialInterface* DefaultMaterial;
	// Material to use when translucency is required
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UMaterialInterface* TranslucentMaterial;
	// Material to use when hidden in the editor
	UPROPERTY(EditAnywhere, Category = Mesh)
	UMaterialInterface* HiddenMaterial;
	// Material to use for the editor physics visualization view mode
	UPROPERTY(EditAnywhere, Category = Mesh)
	UMaterialInterface* PhysicsVisualizationMaterial;

	// The brick color to use when rendering thumbnails
	// NOTE: This used to be DefaultBrickColor, but changing a default property value causes issues since they are not saved in existing creations
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	FColor ThumbnailBrickColor;
	// Dimensions of the brick
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	FVector BrickSize;
	// Whether the brick size should be appended to the display name
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	bool bAppendBrickSizeToDisplayName;
	// Cost to add per size unit
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	float PricePerSize;
	// List of connectors on this brick
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	TArray<FConnectorField> Connectors;
	// Cached maximum distance of all connectors to the origin of the brick
	UPROPERTY(VisibleDefaultsOnly, Category = Brick)
	float MaxConnectorDist;

	// Scale to apply to the bricks mass
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	float MassScale;
	// Additional mass scale applied to the displayed mass
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	float DisplayMassScale;
	// Scale to apply to the bricks inertia tensor
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	FVector InertiaTensorScale;
	// Optional physical material to override the one of the brick's material
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	UBrickPhysicalMaterial* PhysicalMaterialOverride;
	// Whether a fluid dynamic surface should be calculated for the brick (NOTE: This also depends on the shape of the brick)
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	uint8 bGenerateFluidDynamicSurface : 1;
	// Whether lift generation should be enabled by default
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	uint8 bDefaultGenerateLift : 1;
	// Whether physics properties should be calculated from the shapes
	UPROPERTY(EditDefaultsOnly, Category = Physics)
	uint8 bCalculatePhysicsPropertiesFromShapes : 1;
	// Cached mass and buoyancy related properties
	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	float Volume;
	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	FVector CenterOfMass;
	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	FVector InertiaTensor0;
	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	FVector InertiaTensor1;
	UPROPERTY(VisibleDefaultsOnly, Category = Physics)
	FVector InertiaTensor2;

	// The higher this value the more likely the connected part is to be replicated
	UPROPERTY(EditDefaultsOnly, Category = Replication)
	int32 MovementReplicationPriority;
	// ~Properties

	// ~Constructor
	UBrickStaticInfo();

	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	virtual FText GetDisplayName() const override;
	virtual void GetBrickEditorFilterTags(FGameplayTagContainer& OutTags) const override;
	virtual FVector GetBrickEditorObjectSize() const override;
	virtual float GetBrickEditorObjectPrice() const override;
	virtual void GetTooltipContent(FTooltipContent& OutContent) const override;
#if WITH_EDITOR
	virtual void PostLoad() override;
	virtual void BuildCachedData() override;
#endif
	// ~Super Interface

	// Used to get the axis that should be used for the surface, or INDEX_NONE
	static int32 CalcFluidDynamicSurfaceAxis(const FVector& BoundsSize, const FVector* AverageNormals);

	// Used to calculate the actual mass properties at runtime
	void GetMassProperties(UBrickPhysicalMaterial* PhysMaterial, float InMassScale, float InVolumeScale, PxMassProperties& OutMassProps) const;
	// Applies scaling etc. to the mass properties
	void ConvertMassProperties(UBrickPhysicalMaterial* PhysMaterial, float InMassScale, PxMassProperties& InOutMassProps) const;
	// Used to calculate the mass properties from a list of shapes
	void CalcMassPropertiesFromShapes(const TArray<FPhysicsShapeHandle_PhysX>& Shapes, PxMassProperties& OutMassProps) const;
	// Returns the mass scale that should be used in the editor
	float GetBrickEditorMassScale() const
	{
		return MassScale * DisplayMassScale;
	}

	// Calculates the price of the brick
	float CalcPrice(const UBrickMaterial* BrickMaterial, const FVector& InBrickSize, bool bGenerateLift) const;
	// Calculates the price of fuel
	static float CalcFuelPrice(const FFuelTankParams& Params);

	// Can be implemented for bricks which store fuel
	virtual FFuelTankParams GetFuelTankParams() const
	{
		return {};
	}

protected:
	// Returns the physical material that is used by default
	UBrickPhysicalMaterial* GetDefaultPhysMaterial() const;
#if WITH_EDITOR
	// Used to recalculate the cached connector properties
	void CalcConnectorProperties();
	// Updates the cached fluid dynamic properties
	void CalcFluidDynamicProperties();
	// Updates the cached mass related properties
	virtual void CalcMassProperties();
	// Get the static mesh that should be used to calculate mass properties
	virtual UStaticMesh* GetMassPropertiesProxyMesh(FTransform& OutTransform) const;
	// Calculates the mass properties for any static mesh
	PxMassProperties CalcMassPropertiesForMesh(UStaticMesh* Mesh, const FTransform& Transform) const;
	// Used to process and store the fluid dynamic properties
	virtual void ProcessFluidDynamicProperties(const FVector& BoundsSize, const FVector& SurfaceAreas, const FVector& AverageLocation, const FVector* AverageNormals);
	// Get the mesh that should be used to calculate the fluid dynamic properties
	virtual UStaticMesh* GetFluidDynamicProxyMesh() const;
	// Can be used to calculate a socket transform for the cached data
	bool CalcSocketTransform(const FName& SocketName, FTransform& OutTransform) const;
#endif
};

UCLASS(Blueprintable)
class BRICKRIGS_API UBrick : public UBrickEditorObject
{
	GENERATED_BODY()

protected:
	static constexpr auto MaxCustomBrickNameLength = 32;

	enum EBrickPrimitiveData
	{
		// IMPORTANT: When entries are added here, materials that use data indices after these have to be updated!
		// This applies to the light brick halo material and tire material for example
		// IMPORTANT: Also the brick material thumbnail rendering might break!
		Color = 0,
		RefLocation = Color + 4,
		RefPriAxis = RefLocation + 3,
		RefSecAxis = RefPriAxis + 3,
		Max = RefSecAxis + 3
	};

	// ~Variables
	// All connections that involve this brick
	UPROPERTY(Transient)
	TArray<UBrickConnection*> BrickConnections;
	// Current root of the part this brick belongs to
	UPROPERTY(Transient)
	UBrick* BrickPartRoot;
	// Current root of the cluster this brick belongs to
	UPROPERTY(Transient)
	UBrick* BrickClusterRoot;
	// Struct used on part roots to store additional data
	TUniquePtr<FBrickPartRootParams> PartRootParams;

	// Replicated damage information
	FBrickDamage BrickDamage;
	// While on fire: Time remaining until the brick spreads fire or stops burning
	// While not on fire: Next time the brick is allowed to catch fire (after being extinguished)
	float FireTime;

	// Number of burn intervals the brick yet has to do
	uint8 NumBurnIntervalsRemaining;
	// Key used to keep track of replicated changes
	uint16 ReplicationKey;

	// Used during UpdateBrickConnections to indicate if the brick's part or cluster root has already been updated
	uint16 bHasPartRootBeenUpdated : 1;
	uint16 bHasClusterRootBeenUpdated : 1;
	uint16 bHasRepMoveClusterBeenUpdated : 1;
	uint16 bIsRepMovePart : 1;
	// Used during UpdateBrickConnections to indicate if children have been added or removed to the part root or cluster root
	uint16 bPartRootChildrenAddedOrRemoved : 1;
	uint16 bClusterRootChildrenAddedOrRemoved : 1;
	// Used during UpdateBrickConnections to indicate whether the cluster contains any RC bricks
	uint16 bClusterContainsRCBrick : 1;
	// Whether there are any fluid dynamic elements on this cluster
	uint16 bClusterHasFluidDynamicElements : 1;
	// Whether the part root should currently simulate physics
	uint16 bPartRootSimulatePhysics : 1;
	// Whether the brick is currently connected to the root brick
	uint16 bIsConnectedToRoot : 1;
	// Whether the brick is currently controllable, i.e. connected to the root brick or an RC brick
	uint16 bIsControllable : 1;
	// Whether the brick is connected to any fuel tanks with fuel
	uint16 bHasAnyFuel : 1;
	// Cached flag indicating if the brick has a lift surface
	uint16 bHasFluidDynamicLiftSurface : 1;
	// Whether we are currently creating the physics state
	uint16 bIsCreatingPhysicsState : 1;
	// Whether the vehicle and brick are being repaired
	uint16 bIsRepairing : 1;
	// Whether replication is currently enabled
	uint16 bIsReplicated : 1;
	// ~Variables

	// ~Properties
	// Whether Blueprint events should be called
	UPROPERTY(EditDefaultsOnly)
	uint16 bEnableBlueprintEvents : 1;
	// Primary tick parameters
	UPROPERTY(EditDefaultsOnly)
	FBrickTickFunction PrimaryBrickTick;
	// ~Properties

	// ~Brick Properties
	// Physical material used for this Brick
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	UBrickMaterial* BrickMaterial;
	// Custom color of the brick
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	FColor BrickColor;
	// Texture pattern for this brick
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	TSubclassOf<UBrickPattern> BrickPattern;
	// Whether this brick should influence fluid dynamics
	UPROPERTY(EditDefaultsOnly, Category = Brick)
	bool bGenerateLift;
	// ~Brick Properties

public:
	// Used to access property names safely
	static FName GetBrickMaterialPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(ThisClass, BrickMaterial);
	}

	static FName GetBrickColorPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(ThisClass, BrickColor);
	}

	static FName GetBrickPatternPropertyName()
	{
		return GET_MEMBER_NAME_CHECKED(ThisClass, BrickPattern);
	}

	// ~Constructor
	UBrick();

	// ~Super Interface
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode) override;
	virtual void OnIsHiddenInEditorChanged() override;
	virtual void OnMirrorBrickEditorObject(EAxis::Type MirrorAxis) override;
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params) override;
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver) override;
	virtual void PostInitializeBrickEditorObject() override;
	virtual void UninitializeBrickEditorObject() override;
	virtual void RecycleBrickEditorObject() override;
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params) override;
	virtual bool ShouldBeHiddenByViewMode(EBrickEditorViewMode ViewMode) const override;
	virtual void OnEditorMoveCommitted() override;
	virtual void OnEditorMoveUpdate(const FTransform& NewTransform) override;
	virtual void CalcBrickEditorObjectMass(float& OutMass, FVector& OutCenterOfMass) const override;
	virtual float CalcBrickEditorObjectPrice() const override;
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual bool ResolveRemovedBrickProperty(const FResolveBrickPropertyParams& Params) override;
	virtual void UpdateEditorVisualization() override;

	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const override
	{
		return MakeEditorParams<FBrickEditorParams>();
	}

	// ~Super Interface

	// ~Brick
	// Get the owning vehicle, can be null
	UFUNCTION(BlueprintPure)
	ABrickVehicle* GetVehicle() const;

	// Get the outer vehicle component, always valid
	UFUNCTION(BlueprintPure)
	UBrickVehicleComponent* GetVehicleComponent() const;

	// Used to get the static info class from the owning vehicle, always returns a valid pointer
	UFUNCTION(BlueprintPure)
	const UBrickVehicleStaticInfo* GetVehicleStaticInfo() const;

	// Returns the root component cast to a static mesh
	UBrickStaticMeshComponent* GetStaticMeshComponent() const;

	// Returns the static mesh to use for the root component
	virtual UStaticMesh* GetStaticMesh() const;

	// Returns the body setup to use
	virtual UBodySetup* GetBodySetup() const;

	// Called by the static mesh component to calculate the local bounds
	virtual FBoxSphereBounds CalcStaticMeshBounds() const;

	// Used to gather slots and the default loadout for the vehicle inventory
	virtual void SetupVehicleInventory(FInventoryProperties& OutProperties, FInventoryLoadout& OutLoadout);

	// Called after the vehicle has been constructed
	virtual void PostConstructVehicle();

	// Called from the vehicle when any CVar has changed, testing only
	virtual void OnCVarChanged();

	// Return whether the brick should be replicated over the network
	virtual bool ShouldReplicate() const
	{
		return false;
	}

	// Returns whether replication is currently enabled
	bool IsReplicated() const
	{
		return bIsReplicated;
	}

	// Enables or disables replication if needed
	void UpdateIsReplicated();

	// Returns the current replication key
	auto GetReplicationKey() const
	{
		return ReplicationKey;
	}

	// Calculates the replication priority of the part
	int32 GetPartReplicationPriority(const UBrick* RootBrickPartRoot, const int32 NumBricks) const;

protected:
	// Should be called whenever a replicated variable has been changed
	void MarkReplicationDirty();

	// Called after the brick has been created in the editor or when spawned
	UFUNCTION(BlueprintImplementableEvent)
	void BeginPlay();

	// Called before the brick is destroyed
	UFUNCTION(BlueprintImplementableEvent)
	void EndPlay();

	// Called when creating the root mesh component
	virtual void SetupCreateMeshComponentParams(FBrickEditorMeshComponentParams& Params);

	// Called when creating the root static mesh component
	virtual void SetupCreateStaticMeshComponentParams(FBrickStaticMeshComponentParams& Params);

	// Updates the collision profile settings if needed
	void UpdateCollisionProfile();

	// Returns the desired collision response for the pawn channel
	ECollisionResponse GetPawnCollisionResponseOverride() const
	{
		return IsConnectedToRoot() ? ECR_Block : ECR_Ignore;
	}

	// Returns the collision channel that should be used
	ECollisionChannel GetCollisionChannelOverride() const
	{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
		if (!IsConnectedToRoot())
		{
			return ECC_DetachedBrick;
		}
#endif
		return ECC_MAX;
	}

	// Whether the pawn channel collision override should be used
	bool NeedsPawnCollisionResponseOverride() const
	{
		// Disable player collision for detached bricks only in multiplayer, since they aren't synced and teleport the player when they step on them
		return !IsNetMode(NM_Standalone);
	}

	// Used to get the desired position and velocity solver iteration counts to use
	void GetRootComponentSolverIterationCounts(uint8& PositionSolverCount, uint8& VelocitySolverCount) const;

	// Updates position and velocity solver counts if needed
	void UpdateSolverIterationCounts();

	// Used by guns, flamethrowers etc. to get the attached barrels and muzzlet transform
	void CalcMuzzleTransform(FTransform& OutMuzzleTransform, float* OutBarrelLength = nullptr) const;

	// Can be used by bricks that have cosmetic sub meshes that need to be considered for the bounds
	void AddCosmeticMeshToLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax, UStaticMesh* StaticMesh, const FTransform& RelativeTransform) const;

public:
	// Returns the local bounds size of the part
	void GetBrickPartLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const
	{
		OutBoundsMin = FVector(MAX_flt);
		OutBoundsMax = FVector(-MAX_flt);

		auto BrickBoundsMin = FVector::ZeroVector;
		auto BrickBoundsMax = FVector::ZeroVector;
		((UBrick*)this)->ForEachConnectedBrick<UBrick, false>([&](auto* Brick)
		{
			Brick->GetBrickEditorObjectLocalBounds(BrickBoundsMin, BrickBoundsMax);

			// Iterate all corners
			for (auto i = 0; i < 8; ++i)
			{
				const auto Offset = FVector((i & 1) > 0 ? BrickBoundsMax.X : BrickBoundsMin.X, (i & 2) > 0 ? BrickBoundsMax.Y : BrickBoundsMin.Y, (i & 4) > 0 ? BrickBoundsMax.Z : BrickBoundsMin.Z);
				const auto Extrema = Brick->GetParentRelativeTransform().TransformPosition(Offset);
				OutBoundsMin = OutBoundsMin.ComponentMin(Extrema);
				OutBoundsMax = OutBoundsMax.ComponentMax(Extrema);
			}
		});
	}

	// ~Brick

	// ~Scaling
	// Returns the desired mesh render scale
	virtual FVector GetRenderScale3D() const;

	// Returns the desired body setup scale
	virtual FVector GetBodySetupScale3D() const;

protected:
	// Updates the current render scale
	void UpdateRenderScale3D();

	// Updates the current body setup scale
	void UpdateBodySetupScale3D();
	// ~Scaling

public:
	// ~Physics
	// Whether we are currently creating the physics state
	bool IsCreatingPhysicsState() const
	{
		return bIsCreatingPhysicsState;
	}

	// Lets the root component create its physics state
	void CreatePhysicsState();

	// Updates the physics properties of the part root
	void PostShapeChange();

	struct FTickFluidDynamicsParams
	{
		const float DeltaTime;
		const FVector& WindVelocity;
		const ABrickWorldSettings* WorldSettings;
		const bool bHasPhysicsAuthority;
	};

	// Called on the cluster root to update fluid dynamics
	void TickClusterFluidDynamics(const FTickFluidDynamicsParams& Params);

private:
	// Updates fluid dynamics on the part root level
	void TickPartRootFluidDynamics(const FTickFluidDynamicsParams& Params);

public:
	// Used by the sliding effects particle module
	FVector GetRandomSlidingEffectLocation(UActorComponent* Comp) const;

protected:
	// Whether the SpawnCollisionEffects function needs to be called
	virtual bool ShouldSpawnCollisionEffects(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const;

	// Called by the vehicle upon a collision to update impact effects
	virtual void SpawnCollisionEffects(const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo);

	// Called on the part root to update the hit effects
	void SpawnCollisionEffectsInternal(const UBrick* HitBrick, const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo);

	// Callback for the global physics delegate
	void OnPhysDispatchNotifications(FPhysScene* PhysicsScene);

	// Used to unbind the delegate, part root only
	void UnbindOnPhysDispatchNotifications();

	// Immediatelly removes all sliding effects and unbinds the physics delegate
	void RemoveAllSlidingEffects();

	// Callback for the body instance
	virtual void OnCalculateMassProperties(FBodyInstance* BodyInstance, PxMassProperties& OutMassProps, FTransform& OutMassTransform);

	// Calculates mass properties based on the shapes of the brick
	virtual void CalcMassPropertiesFromShapes(const FBodyInstance* BodyInstance, UBrickPhysicalMaterial* PhysMaterial, float MassScale, PxMassProperties& OutMassProps) const;

public:
	// Returns the physics body volume scale to use when spawned
	virtual float GetVolumeScale() const;

	// Returns the volume scale factor that should be used in the editor
	virtual float GetBrickEditorVolumeScale() const;

	// Whether the brick has a lift surface that should be used
	bool ShouldUseFluidDynamicLiftSurface() const
	{
		return bHasFluidDynamicLiftSurface && bGenerateLift;
	}

protected:
	// Whether the lift property should be displayed
	virtual bool ShouldShowGenerateLiftProperty() const
	{
		return bHasFluidDynamicLiftSurface;
	}

	// Used to gather all fluid dynamic elements for the brick
	virtual void GetFluidDynamicElements(FFluidDynamicElements& OutElements, const FTransform& Transform) const;

	// Used to gather the fluid dynamic surface
	virtual void GetFluidDynamicSurface(FFluidDynamicSurface& OutElement, const FTransform& Transform) const;
	// ~Physics

public:
	// ~Ticking
	// Overridable main tick function
	virtual void TickBrick(float DeltaTime);

	// Blueprint tick function
	UFUNCTION(BlueprintImplementableEvent)
	void Tick(float DeltaTime);

	// Whether the brick should currently be ticking
	virtual bool ShouldBrickTick() const;

	// Blueprint version
	UFUNCTION(BlueprintNativeEvent)
	bool ShouldTick() const;

	// Enables or disables ticking if needed
	UFUNCTION(BlueprintCallable)
	bool UpdateBrickTickEnabled();

	// Whether we are currently ticking
	UFUNCTION(BlueprintPure)
	bool IsBrickTickEnabled() const
	{
		return PrimaryBrickTick.IsTickFunctionEnabled();
	}

	// ~Ticking

	// ~Net
	// Get the current net mode
	ENetMode GetNetMode() const
	{
		// IsRunningDedicatedServer() is a compile-time check in optimized non-editor builds.
		if (IsRunningDedicatedServer())
		{
			return NM_DedicatedServer;
		}
		return GetNetModeInternal();
	}

	// Test whether the current net mode is the given mode
	bool IsNetMode(ENetMode Mode) const
	{
#if UE_EDITOR
		// Editor builds are special because of PIE, which can run a dedicated server without the app running with -server.
		return GetNetMode() == Mode;
#else
		// IsRunningDedicatedServer() is a compile-time check in optimized non-editor builds.
		if (Mode == NM_DedicatedServer)
		{
			return IsRunningDedicatedServer();
		}
		else
		{
			return !IsRunningDedicatedServer() && GetNetModeInternal() == Mode;
		}
#endif
	}

private:
	// Internal version that gets the net mode from the owner
	ENetMode GetNetModeInternal() const;
	// ~Net

public:
	// ~Connections
	// Get all connectors this brick uses
	virtual const TArray<FConnectorField>& GetBrickConnectors() const;

	// Can be overridden to highlight directions on the connector visualization
	virtual uint8 GetFocusedConnectorAxisFlags() const
	{
		return 0;
	}

	// Get the maximum distance of a connector nob on this brick
	virtual float GetMaxConnectorDist() const;

	// Returns the connector field at the given index if it exists
	const FConnectorField* GetConnectorAtIndex(int32 ConnectorIndex) const;

	// Get the type of a connector field at the given index
	EConnectorType GetConnectorType(int32 ConnectorIndex) const;

	// Get the relative location and rotation of a connector field
	virtual FTransform GetConnectorRelativeTransform(const FConnectorField& ConnectorField) const;

	// Get the relative location and rotation of a connector field at the given index
	FTransform GetConnectorRelativeTransform(const int32 ConnectorIndex) const;

	// Get the current world transform of a connector field
	FTransform GetConnectorWorldTransform(const int32 ConnectorIndex) const;

	// Get all connections involving this brick
	UFUNCTION(BlueprintPure)
	const TArray<UBrickConnection*>& GetBrickConnections() const
	{
		return BrickConnections;
	}

	// Whether this part is connected to any external one
	bool IsConnectedToExternalPart() const
	{
		const auto& RootParams = GetBrickPartRootParams();
		return RootParams.IsValid() && RootParams->ExternalPartConnections.Num() > 0;
	}

	// Returns true if there are multiple bricks in the cluster
	UFUNCTION(BlueprintPure)
	bool IsConnectedToAnyBrick() const
	{
		const auto& RootParams = GetBrickPartRootOrSelf()->PartRootParams;
		return RootParams.IsValid() && (RootParams->Children.Num() > 0 || IsConnectedToExternalPart());
	}

	// Returns whether this brick is directly connected to another brick
	bool IsDirectlyConnectedToBrick(const UBrick* Other) const
	{
		return BrickConnections.ContainsByPredicate([&](UBrickConnection* Connection)
		{
			return !Connection->IsBroken() && Connection->GetOtherBrick(this) == Other;
		});
	}

	// Returns whether this brick was connected to the other last frame
	bool WasConnectedToBrickLastFrame(const UBrick* Other) const;

	// Can be used to create a custom brick connection type
	virtual UBrickConnection* CreateCustomBrickConnection(UBrick* OtherBrick, const FName& ConnectionName, FBrickConnectionParams& Params) const;

	// Add a brick connection to the list
	void AddBrickConnection(UBrickConnection* InConnection);

	// Remove a brick connection
	bool RemoveBrickConnection(UBrickConnection* InConnection, bool bAllowShrinking = true);

	// Remove all brick connections
	void ClearBrickConnections();

	// Called when a connection involving this brick has been activated
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection);

	// Called when a connection involving this brick has broken
	virtual void OnBrickConnectionBroke(UBrickConnection* InConnection);

	// Returns the number of connected bricks, either on the part or cluster layer
	int32 GetNumConnectedBricks(bool bCluster = true) const
	{
		if (bCluster)
		{
			auto OutNum = 0;
			((UBrick*)this)->ForEachConnectedBrick<UBrick, true, true, true, false>([&](auto* Brick)
			{
				const auto& RootParams = Brick->GetBrickPartRootParams();
				OutNum += RootParams.IsValid() ? Brick->PartRootParams->Children.Num() + 1 : 0;
			});

			// Subtract this brick
			return OutNum - 1;
		}

		const auto& RootParams = GetBrickPartRootParams();
		return RootParams.IsValid() ? RootParams->Children.Num() : 0;
	}

	// Gets all directly and indirectly connected bricks
	// NOTE: Usually ForEachConnectedBrick will be faster
	template <class T>
	void GetConnectedBricks(TArray<T*>& OutBricks) const
	{
		((UBrick*)this)->ForEachConnectedBrick<T>([&](T* Brick)
		{
			OutBricks.Add(Brick);
		});
	}

	// Useful to iterate all directly or directly connected bricks or part roots
	template <typename BrickType = UBrick, bool bCluster = true, bool bOwnRoot = true, bool bChildRoots = true, bool bChildren = true>
	void ForEachConnectedBrick(const TFunction<void(BrickType*)>& Function)
	{
		const auto OwnPartRoot = bCluster ? GetBrickClusterRootOrSelf() : GetBrickPartRootOrSelf();
		if (bOwnRoot)
		{
			ForEachConnectedBrickInternal<BrickType>(OwnPartRoot, Function);
		}

		if (bChildren)
		{
			if (OwnPartRoot->PartRootParams.IsValid())
			{
				for (auto ChildBrick : OwnPartRoot->PartRootParams->Children)
				{
					ForEachConnectedBrickInternal<BrickType>(ChildBrick, Function);
				}
			}
		}

		if (bCluster && (bChildRoots || bChildren))
		{
			const auto* OwnClusterRoot = GetBrickClusterRootOrSelf();
			if (OwnClusterRoot->IsBrickClusterRoot())
			{
				for (auto PartRoot : OwnClusterRoot->PartRootParams->ClusterRootParams->ChildPartRoots)
				{
					if (bChildRoots)
					{
						ForEachConnectedBrickInternal<BrickType>(PartRoot, Function);
					}

					if (bChildren)
					{
						if (PartRoot->IsBrickPartRoot())
						{
							for (auto ChildBrick : PartRoot->PartRootParams->Children)
							{
								ForEachConnectedBrickInternal<BrickType>(ChildBrick, Function);
							}
						}
					}
				}
			}
		}
	}

private:
	// Helpers used for ForEachConnectedBrick to
	template <typename BrickType>
	void ForEachConnectedBrickInternal(UBrick* Brick, const TFunction<void(BrickType*)>& Function)
	{
		if (Brick->IsA<BrickType>())
		{
			Function(CastChecked<BrickType>(Brick));
		}
	}

	// Specialized version that makes iterating over base class bricks faster
	void ForEachConnectedBrickInternal(UBrick* Brick, const TFunction<void(UBrick*)>& Function)
	{
		Function(Brick);
	}

protected:
	// Checks whether there is any brick on the part or cluster that matches the given predicate
	template <bool bCluster = true>
	bool IsConnectedToBrickByPredicate(const TFunction<bool(const UBrick*)>& Function) const
	{
		// Check the root
		const auto* OwnPartRoot = bCluster ? GetBrickClusterRootOrSelf() : GetBrickPartRootOrSelf();
		if (Function(OwnPartRoot))
		{
			return true;
		}

		// Check our children
		if (OwnPartRoot->IsBrickPartRoot())
		{
			for (const auto* ChildBrick : OwnPartRoot->PartRootParams->Children)
			{
				if (Function(ChildBrick))
				{
					return true;
				}
			}
		}

		if (bCluster)
		{
			// Check the cluster children
			const auto* OwnClusterRoot = GetBrickClusterRootOrSelf();
			if (OwnClusterRoot->IsBrickClusterRoot())
			{
				for (const auto* PartRoot : OwnClusterRoot->PartRootParams->ClusterRootParams->ChildPartRoots)
				{
					if (Function(PartRoot))
					{
						return true;
					}

					if (PartRoot->IsBrickPartRoot())
					{
						for (const auto* ChildBrick : PartRoot->PartRootParams->Children)
						{
							if (Function(ChildBrick))
							{
								return true;
							}
						}
					}
				}
			}
		}

		return false;
	}

public:
	// Get all bricks that are connected to this one via a rigid brick connection
	void GatherBrickPartRootChildren(TArray<UBrick*>& OutBricks, TArray<UPhysicsConstraintConnection*>& OutExternalPartConnections)
	{
		GatherBrickPartRootChildrenInternal(this, OutBricks, OutExternalPartConnections);
	}

private:
	// Internal version that is used for recursion
	void GatherBrickPartRootChildrenInternal(const UBrick* PartRoot, TArray<UBrick*>& OutBricks, TArray<UPhysicsConstraintConnection*>& OutExternalPartConnections)
	{
		bHasPartRootBeenUpdated = true;

		for (auto* Connection : BrickConnections)
		{
			if (!Connection->IsBroken())
			{
				if (UNLIKELY(Connection->IsPhysicsConstraintConnection()))
				{
					OutExternalPartConnections.AddUnique(CastChecked<UPhysicsConstraintConnection>(Connection));
				}
				else
				{
					auto* OtherBrick = Connection->GetOtherBrick(this);
					// Rigid connections cannot exist between different vehicles
					check(OtherBrick && OtherBrick->GetVehicle() == GetVehicle());

					// Check if the brick has been included in a part yet
					if (!OtherBrick->bHasPartRootBeenUpdated)
					{
						OutBricks.Add(OtherBrick);
						OtherBrick->GatherBrickPartRootChildrenInternal(PartRoot, OutBricks, OutExternalPartConnections);
					}
				}
			}
		}
	}

public:
	// Get all part roots that are connected directly or indirectly to this part (the cluster root)
	void GatherBrickClusterRootChildParts(TArray<UBrick*>& OutPartRoots, bool& bOutChildrenAddedOrRemoved)
	{
		check(IsBrickPartRoot());

		GatherBrickClusterRootChildPartsInternal(this, OutPartRoots, bOutChildrenAddedOrRemoved);
	}

private:
	// Internal version that is used for recursion
	void GatherBrickClusterRootChildPartsInternal(const UBrick* ClusterRoot, TArray<UBrick*>& OutPartRoots, bool& bOutChildrenAddedOrRemoved)
	{
		check(IsBrickPartRoot());

		bHasClusterRootBeenUpdated = true;
		bOutChildrenAddedOrRemoved |= bPartRootChildrenAddedOrRemoved;

		for (const auto* Connection : PartRootParams->ExternalPartConnections)
		{
			auto* OtherPartRoot = Connection->GetOtherBrickPartRoot(this);
			// NOTE: Only include parts of the same vehicle
			if (!OtherPartRoot->bHasClusterRootBeenUpdated && OtherPartRoot->GetVehicle() == GetVehicle())
			{
				OutPartRoots.Add(OtherPartRoot);
				OtherPartRoot->GatherBrickClusterRootChildPartsInternal(ClusterRoot, OutPartRoots, bOutChildrenAddedOrRemoved);
			}
		}
	}

public:
	// Get all part roots that are somewhat rigidly connected to this one and should replicate movement together
	void GatherRepMoveCluster(const UBrick* RootBrickPartRoot, UBrick*& OutRoot, TArray<UBrick*>& OutPartRoots)
	{
		auto MaxMass = -1.f;
		GatherRepMoveClusterInternal(RootBrickPartRoot, OutRoot, OutPartRoots, MaxMass);
	}

private:
	// Used for recursion
	void GatherRepMoveClusterInternal(const UBrick* RootBrickPartRoot, UBrick*& OutRoot, TArray<UBrick*>& OutPartRoots, float& MaxMass)
	{
		check(IsBrickPartRoot());

		bHasRepMoveClusterBeenUpdated = true;
		OutPartRoots.Add(this);

		if (this == RootBrickPartRoot)
		{
			// Force the part containing the root brick to be the replication root
			OutRoot = this;
			MaxMass = MAX_flt;
		}
		else
		{
			// Pick the heaviest part as the root
			const auto Mass = GetMass();
			if (Mass > MaxMass)
			{
				OutRoot = this;
				MaxMass = Mass;
			}
		}

		// Recursively iterate all connected parts
		for (const auto* Connection : PartRootParams->ExternalPartConnections)
		{
			auto* OtherPartRoot = Connection->GetOtherBrickPartRoot(this);

			// Ignore connections with other vehicles and parts that have already been processed
			if (OtherPartRoot->GetVehicle() != GetVehicle() || OtherPartRoot->HasRepMoveClusterBeenUpdated())
			{
				continue;
			}

			// Skip if the other part is replicated itself and we can't join replication
			if (OtherPartRoot->bIsRepMovePart && !Connection->CanJoinMovementReplication())
			{
				continue;
			}

			OtherPartRoot->GatherRepMoveClusterInternal(RootBrickPartRoot, OutRoot, OutPartRoots, MaxMass);
		}
	}

public:
	// Calculates a possible snapping offset to attach to the editor grid
	bool CalculateConnectionToEditorGrid(
		const FTransform& OwnTransform,
		const FConnectorField& EditorConnectorField,
		FBrickConnectionParams& OutParams,
		FBrickConnectionTollerance& Tolerance,
		FBrickEditorSnappingOffset& OutSnappingOffset
	);

	// Calculate a potential connection between two bricks, optionally outputs the closest snapping offset
	bool CalculateConnectionTo(
		UBrick* OtherBrick,
		const FTransform& OwnTransform,
		const FTransform& OtherTransform,
		FBrickConnectionParams& OutParams,
		FBrickConnectionTollerance& Tollerance,
		FBrickEditorSnappingOffset* OutSnappingOffset = nullptr
	) const;

private:
	// Get the connection between two connector fields from this and another brick
	// Returns the number of nobs that would connect the two bricks
	int32 GetConnectorFieldConnection(
		const FConnectorField& ConnectorField,
		const FTransform& BrickTransform,
		const UBrick* OtherBrick,
		const FConnectorField& OtherConnectorField,
		const FTransform& OtherBrickTransform,
		FBrickConnectionTollerance& Tollerance,
		FBrickEditorSnappingOffset* OutSnappingOffset
	) const;

	// Internal version used for convenience
	int32 GetConnectorFieldConnectionInternal(
		const FConnectorField& ConnectorField,
		const FTransform& ConnectorTransform,
		const FVector& ConnectorNormal,
		const FConnectorField& OtherConnectorField,
		const FTransform& OtherConnectorTransform,
		const FVector& OtherConnectorNormal,
		float ConnectorAngle,
		FBrickConnectionTollerance& Tollerance,
		FBrickEditorSnappingOffset* OutSnappingOffset,
		bool bInvertSnapping
	) const;
	// ~Connections

public:
	// ~Part Root
	// Sets the brick up as a part root
	void InitializeAsPartRoot(bool bShouldSimulatePhysics);

	// Attaches new children to the part root and removes old ones
	void SetPartRootChildren(const TArray<UBrick*>& NewChildren, const TArray<UPhysicsConstraintConnection*>& NewExternalPartConnections, TArray<UPhysicsConstraintConnection*>& OutConnectionsToUpdate, bool bIsInitializingConnections, bool bInitialzedPartRoot, bool bShouldSimulatePhysics);

	// Lets the part root enable or disable physics as needed, returns true if the flag was changed
	bool SetPartRootSimulatePhysics(bool bNewSimulate);

	// Enables or disables physics if needed, returns true if it changed
	bool UpdatePartRootSimulatePhysics();

	// Called on the part root when the pin mode has changed, returns whether simulate physics has changed
	bool OnPinModeChanged(bool bAllPinned, bool bNewSimulate);

private:
	// Called after bricks have been attached or detached
	void UpdatePartRoot(bool bInHasAnyFuel, bool& bOutHasAnyFluidDynamicElements);

	// Ends this brick as the part root
	void UninitializePartRoot();

	// Used to detach children from the part root
	void DetachPartRootChildren(const TArray<UBrick*>& NewChildren);

	// Attaches the brick to a new part root
	void AttachToPartRoot(UBrick* InPartRoot);

public:
	// Attaches and welds the root component
	void AttachRootComponentToPartRoot();

	// Detaches the brick from its part root
	void DetachFromPartRoot();

protected:
	struct FOnUpdatePartRootParams
	{
		FFluidDynamicElements& FluidDynamicElements;
		uint16& MovementReplicationPriority;
	};

	// Called on every child brick during UpdatePartRoot (if bricks were added or removed)
	virtual void OnUpdatePartRoot(FOnUpdatePartRootParams& Params);

	// Called whenever the part root has changed
	virtual void OnBrickPartRootChanged()
	{
	}

public:
	// Used to get the spawn transform relative to the part root brick
	// NOTE: Relative scale 3d is not needed since scale it absolute
	void GetPartRootRelativeSpawnTransform(FVector& OutRelativeLocation, FRotator& OutRelativeRotation) const;

	// Returns the transform relative to the part root, identity if this is the part root
	FTransform GetPartRootRelativeTransform() const
	{
		return IsBrickPartRoot() ? FTransform::Identity : GetRelativeTransform();
	}

	// Moves a brick part, to be called on the part root
	void AddBrickPartOffset(const FVector& Offset);

	// Whether this brick is a part root
	UFUNCTION(BlueprintPure)
	bool IsBrickPartRoot() const
	{
		return PartRootParams.IsValid();
	}

	// Get the part root parameters instance
	const TUniquePtr<FBrickPartRootParams>& GetBrickPartRootParams() const
	{
		return GetBrickPartRootOrSelf()->PartRootParams;
	}

	// Returns the brick this brick is attached to
	UFUNCTION(BlueprintPure)
	UBrick* GetBrickPartRoot() const
	{
		return BrickPartRoot;
	}

	// Returns the brick this brick is attached to, or itself it isn't attached. The returned pointer is guaranteed to be valid
	UBrick* GetBrickPartRootOrSelf() const
	{
		return BrickPartRoot ? BrickPartRoot : const_cast<UBrick*>(this);
	}

	// These functions are used during UpdateBrickConnections to accelerate updating
	bool HasPartRootBeenUpdated() const
	{
		return bHasPartRootBeenUpdated;
	}

	bool HasClusterRootBeenUpdated() const
	{
		return bHasClusterRootBeenUpdated;
	}

	bool HasRepMoveClusterBeenUpdated() const
	{
		return bHasRepMoveClusterBeenUpdated;
	}

	void MarkAsRepMovePart()
	{
		bIsRepMovePart = true;
	}

	// Resets all the update flags
	void PreGatherConnectedBricks()
	{
		bHasPartRootBeenUpdated = false;
		bHasClusterRootBeenUpdated = false;
		bHasRepMoveClusterBeenUpdated = false;
		bIsRepMovePart = false;
	}

	// ~Part Root

	// ~Cluster Root
	// Marks the brick as the cluster root
	void InitializeAsClusterRoot();

	// Updates the children parts on the cluster
	void SetClusterRootChildParts(const TArray<UBrick*>& NewChildParts, bool bBricksAddedOrRemoved);

	// Updates the cluster root and all its child parts
	void UpdateClusterRoot(bool bInIsConnectedToRoot, TArray<FBrickEditorObjectID>& ClusterRootBricks, TArray<FBrickEditorObjectID>& ClusterRootBricksWithFluidDynamics);

private:
	// Ends this brick as the cluster root
	void UninitializeClusterRoot();

protected:
	// Called whenever bricks have been attached or detached from the cluster (but only while the object is initialized)
	virtual void OnBricksAddedOrRemovedFromCluster()
	{
	}

public:
	// Whether this brick is the cluster root
	UFUNCTION(BlueprintPure)
	bool IsBrickClusterRoot() const
	{
		return IsBrickPartRoot() && PartRootParams->ClusterRootParams.IsValid();
	}

	// Get the cluster root parameters instance
	const auto& GetBrickClusterRootParams() const
	{
		check(IsBrickClusterRoot());
		return PartRootParams->ClusterRootParams;
	}

	// Returns the root brick of the cluster this brick belongs to
	UFUNCTION(BlueprintPure)
	UBrick* GetBrickClusterRoot() const
	{
		return BrickClusterRoot;
	}

	// Returns the cluster root or this brick, if it is null
	UBrick* GetBrickClusterRootOrSelf() const
	{
		return BrickClusterRoot ? BrickClusterRoot : (UBrick*)this;
	}

	// Whether the cluster has any fuel
	UFUNCTION(BlueprintPure)
	bool ClusterRootHasAnyFuel() const
	{
		// NOTE: Don't check the FuelLevel, since that isn't reliable on clients
		return GetBrickClusterRootParams()->NumBricksWithFuel > 0;
	}

	// Lets the cluster consume fuel, returns true if there was any left
	UFUNCTION(BlueprintCallable)
	bool ClusterConsumeFuel(float Amount);

	// Allows tanks to add or remove themselves from the cluster
	void ClusterRootAddFuelTank(UBrick* Brick, bool bAdd);
	// ~Cluster Root

	// ~Material
	// Used to manually assign a new material for thumbnail rendering
	void SetThumbnailRenderMaterial(UBrickMaterial* NewMaterial);

protected:
	// Get the material that should be applied to the brick
	UMaterialInterface* GetBrickMaterial() const;

	// Returns the material that should be used for the given slot in the editor
	virtual UMaterialInterface* GetEditorBrickMaterial(int32 MaterialIndex) const;

	// Returns the material that should be applied in the editor, or null
	UMaterialInterface* GetEditorMaterialOverride() const;

	// Updates the material instance parameters
	virtual void UpdateBrickMaterial();

	// Returns the number of material slots on the root component
	virtual int32 GetNumMaterialSlots() const;

	// Used to assign materials
	template <typename T>
	void AssignBrickMaterials(T Container, int32 NumMaterials) const
	{
		if (IsInEditor())
		{
			// Override all material slots with the editor material if needed
			auto* MaterialOverride = GetEditorMaterialOverride();
			for (auto MaterialIdx = 0; MaterialIdx < NumMaterials; ++MaterialIdx)
			{
				auto* Material = MaterialOverride ? MaterialOverride : GetEditorBrickMaterial(MaterialIdx);
				Container->SetMaterial(MaterialIdx, Material);
			}
		}
		else
		{
			Container->SetMaterial(0, GetBrickMaterial());
		}
	}

	// Used to assign brick material specific custom data
	template <typename T>
	void AssignBrickMaterialCustomPrimitiveData(T Container, const FTransform& Transform, const FTransform& RelativeTransform = FTransform::Identity) const
	{
		if (GetViewMode() == EBrickEditorViewMode::Physics)
		{
			// Assign data for the fluid dynamics material
			auto Element = FFluidDynamicSurface();
			if (ShouldUseFluidDynamicLiftSurface())
			{
				// NOTE: Store the surface in local space, so it remains correct after being moved
				const auto SurfaceTransform = RelativeTransform.Inverse();
				GetFluidDynamicSurface(Element, SurfaceTransform);
			}
			else
			{
				Element.Location = -RelativeTransform.GetLocation();
				Element.Normal = RelativeTransform.InverseTransformVectorNoScale(FVector::UpVector);
				Element.Radius = 0.f;
			}

			// NOTE: If any data is added the assert below should be adapted
			Container->SetCustomPrimitiveDataFloat(0, FMath::Square(Element.Radius) * PI);
			Container->SetCustomPrimitiveDataVector3(1, Element.Location);
			Container->SetCustomPrimitiveDataVector3(4, Element.Normal);

			// Make sure there is enough slack for the fluid dynamics material data
			static_assert(7 <= Max, "Not enough slack for fluid dynamics visualization custom data");
		}
		else
		{
			// Use white for the burnt pattern
			const auto NewColor = IsBrickBurnt() ? FLinearColor::White : FLinearColor(BrickColor);
			Container->SetCustomPrimitiveDataVector4(Color, NewColor);

			// Add the transform vectors
			Container->SetCustomPrimitiveDataVector3(RefLocation, Transform.GetLocation());
			const auto RefPriAxis = Transform.GetUnitAxis(EAxis::X);
			Container->SetCustomPrimitiveDataVector3(EBrickPrimitiveData::RefPriAxis, RefPriAxis);
			const auto RefSecAxis = Transform.GetUnitAxis(EAxis::Y);
			Container->SetCustomPrimitiveDataVector3(EBrickPrimitiveData::RefSecAxis, RefSecAxis);
		}
	}

public:
	// Get the current physical material
	virtual UBrickPhysicalMaterial* GetBrickPhysMaterial(bool bAllowOverrideMaterial = true) const;

	// Updates the current physical material
	void UpdateBrickPhysMaterial();
	// ~Material

	// ~Interaction
	// Whether interaction should be shown for the brick
	bool ShouldFocusedBrickInteractionBeVisible(ABrickPlayerController* PC, const FHUDIconContext& Context) const;

	// Returns the interaction options for the brick
	void GetFocusedBrickInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);

	// Returns the desired interaction location for the brick
	FVector GetFocusedBrickInteractionLocation() const;

protected:
	// Whether this brick can currently be picked up
	virtual bool CanBePickedUp() const
	{
		return true;
	}

public:
	// Whether this brick (part) can currently be carried
	bool CanBrickPartBePickedUp() const;

	// Used while a brick is being carried to update it's location
	void CarryBrick(float DeltaTime, const FVector& TargetLocation, float SpeedCoefficient, float Acceleration);

	// Allows characters to drop/throw a carried brick
	void DropBrick(const FVector& Location, const FRotator& Rotation, const FVector& Velocity);

private:
	UFUNCTION()
	void Interact_PickUp(ABrickPlayerController* OtherPC);
	// ~Interaction

public:
	// ~Root Connection
	// Whether the brick is directly or indirectly connected to the root brick
	UFUNCTION(BlueprintPure)
	bool IsConnectedToRoot() const
	{
		return bIsConnectedToRoot;
	}

private:
	void SetIsConnectedToRoot(bool bNewConnected)
	{
		if (bNewConnected != bIsConnectedToRoot)
		{
			bIsConnectedToRoot = bNewConnected;

			// Collision object type or pawn collision response may need to be updated
			UpdateCollisionProfile();
		}
	}

	// ~Root Connection

public:
	// ~Controllability
	// Whether the brick is connected to the root brick or an RC brick
	UFUNCTION(BlueprintPure)
	bool IsControllable() const
	{
		return bIsControllable;
	}

	// Whether this brick makes detached parts controllable (RC brick for example)
	virtual bool IsRCBrick() const
	{
		return false;
	}

private:
	void SetIsControllable(bool bNewControllable)
	{
		if (bNewControllable != bIsControllable)
		{
			bIsControllable = bNewControllable;

			// Only notify the brick if it has already been initialized
			if (IsBrickEditorObjectInitialized())
			{
				OnIsControllableChanged();
			}
		}
	}

protected:
	// Called whenever the controllability state of the cluster has changed (but only while the brick is initialized)
	virtual void OnIsControllableChanged()
	{
	}

	// ~Controllability

	// ~Input Channels
	// Registers a single input channel, should be called in PostInitializeBrickEditorObject
	void RegisterInputChannel(const FVehicleInputChannel& InputChannel, const FInputChannelDelegate& Delegate) const;

	// Convenience version that takes value to update
	void RegisterInputChannel(const FVehicleInputChannel& InputChannel, float& InputChannelValue);

	// Version that automatically clamps the value
	void RegisterInputChannel(const FVehicleInputChannel& InputChannel, float& InputChannelValue, float Min, float Max);

	// Version that takes a boolean
	void RegisterBoolInputChannel(const FVehicleInputChannel& InputChannel, bool& bInputChannelValue);

public:
	// Implement if the brick has any input channel
	virtual bool HasAnyInputChannel() const
	{
		return false;
	}

	// Implement to return all input channels in use
	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const
	{
	}

	// Implement if the brick has any output channel
	virtual bool HasAnyOutputChannel() const
	{
		return false;
	}

	// Implement to return the brick's output channel
	virtual FVehicleOutputChannel* GetOutputChannel() const
	{
		return nullptr;
	}

	// Get the seat which is controlling the given input channel
	USeatBrick* GetSeatForInputChannel(const FVehicleInputChannel& InputChannel) const;

	// Version that takes a driver seat
	USeatBrick* GetSeatForInputChannel(const FVehicleInputChannel& InputChannel, const USeatBrick* DriverSeat) const;

	// Returns the effective source bricks to use for an input channel
	void GetSourceBricksForInputChannel(const FVehicleInputChannel& InputChannel, const USeatBrick* DriverSeat, TArray<UBrick*>& OutSourceBricks) const;

	// Get the character responsible for the given input channel
	ABrickCharacter* GetInputChannelInstigator(const FVehicleInputChannel& InputChannel) const;
	// ~Input Channels

	// ~Damage
	// Whether this brick is currently on fire
	UFUNCTION(BlueprintPure)
	bool IsOnFire() const
	{
		return BrickDamage.IsOnFire();
	}

	// Tries to set the brick on fire, returns true if successful, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool SetOnFire();

	// Whether the brick can currently burn
	UFUNCTION(BlueprintPure)
	bool CanBrickBurn() const;

	// Tries to extinguish the brick, returns true if successful, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	bool ExtinguishFire();

	// Whether the brick is destroyed, i.e. unusable
	UFUNCTION(BlueprintPure)
	bool IsBrickDamaged() const
	{
		return BrickDamage.IsDamaged();
	}

	// Whether the brick has been burnt
	UFUNCTION(BlueprintPure)
	bool IsBrickBurnt() const
	{
		return BrickDamage.IsBurnt();
	}

	// Called from the vehicle to apply the new replicated damage state
	void SetReplicatedBrickDamage(const FBrickDamage& NewDamage);

	// Resets the damage to default upon repairing, server only
	void ResetBrickDamage();

private:
	// Updates the brick damage info, server only
	void SetBrickDamage(const FBrickDamage& NewDamage, bool bUpdateReplicatedDamage = true);

	// Called whenever the damage state has changed
	void OnBrickDamageChanged(const FBrickDamage& PrevDamage);

protected:
	// Marks the brick as damaged, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetIsBrickDamaged(bool bNewDamaged);

	// Marks the brick as damaged and burned, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void MarkBrickBurnt();

	// Marks the brick as damaged and burned, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetIsBrickBurnt(bool bNewBurnt);

	// Called when the state of damage of the brick has changed
	virtual void OnBrickDamageStateChanged(EBrickDamageState PrevState);

	// Called when the brick has been set on fire or extinguished
	virtual void OnIsOnFireChanged();

private:
	// Used to ignite the brick, server only
	void SetIsOnFire(bool bNewOnFire);

	// Called on the cluster root when a brick has caught fire or was extinguished
	void OnChildBrickOnFireChanged(UBrick* Brick, bool bNewOnFire);

	// Adds a brick to the list of bricks on fire
	void AddChildBrickOnFire(UBrick* Brick);

	// Registers or unregisters the cluster root with the vehicle
	void UpdateClusterRootOnFire(bool bIsDestructing = false);

	// Sets up the timer for the next burn interval
	void SetupBurnInterval();

	// Spreads fire to connected bricks and other objects in the world
	void SpreadFire(float SpreadProbability);

public:
	// Used to update fire on the part root
	void TickFire(float DeltaTime, const UBrickVehicleStaticInfo* VehicleStaticInfo);

	// Whether the cluster is on fire, this may only be called on the cluster root
	bool IsClusterRootOnFire() const
	{
		if (IsBrickClusterRoot())
		{
			const auto& FireParams = PartRootParams->ClusterRootParams->FireParams;
			return FireParams.IsValid() && FireParams->bIsClusterOnFire && FireParams->BricksOnFire.Num();
		}

		return false;
	}

	// Used by the fire spawn module to initialize the location and velocity of fire particles
	void SpawnFireParticle(FVector& OutLocation, FVector& OutVelocity);

	// Moves the part root to the desired spawn transform
	void TeleportPartRoot(const FTransform& SpawnTransform);

	// Called before repairing to move part roots to the new spawn location for example
	virtual void PreRepairBrick(const FTransform& SpawnTransform);

	// Resets the brick state
	virtual void RepairBrick();

	// Blueprint version
	UFUNCTION(BlueprintImplementableEvent)
	void Repair();

	// Called from the vehicle to apply damage to the brick
	void ReceiveDamage(int32 DamageDepth, float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	// Applies an impulse to the root component
	void ApplyDamageImpulse(const FDamageEvent& DamageEvent, const FHitResult& Hit) const;

	// For IBrickContactModifyCallback events forwarded from the vehicle
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo) const;

	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx, const UBrickVehicleStaticInfo* VehicleStaticInfo);

	// Returns the damage required to break the brick off
	float GetBreakOffDamage() const;

protected:
	// Called whenever the brick has taken direct or indirect damage
	virtual void ReceiveDamageInternal(int32 DamageDepth, float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	// ~Damage

public:
	// ~Fuel
	// Returns whether this brick is a fuel tank
	bool IsFuelTank() const
	{
		return GetFuelTankRuntimeParams() != nullptr;
	}

protected:
	// Implement for fuel tanks
	virtual const FFuelTankRuntimeParams* GetFuelTankRuntimeParams() const
	{
		return nullptr;
	}

public:
	// Returns the total fuel capacity of the brick
	virtual float GetFuelCapacity() const;

	// Returns the fuel type stored in the brick
	virtual TSubclassOf<UExplosiveMaterial> GetFuelType() const;

	// Helper to return the CDO of the fuel type class
	const UExplosiveMaterial* GetFuelTypeCDO() const;

	// Returns the current fuel level of the brick
	float GetFuelLevel() const;

	// Blows up this brick, assuming that it's a fuel tank
	void ExplodeFuelTank();

protected:
	// Consumes the given amount of fuel instantly
	bool ConsumeFuelInstant(const float Amount);

	// Consumes fuel at the given rate
	bool ConsumeFuelRate(const float Rate, const float DeltaTime);

	// Whether the brick is connected to tanks with fuel
	bool HasAnyFuel() const
	{
		return bHasAnyFuel;
	}

	// Called whenever the fuel flag has changed
	virtual void OnHasAnyFuelChanged()
	{
	}

private:
	void SetHasAnyFuel(bool bNewHasFuel)
	{
		if (bNewHasFuel != bHasAnyFuel)
		{
			bHasAnyFuel = bNewHasFuel;

			// Only notify the brick if it has already been initialized
			if (IsBrickEditorObjectInitialized())
			{
				OnHasAnyFuelChanged();
			}
		}
	}

	// ~Fuel

public:
	// ~Debug
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	// These are
	void DrawDebugConnectors();
	void DrawDebugFluidDynamics();

private:
	// Used to visualize a fluid dynamic surface
	void DrawDebugFluidDynamicSurface(const FFluidDynamicSurface& Surface, const FTransform& Transform);
#endif
	// ~Debug

protected:
	// Property callbacks
	static bool StaticIsBrickPatternSupported(const FBrickPropertyContainer& Container, UObject* Object);
	static bool StaticIsBrickMaterialSupported(const FBrickPropertyContainer& Container, UObject* Object);

	// Can be implemented by subclasses to restrict the use of patterns or materials
	virtual bool IsBrickPatternSupported(const UBrickPattern* InPattern) const;
	virtual bool IsBrickMaterialSupported(const UBrickMaterial* InMaterial) const;

public:
	// ~IBrickPropertyInterface
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	virtual void PostModifyBrickProperty(const FBrickPropertyChangedEvent& Event) override;
	// ~IBrickPropertyInterface
};
