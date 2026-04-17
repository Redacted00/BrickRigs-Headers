// Copyright Fluppisoft, 2020

#pragma once

#include "Misc/BrickAssetManager.h"
#include "Misc/AlphabeticId.h"
#include "Serialization/SerializationHelper.h"
#include "CoreMinimal.h"
#include "SpawnArea.h"
#include "CapturePoint.generated.h"

class ABaseCharacter;
class ABrickBuilding;
class UMaterialInstanceDynamic;
class UStaticMeshComponent;

// Shape of the "hot zone"
UENUM(BlueprintType)
enum class ECapturePointShape : uint8
{
	Box,
	Sphere,
	Capsule
};

USTRUCT()
struct FCapturePointState
{
	GENERATED_BODY()

	// Current capture percentage
	float CaptureRatio;
	// Whether the flag has been captured by CapturedBy
	bool bHasBeenCaptured;
	// The team that is currently raising or lowering the flag
	FGenericTeamId CapturingTeam;
	// The team this flag is captured by, or the team that started capturing
	FGenericTeamId CapturedBy;

	bool operator==(const FCapturePointState& Other) const
	{
		return CaptureRatio == Other.CaptureRatio && bHasBeenCaptured == Other.bHasBeenCaptured && CapturingTeam == Other.CapturingTeam && CapturedBy == Other.CapturedBy;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		// Compress the capture ratio to a byte
		// IMPORTANT: Round down so the value is really only 1 when the point is fully captured
		FSerializationHelper::SerializeFloatCompressed(Ar, CaptureRatio, 0.f, 1.f, 8, false);
		// Serialize the remaining variables
		Ar << bHasBeenCaptured;
		FSerializationHelper::SerializeTeamId(Ar, CapturedBy);
		FSerializationHelper::SerializeTeamId(Ar, CapturingTeam);

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FCapturePointState> : TStructOpsTypeTraitsBase2<FCapturePointState>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

/**
 * A capture point is used in game modes like conquest and rush as an objective
 */
UCLASS(Abstract)
class BRICKRIGS_API ACapturePoint : public ASpawnArea
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCaptureRatioChanged, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnCapturingTeamChanged, const FGenericTeamId&);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCapturedByChanged, const FGenericTeamId&, bool);
	DECLARE_MULTICAST_DELEGATE(FOnCharacterEnteredOrLeft);

	// ~Variables
	FSmartStreamableHandle StreamableHandle_BadgeTexture;

	// The replicated capture point state information
	UPROPERTY(ReplicatedUsing = OnRep_CapturePointState)
	FCapturePointState CapturePointState;
	UFUNCTION()
	void OnRep_CapturePointState(const FCapturePointState& OldState);

	// Cached list of characters in the capturing zone
	UPROPERTY(Transient)
	TArray<ABaseCharacter*> OverlappingCharacters;

	// Flag material instance
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* FlagMID;
	// Cached material parameter indices
	int32 FlagHeightParamIndex;

	// The building this capture point is standing on top of, used to prevent it from collapsing
	UPROPERTY(EditAnywhere, Category = CapturePoint)
	TSoftObjectPtr<ABrickBuilding> BuildingBase;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UStaticMeshComponent* MeshComponent;
	// Used for player overlap queries
	UPROPERTY(Transient)
	UShapeComponent* ShapeComponent;
	// ~Components

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = CapturePoint)
	UStaticMesh* StaticMesh;
	UPROPERTY(EditDefaultsOnly, Category = CapturePoint)
	float MaxDrawDistance;
	UPROPERTY(EditDefaultsOnly, Category = CapturePoint)
	float MaxFlagHeight;
	// Relative location of the HUD icon
	UPROPERTY(EditDefaultsOnly, Category = CapturePoint)
	FVector IconOffset;
	// Time in seconds it takes to fully capture this flag
	UPROPERTY(EditDefaultsOnly, Category = CapturePoint)
	float TimeToCapture;

	UPROPERTY(EditAnywhere, Category = CapturePoint)
	EAlphabeticId CapturePointId;
	UPROPERTY(EditAnywhere, Category = CapturePoint)
	ECapturePointShape CapturePointShape;
	UPROPERTY(EditAnywhere, Category = CapturePoint)
	FVector ShapeOffset;
	UPROPERTY(EditAnywhere, Category = CapturePoint)
	FRotator ShapeRotation;
	UPROPERTY(EditAnywhere, Category = CapturePoint)
	FVector ShapeScale;
	// ~Properties

