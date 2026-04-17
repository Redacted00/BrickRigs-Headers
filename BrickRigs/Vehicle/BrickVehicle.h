// Copyright Fluppisoft, 2015

#pragma once

#include "Physics/BrickPhysicsReplication.h"
#include "Bricks/WinchBrick.h"
#include "Inventory/InventoryLoadout.h"
#include "Game/SpawnPoint.h"
#include "BrickVehicleStaticInfo.h"
#include "Components/BrickVehicleComponent.h"
#include "VehicleDamage.h"
#include "UGC/UGCTypes.h"
#include "FuelLevel.h"
#include "VehicleInputChannel.h"
#include "Stats/Stats.h"
#include "Bricks/TankBrick.h"
#include "Bricks/MotorBrick.h"
#include "BrickConnection.h"
#include "World/FireInterface.h"
#include "Misc/BrickTeamId.h"
#include "Player/ViewTargetInterface.h"
#include "Player/BrickPawnInterface.h"
#include "Character/BrickCharacter.h"
#include "GameFramework/Pawn.h"
#include "Physics/ContactModifyInterface.h"
#include "BrickVehicle.generated.h"

// Components
class UHUDIconComponent;
class UFirearmComponent;
class UInventoryComponent;
class UDestructibleInstancesComponent;
// Bricks
class UBrick;
class USeatBrick;
class UCameraBrick;
class UGunBrick;

// Performance stats
DECLARE_STATS_GROUP(TEXT("FluidDynamics"), STATGROUP_FluidDynamics, STATCAT_Advanced);

DECLARE_CYCLE_STAT(TEXT("TickFluidDynamics"), STAT_TickFluidDynamics, STATGROUP_FluidDynamics);
DECLARE_CYCLE_STAT(TEXT("TickClusterFluidDynamics"), STAT_TickClusterFluidDynamics, STATGROUP_FluidDynamics);
DECLARE_CYCLE_STAT(TEXT("TickPartFluidDynamics"), STAT_TickPartFluidDynamics, STATGROUP_FluidDynamics);
DECLARE_CYCLE_STAT(TEXT("InitElements"), STAT_InitFluidDynamicElements, STATGROUP_FluidDynamics);
DECLARE_CYCLE_STAT(TEXT("IterateElements"), STAT_IterateFluidDynamicElements, STATGROUP_FluidDynamics);
DECLARE_CYCLE_STAT(TEXT("AddForce"), STAT_AddFluidDynamicForce, STATGROUP_FluidDynamics);

DECLARE_CYCLE_STAT(TEXT("Construct Vehicle"), STAT_Vehicle_Construct, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Construct Vehicle - CalcConnections"), STAT_Vehicle_Construct_CalcConnections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Construct Vehicle - InitConnections"), STAT_Vehicle_Construct_InitConnections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Construct Vehicle - PostInitConnections"), STAT_Vehicle_Construct_PostInitConnections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Construct Vehicle - PostConstruct"), STAT_Vehicle_Construct_PostConstruct, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Repair Vehicle"), STAT_Vehicle_Repair, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Handle Vehicle Collisions"), STAT_Vehicle_HandleCollisions, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections"), STAT_Vehicle_UpdateBrickConnections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - Connections"), STAT_Vehicle_UpdateBrickConnections_Connections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - PreGatherConnectedBricks"), STAT_Vehicle_UpdateBrickConnections_PreGatherConnectedBricks, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - BricksToUpdate"), STAT_Vehicle_UpdateBrickConnections_BricksToUpdate, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - Bricks"), STAT_Vehicle_UpdateBrickConnections_Bricks, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - Clusters"), STAT_Vehicle_UpdateBrickConnections_Clusters, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - UpdateClusterRoot"), STAT_Vehicle_UpdateBrickConnections_UpdateClusterRoot, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - Parts"), STAT_Vehicle_UpdateBrickConnections_ActivateConnections, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - ReplicateDamage"), STAT_Vehicle_UpdateBrickConnections_ReplicateDamage, STATGROUP_Game);
DECLARE_CYCLE_STAT(TEXT("Update Brick Connections - PostUpdateConnections"), STAT_Vehicle_UpdateBrickConnections_PostUpdateConnections, STATGROUP_Game);

UENUM()
enum class EVehiclePinMode : uint8
{
	None,
	RootBrick,
	AllBricks
};

// The main vehicle actor class
UCLASS()
class BRICKRIGS_API ABrickVehicle : public APawn, public IBrickPawnInterface, public IViewTargetInterface, public IGenericTeamAgentInterface, public ISpawnPointInterface, public IFireInterface, public IWinchBrickInterface, public IPhysicsReplicationInterface, public IContactModifyInterface
{
	GENERATED_BODY()

