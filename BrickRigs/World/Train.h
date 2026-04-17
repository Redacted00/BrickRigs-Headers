// Copyright Fluppisoft, 2021

#pragma once

#include "Engine/StreamableManager.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.generated.h"

class UStaticMeshComponent;
class UAudioComponent;

// Contains curve data for the track a train should follow
USTRUCT(BlueprintType)
struct FTrainTrack
{
	GENERATED_BODY()

	// Automatically generated spline info
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveVector PositionCurve;
	// Contains the roll value for each position point
	UPROPERTY(BlueprintReadWrite)
	TArray<float> RollCurve;
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveFloat TimeCurve;
#if WITH_EDITORONLY_DATA
	// Total length of the track
	UPROPERTY(VisibleAnywhere)
	float Length = 0.f;
#endif

	float GetLength() const
	{
		return TimeCurve.Points.Num() ? TimeCurve.Points.Last().InVal : 0.f;
	}

	void GetLocationAndDirectionAtTime(FVector& OutLocation, FVector& OutForward, FVector& OutRight, float Param) const
	{
		if (!PositionCurve.Points.Num() || PositionCurve.Points.Num() != RollCurve.Num())
		{
			return;
		}

		// Fetch the location
		OutLocation = PositionCurve.Eval(Param);

		// NOTE: The code to calculate the rotation is copied from SplineMeshComponent.cpp, to make sure the train follows exactly the spline mesh
		OutForward = PositionCurve.EvalDerivative(Param, FVector::ZeroVector);
		const auto BaseXVec = (FVector::UpVector ^ OutForward).GetSafeNormal();
		const auto BaseYVec = (OutForward ^ BaseXVec).GetSafeNormal();

		// Apply roll to frame around spline
		const auto MinPoint = FMath::Clamp(FMath::FloorToInt(Param), 0, RollCurve.Num() - 1);
		const auto MaxPoint = FMath::Clamp(FMath::CeilToInt(Param), 0, RollCurve.Num() - 1);
		const auto UseRoll = FMath::Lerp(RollCurve[MinPoint], RollCurve[MaxPoint], FMath::Frac(Param));
		const auto CosAng = FMath::Cos(UseRoll);
		const auto SinAng = FMath::Sin(UseRoll);
		OutRight = (CosAng * BaseXVec) - (SinAng * BaseYVec);
	}

	void GetLocationAndRotationAtDistance(FVector& OutLocation, FQuat& OutRotation, float Dist) const
	{
		auto ForwardVector = FVector::ForwardVector, RightVector = FVector::RightVector;
		GetLocationAndDirectionAtTime(OutLocation, ForwardVector, RightVector, TimeCurve.Eval(Dist));
		OutRotation = FRotationMatrix::MakeFromXY(ForwardVector, RightVector).ToQuat();
	}
};

// Used to define a wagon/loko mesh to add to the train
USTRUCT(BlueprintType)
struct FTrainSegment
{
	GENERATED_BODY()

	// The mesh to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* MaterialOverride = nullptr;
	// Offset of the front and back bumper
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FrontCouplingOffset = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackCouplingOffset = 0.f;
};

UCLASS(Abstract)
class BRICKRIGS_API ATrain : public AActor
{
	GENERATED_BODY()

private:
	// ~Variables
	// Index of the world settings train track
	int32 TrainTrackIndex = INDEX_NONE;
	// Saved handle to the last horn sweep task
	FTraceHandle TraceHandle_HornSweep;
	// Used for async loading of the horn sound
	TSharedPtr<FStreamableHandle> StreamableHandle_HornSound;
	// Next world time we should sweep for an obstacle (not zero because we probably don't need to do this right after level start)
	float NextHornSweepTime = 1.f;
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(Transient)
	TArray<UStaticMeshComponent*> WagonMeshComponents;
	UPROPERTY(Transient)
	UAudioComponent* AudioComponent;
	UPROPERTY(Transient)
	UAudioComponent* HornAudioComponent;
	// ~Components

public:
	// ~Properties
	UPROPERTY(EditAnywhere, Category = Train)
	FTrainSegment LocoSegment;
	UPROPERTY(EditAnywhere, Category = Train)
	TArray<FTrainSegment> WagonSegments;
	// The speed with which the train moves
	UPROPERTY(EditAnywhere, Category = Train)
	float Speed;
	UPROPERTY(EditAnywhere, Category = Train)
	USoundBase* TrainSound;

	// Sound to be played when something is in front of the train
	UPROPERTY(EditAnywhere, Category = Horn)
	TSoftObjectPtr<USoundBase> HornSound;
	// The cached horn attenuation radius
	UPROPERTY(VisibleAnywhere)
	float HornSoundAttenuationRadius;
	// The delay between individual horn sweeps
	UPROPERTY(EditAnywhere, Category = Horn)
	float HornSweepInterval;
	// Maximum distance for horn obstacle sweeps
	UPROPERTY(EditAnywhere, Category = Horn)
	float HornSweepDist;
	// Minimum delay between sounding the horn again
	UPROPERTY(EditAnywhere, Category = Horn)
	float MinHornDelay;
	// ~Properties

	// ~Constructor
	ATrain();

	// ~Super Interface
	virtual void PreRegisterAllComponents() override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual FVector GetVelocity() const override;
#if WITH_EDITOR
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
#endif
	// ~Super Interface

	// Sets the generated train track to drive on
	UFUNCTION(BlueprintCallable)
	void SetTrainTrack(int32 Index);

private:
	// Callback for the async horn sweep
	void OnHornSweep(const FTraceHandle& InHandle, FTraceDatum& InDatum);
	// Used for async loading of the horn sound
	void OnHornSoundLoaded();
	// Plays the horn sound
	void PlayHornSound(USoundBase* Sound);

	// Builds the offline cached data, i.e. the horn sound attenuation radius
#if WITH_EDITOR
	void BuildCachedData();
#endif
};
