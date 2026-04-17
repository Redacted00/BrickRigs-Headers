// Copyright Fluppisoft, 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrickVehicleDownloadReplicator.generated.h"

class ABrickVehicle;
class ABrickPlayerController;

UCLASS()
class BRICKRIGS_API ABrickVehicleDownloadReplicator : public AActor
{
	GENERATED_BODY()

	// ~Variables
	// Vehicle being spawned
	UPROPERTY(Transient)
	ABrickVehicle* Vehicle;
	UPROPERTY(Transient, Replicated)
	ABrickPlayerController* SpawningPC;
	UPROPERTY(Transient, Replicated)
	FString VehicleDisplayName;
	UPROPERTY(Transient, ReplicatedUsing = OnRep_DownloadProgress)
	uint8 DownloadProgress;
	UFUNCTION()
	void OnRep_DownloadProgress();
	// ~Variables

public:
	// ~Constructor
	ABrickVehicleDownloadReplicator();

	// ~Super Interface
	virtual bool IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const override;
	virtual void PostNetInit() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(const float DeltaTime) override;
	// ~Super Interface

	void InitDownloadReplicator(ABrickVehicle* InVehicle, ABrickPlayerController* InSpawningPC, const FString& InDisplayName);

	// Get the replicated download percentage
	float GetDownloadProgress() const
	{
		return DownloadProgress / 255.f;
	}

	// Set the current download progress
	void SetDownloadProgress(const float Value);

	// Get the replicated vehicle name
	auto GetVehicleDisplayName() const
	{
		return FText::AsCultureInvariant(VehicleDisplayName);
	}

private:
	void OnDownloadProgressChanged();

	// Notifies the player about the download progress changing
	void NotifySpawningPlayer(const bool bIsDownloading);
};