	// ~Types
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUGCDownloadProgressChanged, bool, float);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnPassengerEnteredOrLeft, ABrickCharacter*, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnFuelLevelChanged, float, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPinModeChanged, EVehiclePinMode);

	// Used for the UpdateBrickConnections function
	enum class EUpdateBrickConnectionsMode : uint8
	{
		Initialize,
		InitializeDynamic,
		Repair,
		Break,
		Destroy
	};

	// ~Types

	// ~Variables
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	FConsoleVariableSinkHandle CVarSinkHandle;
#endif

	// File info the vehicle has been spawned from
	UPROPERTY(Transient, Replicated)
	FUGCFileInfo VehicleFileInfo;
	// Last location the vehicle has been restored at
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RestartTransform)
	FPawnRestartTransform RestartTransform;
	UFUNCTION()
	void OnRep_RestartTransform();
	// Replicated brick movement states
	UPROPERTY(Transient, ReplicatedUsing = OnRep_RepPhysicsMovement)
	FRepPhysicsMovement RepPhysicsMovement;
	UFUNCTION()
	void OnRep_RepPhysicsMovement();
	// Whether vehicle construction has been started yet
	uint32 bStartedConstructVehicle : 1;
	// Whether initial collision should be avoided after constructing the vehicle
	uint32 bAvoidCollisionOnConstruct : 1;
	// Whether the UGC data was replicated after initial replication
	uint32 bDelayedUGCFileReplication : 1;
	// Whether the vehicle has been constructed
	uint32 bIsVehicleConstructed : 1;
	// Set to true while connections are being initialized
	uint32 bIsInitializingBrickConnections : 1;
	// Whether the physics are locally authoritative
	uint32 bHasPhysicsAuthority : 1;
	// Flags set to true if a value has been replicated
	uint32 bRestartTransformReplicated : 1;
	uint32 bConnectionDamageReplicated : 1;
	// Used to track changes through replication
	uint32 bSavedCanBeDamaged : 1;
	// The replicated download progress in the range of 0-100, used to show clients the progress
	// NOTE: A value of 255 is reserved to indicate clients they can spawn their vehicle locally
	UPROPERTY(Transient, ReplicatedUsing = OnRep_UGCDownloadProgress)
	uint8 UGCDownloadProgress;
	UFUNCTION()
	void OnRep_UGCDownloadProgress();
	// Vehicle bounding box (relative to the root), cached for better performance (and because it can be modified by axles)
	FVector VehicleBoundsMin;
	FVector VehicleBoundsMax;
	// Spawn price of this vehicle
	float VehiclePrice;
	// Last world time the vehicle has been repaired
	float LastRepairTime;
	// Last time a movement update has been sent
	float LastMovementReplicationTime;
	// Maximum steering angle of all axles
	float MaxSteeringAngle;
	// The player that has spawned this vehicle
	UPROPERTY(Transient, Replicated)
	ABrickPlayerState* SpawningPlayerState;
	// The actor the vehicle has been spawned at, this needs to be replicated so clients also know the required spawn offset
	UPROPERTY(Transient, Replicated)
	AActor* OriginalSpawnPoint;

	// List of all connections between bricks, active and broken
	UPROPERTY(Transient)
	TArray<UBrickConnection*> BrickConnections;
	// List of dynamic connections created by this vehicle
	UPROPERTY(Transient)
	TArray<UBrickConnection*> DynamicBrickConnections;
	// List of dynamic connections involved this vehicle but owned by another one
	UPROPERTY(Transient)
	TArray<UBrickConnection*> ExternalBrickConnections;
	// Connections that are waiting to be broken, done at once for better performance
	UPROPERTY(Transient)
	TArray<UBrickConnection*> BrickConnectionsToBreak;
	// Contains replicated connection damage information
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BrickConnectionDamageArray)
	FBrickConnectionDamageArray BrickConnectionDamageArray;
	UFUNCTION()
	void OnRep_BrickConnectionDamageArray();

	// Used to replicate per brick damage info
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BrickDamageArray)
	FBrickDamageArray BrickDamageArray;
	UFUNCTION()
	void OnRep_BrickDamageArray();
	// Stores and replicates the current fuel level
	UPROPERTY(Transient, ReplicatedUsing = OnRep_FuelLevel)
	FFuelLevel FuelLevel;
	UFUNCTION()
	void OnRep_FuelLevel(const FFuelLevel& PrevFuelLevel);
	// Whether the vehicle is currently pinned in place
	UPROPERTY(Transient, ReplicatedUsing = OnRep_PinMode)
	EVehiclePinMode PinMode;
	UFUNCTION()
	void OnRep_PinMode();
	// The unique teams of all passengers
	// NOTE: This has to be replicated since characters can be culled when far away, which means the client would not know about the team affiliation
	UPROPERTY(ReplicatedUsing = OnRep_PassengerTeamIds)
	FGenericTeamIdSet PassengerTeamIds;
	UFUNCTION()
	void OnRep_PassengerTeamIds();
	// List populated on clients
	TArray<FBrickEditorObjectID> FuelTanksToExplodeOnClient;

	// Root brick of the vehicle, usually the driver seat
	UPROPERTY(Transient)
	UBrick* RootBrick;
	// List of all cluster root bricks
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> ClusterRootBricks;
	// Part roots for which movement is being replicated
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> MovementReplicationBricks;
	// List of all cluster roots that have fluid dynamic elements
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> ClusterRootBricksWithFluidDynamics;
	// List of all cluster roots with bricks on fire
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> ClusterRootBricksOnFire;
	// List of bricks that are replicated
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> ReplicatedBricks;
	// List of seats on this vehicle
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> SeatBricks;
	// List of camera bricks
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> CameraBricks;
	// List of all guns
	UPROPERTY(Transient)
	TArray<FBrickEditorObjectID> GunBricks;
	// Cached motor
	UPROPERTY(Transient)
	UMotorBrick* MotorBrick;

	// Whether the third person camera is currently in interaction mode
	uint8 bIsInteracting : 1;
	// Whether the view rotation has been initialized yet
	uint8 bInitializedViewRotation : 1;
	// View rotation of the camera when interaction mode was entered
	FRotator PreInteractionViewRotation;
	// Transform the view rotation is relative to
	FVector ViewRefLocation;
	FQuat ViewRefRotation;
	FVector ViewRefLinVel;
	// Cached camera zoom values
	mutable FViewTargetZoomCache ZoomCache;

	// These keys are used to keep track of replication changes
	uint16 BricksReplicationKey;
	uint16 ConnectionsReplicationKey;
	uint16 FirearmsReplicationKey;

	// The cached loadout used to restore slots upon repairing
	FInventoryLoadout InventoryLoadout;
	// ~Variables

	// ~Components
	// The vehicle root, responsible for managing the bricks, serialization etc.
	UPROPERTY(Transient)
	UBrickVehicleComponent* VehicleComponent;
	UPROPERTY(Transient)
	UAudioComponent* HandlingAudioComponent;
	// Vehicle icon component
	UPROPERTY(Transient)
	UHUDIconComponent* HUDIconComponent;
	// A list of firearm components from all bricks
	UPROPERTY(Transient)
	TArray<UFirearmComponent*> FirearmComponents;
	// The inventory used for ammo etc.
	UPROPERTY(Transient)
	UInventoryComponent* InventoryComponent;
	// ~Components

