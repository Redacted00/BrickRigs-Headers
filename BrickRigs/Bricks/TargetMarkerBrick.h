// Copyright Fluppisoft, 2016

#pragma once

#include "Components/HUDIconComponent.h"
#include "Bricks/CameraBrick.h"
#include "TargetMarkerBrick.generated.h"

class UGunBrick;

UCLASS(Abstract)
class BRICKRIGS_API UTargetMarkerBrickStaticInfo : public UCameraBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = TargetMarker)
	FHUDIconProperties ImpactPointIconProperties;
	// Number of iterations to go through per gun
	UPROPERTY(EditDefaultsOnly, Category = TargetMarker)
	int32 NumTrajectoryIterationsPerGun;
	// Determines how traces are distributed along the trajectory, higher values mean more precision closer to the gun
	UPROPERTY(EditDefaultsOnly, Category = TargetMarker)
	float TrajectoryDistributionExponent;
	// Maximum number of guns to update per frame
	UPROPERTY(EditDefaultsOnly, Category = TargetMarker)
	int32 MaxGunIterationsPerFrame;
	// ~Properties

	// ~Constructor
	UTargetMarkerBrickStaticInfo();
};

// This specialized HUD icon component is used for the target marker only
UCLASS(ClassGroup = (BrickRigs), meta = (BlueprintSpawnableComponent))
class BRICKRIGS_API UTargetMarkerIconComponent : public UHUDIconComponent
{
	GENERATED_BODY()

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnHasValidTargetChanged, bool);

	// ~Variables
	// Whether the marker is pointing at a valid target
	bool bHasValidTarget;
	// ~Variables

public:
	// ~Properties
	// The gun associated with this icon
	TWeakObjectPtr<UGunBrick> GunBrick;
	// ~Properties

	// ~Delegates
	FOnHasValidTargetChanged OnHasValidTargetChangedDelegate;
	// ~Delegates

	bool HasValidTarget() const
	{
		return bHasValidTarget;
	}

	void SetHasValidTarget(bool bNewHasValidTarget)
	{
		if (bNewHasValidTarget != bHasValidTarget)
		{
			bHasValidTarget = bNewHasValidTarget;
			OnHasValidTargetChangedDelegate.Broadcast(bNewHasValidTarget);
		}
	}
};

struct FTargetMarkerIconComponentParams : public FBrickEditorHUDIconComponentParams
{
	// ~Variables
	TWeakObjectPtr<UGunBrick> GunBrick;
	// ~Variables

	FTargetMarkerIconComponentParams(const UTargetMarkerIconComponent* Default = GetDefault<UTargetMarkerIconComponent>())
		: FBrickEditorHUDIconComponentParams(Default),
		  GunBrick(Default->GunBrick)
	{
	}

	void InitializeComponent(UTargetMarkerIconComponent* Comp)
	{
		FBrickEditorHUDIconComponentParams::InitializeComponent(Comp);

		Comp->GunBrick = GunBrick;
		Comp->OnHasValidTargetChangedDelegate.Clear();
	}
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UTargetMarkerBrick : public UCameraBrick
{
	GENERATED_BODY()

private:
	// ~Variables
	// List of all currently controlled guns, together with their impact point icons
	TArray<TTuple<TWeakObjectPtr<UGunBrick>, TBrickEditorComponentPtr<UTargetMarkerIconComponent>>> ControlledGuns;
	// The last gun that has been updated
	int32 LastUpdatedGunIndex;
	// Set to true if the controlled guns need to be updated
	uint32 bControlledGunsDirty : 1;
	// ~Variables

public:
	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void OnBricksAddedOrRemovedFromCluster() override;
	virtual void GetCameraBrickView(float DeltaTime, FMinimalViewInfo& DesiredView) override;
	virtual void BecomeViewTargetSubobject(APlayerController* PC) override;
	virtual void EndViewTargetSubobject(APlayerController* PC) override;
	virtual FRotator GetFirstTimeViewRotation() const override;
	// ~Super Interface

private:
	// Marks the list of controlled guns as needing to be updated
	void MarkGunsDirty();
	// Updates the list of controlled guns
	void UpdateGuns();
	// Removes the icon for a specific gun
	void RemoveImpactPointIcon(int32 GunIndex);
};
