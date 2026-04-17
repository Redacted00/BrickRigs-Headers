// Copyright Fluppisoft, 2017

#pragma once

#include "TimerManager.h"
#include "DeathmatchGameMode.h"
#include "ConquestGameMode.generated.h"

class AConquestFlag;
class ABrickCharacter;

/**
 * 
 */
UCLASS()
class BRICKRIGS_API AConquestGameMode : public ADeathmatchGameMode
{
	GENERATED_BODY()

private:
	// ~Variables
	// Used for awarding capture point scores every few seconds
	FTimerHandle TimerHandle_UpdateCapturePoints;
	// ~Variables

protected:
	// ~Properties
	// The delay between giving each team the capture point score
	UPROPERTY(EditAnywhere, Category = Conquest)
	float UpdateCapturePointsInterval;
	// Score added for each captured point
	UPROPERTY(EditAnywhere, Category = TeamScore)
	int32 HoldCapturePointTeamScore;
	// ~Properties

public:
	// ~Constructor
	AConquestGameMode();

	// ~Super Interface
	virtual void OnMatchStateSet() override;
	// ~Super Interface

private:
	// This function is called by a timer to add scores to all capturer teams
	void UpdateCapturePoints();
};