public:
	// ~Delegates
	FOnUGCDownloadProgressChanged OnUGCDownloadProgressChangedDelegate;
	FSimpleMulticastDelegate OnVehicleConstructedDelegate;
	FSimpleMulticastDelegate OnVehicleRepairedDelegate;
	// NOTE: This is also broadcast when the player state of a passenger has changed
	FOnPassengerEnteredOrLeft OnPassengerEnteredOrLeftDelegate;
	FOnFuelLevelChanged OnFuelLevelChangedDelegate;
	FOnPinModeChanged OnPinModeChangedDelegate;
	// ~Delegates

protected:
	// ~Properties
	// The static property info class
	UPROPERTY(EditDefaultsOnly, Category = Vehicle)
	TSubclassOf<UBrickVehicleStaticInfo> StaticInfoClass;
	// ~Properties

public:
	// ~Constructor
	ABrickVehicle();

	// ~Super Interface
	virtual void Reset() override;
	virtual void PreNetReceiveSubobjects(const FReplicationFlags& RepFlags) override;
	virtual void PreNetReceive() override;
	virtual void PostNetReceive() override;
	virtual void PostRepNotifies() override;
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	virtual void GatherCurrentMovement() override;
	virtual void OnRep_ReplicatedMovement() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Destroyed() override;
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual void Tick(float DeltaTime) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	virtual void BecomeViewTarget(APlayerController* PC) override;
	virtual void EndViewTarget(APlayerController* PC) override;
	virtual void BecomeViewTargetSubobject(APlayerController* PC) override;
	virtual void EndViewTargetSubobject(APlayerController* PC) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual FRotator GetViewRotation() const override;
	virtual FVector GetPawnViewLocation() const override;

	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;
	virtual FVector GetVelocity() const override;

	virtual bool CanBeDamaged() const override
	{
		// Don't allow damage while the vehicle is fully pinned
		return Super::CanBeDamaged() && PinMode != EVehiclePinMode::AllBricks;
	}

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakeRadialDamage(float Damage, const FRadialDamageEvent& RadialDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual float InternalTakePointDamage(float Damage, const FPointDamageEvent& PointDamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual bool CheckStillInWorld() override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	// ~Super Interface

	// ~Debug
	UFUNCTION(BlueprintCallable, Exec)
	void DebugVehicleClusters();
	// ~Debug

	// ~Static Info
	UFUNCTION(BlueprintPure)
	const UBrickVehicleStaticInfo* GetStaticInfo() const
	{
		return StaticInfoClass != nullptr ? StaticInfoClass->GetDefaultObject<UBrickVehicleStaticInfo>() : GetDefault<UBrickVehicleStaticInfo>();
	}

	// ~Static Info

	// ~Vehicle
	// Public accessor to the vehicle component
	UFUNCTION(BlueprintPure)
	UBrickVehicleComponent* GetVehicleComponent() const
	{
		return VehicleComponent;
	}

	// Whether the vehicle has been constructed yet
	UFUNCTION(BlueprintPure)
	bool IsVehicleConstructed() const
	{
		return bIsVehicleConstructed;
	}

	// Get the replicated download percentage, returns true if downloading
	bool GetUGCDownloadProgress(float& OutProgress) const
	{
		OutProgress = (UGCDownloadProgress - 1) / 255.f;
		return UGCDownloadProgress > 0 && UGCDownloadProgress < 255;
	}

	// Get the file the vehicle was constructed from
	UFUNCTION(BlueprintPure)
	const FUGCFileInfo& GetVehicleFileInfo() const
	{
		return VehicleFileInfo;
	}

	// Get the player state who spawned this vehicle
	UFUNCTION(BlueprintPure)
	ABrickPlayerState* GetSpawningPlayerState() const
	{
		return SpawningPlayerState;
	}

	// Get the player who spawned this vehicle
	ABrickPlayerController* GetSpawningPC() const;

	// Spawns or respawns a vehicle
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ConstructVehicle(const FUGCFileInfo& InFileInfo, AActor* InSpawnPoint, ABrickPlayerController* InSpawningPC, bool bAvoidCollision = true);

	// Returns the price the vehicle was spawned for (server only)
	float GetVehicleSpawnPrice() const
	{
		return VehiclePrice;
	}

private:
	// Constructs the vehicle from the file info
	void ConstructVehicleInternal();

	// Called when the vehicle has been constructed or deconstructed
	void OnIsVehicleConstructedChanged();

public:
	// Respawns the vehicle at the given transform, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void RepairVehicle(const FVector& InLocation, const FRotator& InRotation, AActor* InSpawnPoint, bool bAvoidCollision = true);

private:
	// Repairs the vehicle on server and clients
	void RepairVehicleInternal();

	// Changes the current download progress
	void SetUGCDownloadProgress(uint8 NewProgress);

	// Called any time the download progress has changed
	void OnUGCDownloadProgressChanged();

	// Called after the bricks have been loaded, initializes connections etc.
	void OnUGCItemLoaded(bool bSuccess, TArray<uint8>* ItemData);

	// Should be called after spawning or repairing, sets up the fuel etc.
	void PostConstructOrRepair();

	// Get the desired offset to apply after spawning/repairing the vehicle
	void GetVehicleSpawnOffset(FVector& InOutSpawnLocation, const FQuat& SpawnRotation, AActor* InSpawnPoint, bool bAvoidCollision) const;

public:
	// Removes the vehicle from the level, server only
	void ScrapVehicle();

	// Breaks all brick connections, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void ExplodeVehicle();

	// Updates the destruction timer
	void UpdateAutoRemovalTimer();

private:
	// Callback for the game state
	void OnMatchSettingsChanged(const FMatchSettings& MatchSettings);

public:
	// Returns the current pin mode
	UFUNCTION(BlueprintPure)
	EVehiclePinMode GetPinMode() const
	{
		return PinMode;
	}

	// Changes the pin mode, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetPinMode(EVehiclePinMode NewMode);

protected:
	// Called whenever the pin mode has changed
	void OnPinModeChanged();

public:
	// Public accessor to the inventory
	UFUNCTION(BlueprintPure)
	UInventoryComponent* GetInventoryComponent() const
	{
		return InventoryComponent;
	}

	// Get the amount of ammo stored in the vehicle by default
	UFUNCTION(BlueprintPure)
	int32 GetAmmoCapacity() const;

	// Get the amount of ammo currently stored in the inventory
	UFUNCTION(BlueprintPure)
	int32 GetAmmo() const;

	// Get the min and max gear
	void GetGearRange(FInt32Interval& OutRange) const
	{
		if (MotorBrick)
		{
			MotorBrick->GetGearRange(OutRange);
		}
		else
		{
			OutRange.Min = 0;
			OutRange.Max = 0;
		}
	}

	// Returns the current RPM of the motor(s)
	UFUNCTION(BlueprintPure)
	float GetCurrentRPM() const;

	// Returns the maximum possible RPM
	UFUNCTION(BlueprintPure)
	float GetMaxRPM() const;

	// Get the min and max RPM the user should stay at
	void GetIdealRPMRange(FFloatInterval& OutRange) const;

	// Returns the cached max steering angle of all axles
	auto GetMaxSteeringAngle() const
	{
		return MaxSteeringAngle;
	}

	// Get the number of passengers currently in the vehicle
	UFUNCTION(BlueprintPure)
	int32 GetNumPassengers() const;

	// Get all characters in this vehicle
	UFUNCTION(BlueprintCallable)
	void GetPassengers(TArray<ABrickCharacter*>& OutPassengers) const;

	// Returns true if the vehicle has no passengers or only the given one
	bool HasOnlyPassenger(const ABrickCharacter* InCharacter) const;

	// Returns all seats of the vehicle
	UFUNCTION(BlueprintPure)
	const TArray<FBrickEditorObjectID>& GetSeats() const
	{
		return SeatBricks;
	}

	// Get the seat used as the driver seat
	UFUNCTION(BlueprintPure)
	USeatBrick* GetDriverSeat() const;

	// Get the brick ID of the driver seat
	const FBrickEditorObjectID& GetDriverSeatID() const;

	// Whether the given seat ID exists on the vehicle
	bool IsValidSeatID(const FBrickEditorObjectID& InSeatID) const;

	// Whether the given seat ID belongs to the driver seat
	bool IsDriverSeatID(const FBrickEditorObjectID& InSeatID) const
	{
		return GetDriverSeatID() == InSeatID;
	}

	// Try to find the seat with the given ID
	USeatBrick* FindSeatByID(const FBrickEditorObjectID& InSeatID) const;

	// Whether the vehicle has any visible seat
	UFUNCTION(BlueprintPure)
	bool HasVisibleSeat() const;

	// Get the character sitting in a specific seat
	ABrickCharacter* GetCharacterInSeat(const FBrickEditorObjectID& InSeatID) const;

	// Get the character in the driver seat
	UFUNCTION(BlueprintPure)
	ABrickCharacter* GetDriverCharacter() const;

	// Returns either the driver or the first passenger if there is no driver
	ABrickCharacter* GetDriverOrFirstPassenger() const;

	// Whether there is currently a driver in the vehicle
	bool HasDriver() const;

private:
	// Updates the current team affiliation of the vehicle based on all passengers
	void UpdatePassengerTeamIds();

	// Callbacks for passenger characters
	void OnPassengerPlayerStateChanged(ABrickPlayerState* NewPlayerState);

	void OnPassengerStateOfHealthChanged(ECharacterStateOfHealth NewStateOfHealth);

	// Callback for firearm components
	void OnFirearmReplicationKeyChanged();

public:
	// Allows guns to add their replicated firearm components
	void AddFirearmComponent(UFirearmComponent* InComp);

	// Callback for the inventory
	void OnInventorySlotModified(const FInventorySlot& InSlot, EInventorySlotEvent Event);

	// Resets the input state
	void ResetInput();
	// ~Vehicle

	// ~Connections
	// Can be used to dynamically connect two bricks during gameplay, for example with couplings
	void CreateDynamicBrickConnection(UBrick* Brick0, UBrick* Brick1, const FBrickConnectionParams& Params);

	// Activates a dynamic connection
	void ActivateDynamicBrickConnection(UBrickConnection* InConnection);

	// Breaks a dynamic connection
	void BreakDynamicBrickConnection(UBrickConnection* InConnection);

	// Breaks all dynamically created connections, or just the ones against another vehicle
	void BreakDynamicConnections(bool bIsBeingDestroyed = false);

private:
	// Creates and initialize a new connection
	UBrickConnection* CreateBrickConnection(UBrick* Brick0, UBrick* Brick1, FBrickConnectionParams Params, bool bIsDynamic);

public:
	// Adds a list of connections to the breaking queue
	void QueueBrickConnectionsToBreak(const TArray<UBrickConnection*>& InConnections);

	// Repairs multiple connections
	void RepairBrickConnections(const TArray<UBrickConnection*>& InConnections);

	// Whether the local client has authority over collision damage
	bool HasCollisionDamageAuthority() const;

private:
	// Whether the given damage amount should break off the brick
	bool ShouldBrickBreakOff(const UBrick* Brick, float DamageAmount, TOptional<float>& OutAbsorbedDamage) const;

	// Internal version called for the own and other vehicles
	void BreakDynamicConnectionsInternal(bool bIsBeingDestroyed, ABrickVehicle* OtherVehicle);

	// Processes all connections that are added to the breaking queue
	void ProcessBreakBrickConnectionsQueue(bool bIsBeingDestroyed = false);

	// This is the main function used to initialize, break or repair connections
	void UpdateBrickConnections(const TArray<UBrickConnection*>& InConnections, EUpdateBrickConnectionsMode Mode, bool bCalledFromOtherVehicle = false);

	// Allows clients to send damaged connections to the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBreakConnections(const FBrickConnectionDamageArray& InDamage, const FRestartTransformVersion& ComparisonIndex);

	// Used to determine if a part should simulate physics or not
	template <typename T>
	bool ShouldPartRootSimulatePhysics(T IsConnectedToRootBrick) const
	{
		switch (PinMode)
		{
		case EVehiclePinMode::RootBrick:
			return !IsConnectedToRootBrick();
		case EVehiclePinMode::AllBricks:
			return false;
		default:
			return true;
		}
	}

	// ~Connections

public:
	// ~Damage
	// Called when the vehicle ran over destructible instances
	void OnBrokeDestructibleInstances(UDestructibleInstancesComponent* Comp, const TMap<int32, TArray<int32>>& Instances);

	// Called by seats to damage characters
	void ApplySeatCollisionDamage(USeatBrick* Seat, float DamageAmount);

private:
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOnBrokeDestructibleInstances(UDestructibleInstancesComponent* Comp, const TArray<uint16>& Instances, const FRestartTransformVersion& ComparisonIndex);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerApplySeatCollisionDamage(const FBrickEditorObjectID& SeatID, float DamageAmount, const FRestartTransformVersion& ComparisonIndex);

	// Called upon receiving point or radial damage
	void TakePointOrRadialDamage(const FDamageEvent& DamageEvent, const TArray<FHitResult>& Hits);
	// To be called before and after damage is applied to bricks
	void PreReceiveBrickDamage();
	void PostReceiveBrickDamage();
	// ~Damage

public:
	// ~Fuel
	// Get the total fuel capacity
	UFUNCTION(BlueprintPure)
	float GetFuelCapacity() const
	{
		return FuelLevel.GetCapacity();
	}

	// Get the absolute fuel level in liters
	UFUNCTION(BlueprintPure)
	float GetFuelLevel() const
	{
		return FuelLevel.GetAbsolute();
	}

	// Get the normalized fuel level from 0-1
	float GetRelativeFuelLevel() const
	{
		return FuelLevel.GetRelative();
	}

private:
	// Initialize the fuel capacity
	void SetFuelCapacity(float NewCapacity);

	// Sets the current fuel level, server only
	void SetFuelLevel(float NewLevel);

	// Called whenever the fuel level has changed
	void OnFuelLevelChanged(float OldLevel);

	// Get the fuel level/capacity from the root cluster
	float CalcFuelLevel() const;
	// ~Fuel

public:
	// ~Camera
	// Sets the third person camera to rotate locally, rather than around the vehicle
	void SetIsInteracting(bool bNewInteracting);

	// Whether the camera is currently in interaction mode
	UFUNCTION(BlueprintPure)
	bool IsInteracting() const
	{
		return bIsInteracting;
	}

	// Returns the seat the viewed character is in
	UFUNCTION(BlueprintPure)
	USeatBrick* GetViewedSeat() const;

	// Returns the camera brick that is currently our view target subobject
	UFUNCTION(BlueprintPure)
	UCameraBrick* GetViewTargetCamera() const;

private:
	// Updates the view reference transform
	void UpdateViewRefTransform(float DeltaTime, bool bInit = false);

	// Returns the world pivot of the third person camera
	FVector GetCameraPivot() const;

	// Internal version used to get the view rotation for an arbitrary reference rotation
	FRotator GetViewRotationInternal(const FRotator& InViewRotation) const;
	// ~Camera

public:
	// ~Sound
	// Helper function to play any handling sound
	UFUNCTION(BlueprintCallable)
	void PlayHandlingSound(USoundBase* Sound);

	// Helper function to activate the generic switch sound
	void PlaySwitchSound();
	// ~Sound

	// ~Bricks
	// Called from bricks to update their replicated damage state
	void SetReplicatedBrickDamage(UBrick* Brick, const FBrickDamage& NewDamage);

	// Add the actors that should be ignored during sensors traces
	void GetSensorsTraceIgnoredActors(FCollisionQueryParams& OutParams) const;

	// To be called when a client wants to blow up a tank from an explosion
	void ExplodeTankBrickOnClient(UTankBrickBase* Tank);

	// Sends fuel tanks to explode to the server if needed
	void ServerExplodeTankBricks(bool bForceUpdate);

private:
	// Used to send tanks to explode to the server
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerExplodeTankBricks(const TArray<FBrickEditorObjectID>& Tanks, const FRestartTransformVersion& ComparisonIndex);
	// ~Bricks

	// ~UI
	// HUD icon component callbacks
	bool ShouldIconBeVisible(ABrickPlayerController* OtherPC, const FHUDIconContext& Context) const;

	float GetIconMaxDrawDist(ABrickPlayerController* OtherPC) const;

	FVector GetIconLocation() const;

public:
	// Creates the scrap vehicle option
	FInteractionOption MakeScrapInteractionOption(ABrickPlayerController* OtherPC, const FInteractionOptions& Options);

private:
	void GetIconInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);

	UFUNCTION()
	void Interact_EnterVehicle(ABrickPlayerController* OtherPC);

	UFUNCTION()
	void Interact_ScrapVehicleFromInteraction(ABrickPlayerController* OtherPC);

	UFUNCTION()
	void Interact_ScrapVehicleFromMap(ABrickPlayerController* OtherPC);
	// ~UI

public:
	// ~Thumbnails
	// Whether a thumbnail for this vehicle can be captured
	bool CanCaptureThumbnail() const;

	// Captures a new thumbnail for this vehicle
	void CaptureThumbnail();
	// ~Thumbnails

	// ~IBrickPawnInterface
	virtual void GetTeamAffiliation(TSet<FGenericTeamId>& OutTeams) const override;
	virtual void GetPawnBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const override;
	virtual FTransform GetPawnRestartTransform() const override;
	virtual void OnCanBeDamagedChanged() override;
	// ~IBrickPawnInterface

	// ~Replication
	// Queues a replication update
	void MarkBrickReplicationDirty(UBrick* Brick);

	// Public access to the restart transform comparison index
	const auto& GetRestartTransformVersion() const
	{
		return RestartTransform.GetComparisonIndex();
	}

	// Whether the local client has authority over physics
	bool HasPhysicsAuthority() const
	{
		return bHasPhysicsAuthority;
	}

private:
	// Sends the current movement state to the server
	UFUNCTION(Server, Unreliable, WithValidation)
	void ServerUpdateMovement(const FRepPhysicsMovement& Movement, const FRestartTransformVersion& ComparisonIndex);

	// Gathers the replicated vehicle movement state
	void GatherVehicleMovement();

	// To be called whenever the vehicle might receive or lose physics authority
	void UpdateHasPhysicsAuthority();

	// Updates the replication physics state target
	void UpdateReplicatedPhysicsState(bool bTeleported);
	// ~Replication

	// ~Bricks
	// Applies fluid dynamic forces
	void TickFluidDynamics(float DeltaTime);

	// Lets the cluster roots apply fire damage, spread fire etc.
	void TickFire(float DeltaTime);

public:
	// Adds or removes a cluster root from the list of clusters on fire
	void SetBrickClusterRootOnFire(UBrick* ClusterRoot, bool bNewOnFire);

	// Gets the current world origin of the vehicle bounds
	FVector GetVehicleBoundsOrigin(bool bMoveToBottom = false) const;

	// Get the time seconds since the last repair or recover
	float TimeSinceLastRepair() const;

	// Return vehicle damage condition in range 0-1
	float GetVehicleCondition() const;

	// Get the total number of bricks on the vehicle
	int32 GetNumBricks() const
	{
		return VehicleComponent->GetNumBrickEditorObjects();
	}

	// Returns all bricks this vehicle owns
	const auto& GetBricks() const
	{
		return VehicleComponent->GetBrickEditorObjects<UBrick>();
	}

	// Get all bricks of a specific class
	template <class T>
	void GetBricksByClass(TArray<T*>& OutBricks) const
	{
		return VehicleComponent->GetBrickEditorObjectsByClass<UBrick, T>(OutBricks);
	}

	// Get IDs of all bricks with the given type
	template <class T>
	void GetBrickIDsByClass(TArray<FBrickEditorObjectID>& OutBricks) const
	{
		return VehicleComponent->GetBrickEditorObjectIDsByClass<UBrick, T>(OutBricks);
	}

	// Execute a function on every brick
	void ForEachBrick(const TFunction<void(UBrick*)>& Function) const
	{
		VehicleComponent->ForEachBrickEditorObject<UBrick, UBrick>(Function);
	}

	// Execute a function on every part root of a given type
	template <bool bClusters = true, bool bParts = true, bool bChildren = true>
	void ForEachPartRoot(const TFunction<void(UBrick*)>& Function) const
	{
		for (const auto& ClusterRootID : ClusterRootBricks)
		{
			if (auto* ClusterRoot = GetBrickByID(ClusterRootID))
			{
				ClusterRoot->ForEachConnectedBrick<UBrick, true, true, true, false>(Function);
			}
		}
	}

	// Return the first brick of the given type
	template <class T>
	T* GetBrickByClass() const
	{
		return VehicleComponent->GetBrickEditorObjectByClass<UBrick, T>();
	}

	// Try to find a brick with a specific ID
	template <class T = UBrick>
	T* GetBrickByID(const FBrickEditorObjectID& InID) const
	{
		return VehicleComponent->GetBrickEditorObjectByID<UBrick, T>(InID);
	}

	// Get the main brick, usually the driver seat
	UFUNCTION(BlueprintPure)
	UBrick* GetRootBrick() const
	{
		return RootBrick;
	}

	UFUNCTION(BlueprintPure)
	const TArray<FBrickEditorObjectID>& GetCameras() const
	{
		return CameraBricks;
	}

	UFUNCTION(BlueprintPure)
	const TArray<FBrickEditorObjectID>& GetGuns() const
	{
		return GunBricks;
	}

	UFUNCTION(BlueprintPure)
	UMotorBrick* GetMotor() const
	{
		return MotorBrick;
	}

	// ~Bricks

	// ~Entering and Exiting
	// Try to find the next seat that is currently unoccupied
	USeatBrick* GetNextUnoccupiedSeat(USeatBrick* StartSeat = nullptr, bool bForward = true) const;

	// Force ejects all characters currently in the vehicle
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void EjectAllCharacters();

	// Get the desired exit location for a character, NOTE that the InCharacter can also be the CDO
	FVector GetVehicleExitLocation(const ABrickCharacter* InCharacter, const FTransform& SeatTransform, const TOptional<FVector>& CustomExitLocation) const;

	// Adds a character to the passenger list, server only
	void AddPassenger(ABrickCharacter* InCharacter, const FBrickEditorObjectID& InSeatID);

	// Removes a character from the passenger list, server only
	void RemovePassenger(ABrickCharacter* InCharacter, const FBrickEditorObjectID& InSeatID);

	// These two functions are called respectively when a passengers enters or leaves, server and clients
	void OnPassengerEnteredVehicle(ABrickCharacter* InCharacter);

	void OnPassengerLeftVehicle(ABrickCharacter* InCharacter);
	// ~Entering and Exiting

	// ~IViewTargetInterface
	virtual const FViewTargetZoomParams* GetZoomParams() const override
	{
		return &GetStaticInfo()->ZoomParams;
	}

	virtual FViewTargetZoomCache* GetZoomCache() const override
	{
		return &ZoomCache;
	}

	virtual UObject* GetViewTargetSubobject() const override;
	virtual void UpdateViewRotation(FRotator& InOutRotation, float DeltaTime) override;
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual FVector GetViewTargetAirVelocity() const override;
	// ~IViewTargetInterface

	// ~IGenericTeamAgentInterface
	// NOTE: This is not to be used directly, since vehicles don't have a single team id
	virtual FGenericTeamId GetGenericTeamId() const override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	// ~IGenericTeamAgentInterface

private:
	// ~ISpawnPointInterface
	virtual bool IsAutoSpawnPoint() const override { return false; }
	virtual bool CanPlayerEnterSpawnPoint(ABrickPlayerController* PC) const override;
	virtual void EnterSpawnPoint(ABaseCharacter* InCharacter) override;
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const override;
	virtual bool GetSpawnPoint(ABrickPlayerController* PC, FVector& OutLocation, FRotator& OutRotation) const override;
	// ~ISpawnPointInterface

	// ~IFireInterface
	virtual bool IsOnFire(const FHitResult& Hit) const override;
	virtual bool SetOnFire(const FHitResult& Hit, APawn* DamageInstigator, AActor* DamageCauser) override;
	virtual bool ExtinguishFire(const FHitResult& Hit) override;
	// ~IFireInterface

	// ~IWinchBrickInterface

	// ~IWinchBrickInterface

	// ~IPhysicsReplicationInterface
	virtual void SetPhysicsReplicationState(const FSetPhysicsReplicationStateParams& Params) override;
	// ~IPhysicsReplicationInterface

	// ~ICollisionDamageInterface
	virtual void OnContactModify(const FContactModifyParams& Params, int32 ActorIdx) const override;
	virtual void PostContactModify(const FPostContactModifyParams& Params, int32 ActorIdx) override;
	// ~ICollisionDamageInterface
};
