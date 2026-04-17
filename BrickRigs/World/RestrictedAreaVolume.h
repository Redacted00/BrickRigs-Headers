// Copyright Fluppisoft, 2017

#pragma once

#include "GameFramework/Volume.h"
#include "RestrictedAreaVolume.generated.h"

/**
 *
 */
UCLASS()
class BRICKRIGS_API ARestrictedAreaVolume : public AVolume
{
	GENERATED_BODY()

protected:
	// ~Properties
	// Whether the effect of the volume should be inverted, i.e. the outside is a restricted zone
	UPROPERTY(EditAnywhere, Category = RestrictedArea)
	bool bInvertVolume;
	// Time until the character is killed after entering
	UPROPERTY(EditAnywhere, Category = RestrictedArea)
	float KillDelay;
	// ~Properties

public:
	// ~Constructor
	ARestrictedAreaVolume();

	// ~Super Interface
	virtual void BeginPlay() override;
	// ~Super Interface

	// Public accessor to bInvertVolume
	auto GetInvertVolume() const
	{
		return bInvertVolume;
	}

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	// Helper function to handle begin and end overlap events
	void HandleOverlap(AActor* OverlappedActor, bool bEntered);
};
