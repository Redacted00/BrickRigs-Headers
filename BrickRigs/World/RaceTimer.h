// Copyright Fluppisoft, 2019

#pragma once

#include "DragStrip.h"
#include "GameFramework/Actor.h"
#include "RaceTimer.generated.h"

class UStaticMeshComponent;
class UTextRenderComponent;

UCLASS()
class BRICKRIGS_API ARaceTimer : public AActor
{
	GENERATED_BODY()

public:
	// ~Components
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* TextRenderComponent;
	// ~Components

	// ~Constructor
	ARaceTimer();

	// Show the race results on the display
	void ShowResults(const FDragRaceLane& Results, const TArray<float>& FinishLines);
};
