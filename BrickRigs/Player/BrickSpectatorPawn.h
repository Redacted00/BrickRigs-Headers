// Copyright Fluppisoft, 2016

#pragma once

#include "PlayerSpawnRequest.h"
#include "UGC/UGCTypes.h"
#include "Player/ViewTargetInterface.h"
#include "GameFramework/SpectatorPawn.h"
#include "BrickSpectatorPawn.generated.h"

class ABrickPlayerController;
class UMaterialInstanceDynamic;

UENUM(BlueprintType)
enum class EFreeCamMode : uint8
{
	None,
	Default,
	FollowLocation,
	FollowLocationAndRotation,
	LookAt,
	Max
};

/**
 * This class is used for the free cam in sandbox and for spectating
 */
UCLASS(Abstract)
class BRICKRIGS_API ABrickSpectatorPawn : public ASpectatorPawn, public IViewTargetInterface
{
	GENERATED_BODY()

	// ~Variables
	// The current mode
	EFreeCamMode FreeCamMode;
	// Cached camera speed and zoom values
	mutable FViewTargetCameraSpeedCache CameraSpeedCache;
	mutable FViewTargetCameraSpeedCache ShiftCameraSpeedCache;
	mutable FViewTargetZoomCache ZoomCache;
	// Whether the place pawn button is being held down
	uint8 bWantsToPlacePawn : 1;
	// Whether the pawn is currently being placed
	uint8 bIsPlacingPawn : 1;
	// Whether the player wants to spawn a dummy rather than moving the character
	uint8 bIsSpawningDummy : 1;
	uint8 bUseShiftSpeed : 1;
	// Current interpolation ratio between normal and shift speed
	float ShiftSpeedRatio;
	// Cached request for the vehicle currently being spawned
	TOptional<FPlayerSpawnRequest> VehicleSpawnRequest;
	// The actor that is being followed
	UPROPERTY(Transient)
	AActor* FollowTarget;
	// Last known transform of the follow target
	TOptional<FTransform> LastFollowTargetTransform;
	// Accumulated rotation input
	float RotatePawnInput;
	// Yaw rotation offset of the pawn placement
	float PawnRotationOffset;
	// Current additional size of the placement bounds
	FVector PlacementBoundsSizeOffset;

	// The material instance used for the placement mesh
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// Cached material parameter index
	int32 BoundsParamIndex;
	// ~Variables

	// ~Components
	// The mesh we display before placing the pawn
	UPROPERTY(Transient)
	UStaticMeshComponent* PlacementBoundsMeshComponent;
	// ~Components

protected:
	// ~Properties
	// The mesh to display for the pawn placing bounds
	// NOTE: Not a soft pointer since it might have to be loaded a bunch of times in sandbox
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	UStaticMesh* PlacementBoundsMesh;
	// Default size of the placement bounds mesh without modifications
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	FVector PlacementBoundsDefaultSize;
	// Minimum size the placement bounds can be (visually)
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	FVector MinPlacementBoundsSize;
	// Speed at which the current visual bounds size is interpolated
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	float PlacementBoundsSizeInterpSpeed;
	// Determines how far away the placement bounds has to be to fit inside the viewport, larger numbers mean further away
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	float PlacementBoundsScreenFitInflation;
	// Radius of the sphere to use for placement sweeps
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	float PlacementSweepRadius;
	// Speed at which the pawn is rotated in degrees per second
	UPROPERTY(EditDefaultsOnly, Category = Spectator)
	float PawnRotationSpeed;

	// Camera speed params used for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetCameraSpeedParams CameraSpeedParams;
	// Zoom params to use for the view target interface
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetZoomParams ZoomParams;
	// Rate at which the speed is interpolated between normal and shift mode
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float ShiftInterpSpeed;
	// ~Properties

public:
	// ~Constructor
	ABrickSpectatorPawn(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual UInputComponent* CreatePlayerInputComponent() override;
	virtual void SetupPlayerInputComponent(UInputComponent* MyInputComponent) override;
	virtual void DisableInput(APlayerController* PC) override;
	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime) override;
	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void Tick(float DeltaTime) override;
	virtual void MoveForward(float Val) override;
	virtual void MoveRight(float Val) override;
	virtual FVector GetPawnViewLocation() const override;
	virtual FRotator GetViewRotation() const override;
	virtual bool CheckStillInWorld() override;
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;
	virtual void OutsideWorldBounds() override;
	// ~Super Interface

	// ~Free Cam
	// Initializes the free cam mode
	void InitializeFreeCam(ABrickPlayerController* InPC, EFreeCamMode InFreeCamMode, const TOptional<FPlayerSpawnRequest>& InSpawnRequest);
	// Get the currently used mode
	EFreeCamMode GetFreeCamMode() const
	{
		return FreeCamMode;
	}

	// ~Free Cam

	// ~Spawning
	// Whether it is currently possible to move the pawn
	bool CanPlacePawn() const;
	// Whether the spectator is being used to spawn a vehicle
	bool IsSpawningVehicle() const;
	// ~Spawning

	// ~Input
	// Set whether the player wants to place the pawn
	void SetWantsToPlacePawn(bool bNewPlace, bool bNewSpawnDummy = false);
	// Set whether the player wants to use shift speed setting currently
	void SetUseShiftSpeed(bool bNewShiftSpeed);
	// Adds accumulated input for the rotate pawn axis
	void AddRotatePawnInput(float Val);
	// ~Input

private:
	// Update the current movement speed
	void UpdateMovementSpeed();
	// Set whether the pawn is currently being placed
	void SetIsPlacingPawn(bool bNewPlacing);
	// Updates the size of the placement bounds
	void SetPlacementBoundsSizeOffset(const FVector& NewSize);

	// ~IViewTargetInterface
	virtual void GetCameraViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;
	virtual float GeCameraSpeedRatio() const override;
	virtual void OnCameraSpeedRatioChanged() override;

	virtual const FViewTargetCameraSpeedParams* GetCameraSpeedParams() const override
	{
		return &CameraSpeedParams;
	}

	virtual FViewTargetCameraSpeedCache* GetCameraSpeedCache() const override
	{
		return bUseShiftSpeed ? &ShiftCameraSpeedCache : &CameraSpeedCache;
	}

	virtual const FViewTargetZoomParams* GetZoomParams() const override
	{
		return &ZoomParams;
	}

	virtual FViewTargetZoomCache* GetZoomCache() const override
	{
		return &ZoomCache;
	}

	virtual FVector GetViewTargetAirVelocity() const override
	{
		return GetVelocity();
	}

	// ~IViewTargetInterface
};