public:
	// ~Delegates
	FOnCaptureRatioChanged OnCaptureRatioChangedDelegate;
	FOnCapturingTeamChanged OnCapturingTeamChangedDelegate;
	FOnCapturedByChanged OnCapturedByChangedDelegate;
	FOnCharacterEnteredOrLeft OnCharacterEnteredOrLeftDelegate;
	// ~Delegates

	// ~Constructor
	ACapturePoint();

	// ~Super Interface
#if WITH_EDITOR
	virtual void PostEditMove(bool bFinished) override;
#endif
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;
	virtual void Tick(float DeltaTime) override;
	virtual FVector GetIconLocation() const override;
	// ~Super Interface

	// Get the short name of the capture point, aka the flag letter
	UFUNCTION(BlueprintPure)
	FText GetCapturePointShortDisplayName() const
	{
		return FAlphabeticId::ToText(CapturePointId);
	}

	// Returns whether the given character is on this capture point
	UFUNCTION(BlueprintPure)
	bool IsCharacterOnCapturePoint(ABaseCharacter* InCharacter) const
	{
		return OverlappingCharacters.Contains(InCharacter);
	}

	// Whether the capture point is standing on top of the given building
	UFUNCTION(BlueprintPure)
	bool IsCapturePointOnBuilding(const ABrickBuilding* InBuilding) const;

	// Get the current capture percentage
	UFUNCTION(BlueprintPure)
	float GetCaptureRatio() const
	{
		return CapturePointState.CaptureRatio;
	}

	// Set the current capture ratio
	UFUNCTION(BlueprintCallable)
	bool SetCaptureRatio(float NewRatio);

	// Get the currently capturing team
	UFUNCTION(BlueprintPure)
	const FGenericTeamId& GetCapturingTeam() const
	{
		return CapturePointState.CapturingTeam;
	}

	// Set the team that is currently capturing this point
	UFUNCTION(BlueprintCallable)
	bool SetCapturingTeam(const FGenericTeamId& TeamId);

	// Whether the point has been fully captured
	UFUNCTION(BlueprintPure)
	bool HasBeenCaptured() const
	{
		return CapturePointState.bHasBeenCaptured;
	}

	// Return the team that has captured this point
	UFUNCTION(BlueprintPure)
	const FGenericTeamId& GetCapturedBy() const
	{
		return CapturePointState.CapturedBy;
	}

	// Set the team that has captured this point
	UFUNCTION(BlueprintCallable)
	bool SetCapturedBy(const FGenericTeamId& TeamId, bool bNewHasBeenCaptured);

private:
	// Callbacks for changed variables
	void OnCaptureRatioChanged();
	void OnCapturingTeamChanged();
	void OnCapturedByChanged();

	// Helper function to create the shape component, reuses the current one if the type matches
	template <class T>
	T* CreateShapeComponent();
	void CreateBoxComponent();
	void CreateSphereComponent();
	void CreateCapsuleComponent();
	// Callbacks for the shape
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Handles a begin or end overlap event
	void HandleTriggerOverlap(AActor* OverlappedActor, bool bEntered);

	// Return whether this capture point should be ticking
	bool ShouldBeTicking() const;
	// Enables or disables ticking as required
	void UpdateCapturePointTickEnabled();
	// Creates the flag MID if needed, returns true if it can be used
	bool InitializeFlagMID();
	// Updates the flag height
	void UpdateFlagHeight();
	// Updates the flag color
	void UpdateFlagColor();

public:
	// ~ISpawnPointInterface
	virtual int32 GetSpawnSortPriority() const override;
	virtual bool CanPlayerSpawn(ABrickPlayerController* PC) const override;
	// ~ISpawnPointInterface
};
