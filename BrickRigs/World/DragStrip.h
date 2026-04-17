// Copyright Fluppisoft, 2016

#pragma once

#include "Interaction/InteractionComponent.h"
#include "GameFramework/Actor.h"
#include "DragStrip.generated.h"

// Actors
class ARaceTimer;
class ABrickVehicle;
class ABrickCharacter;
class UBoxComponent;
class UMaterialInstanceDynamic;

// This is a single drag distance and lane time
USTRUCT()
struct FDragRaceTime
{
	GENERATED_BODY()
	;

public:
	float Time;
	float Speed;

	bool operator==(const FDragRaceTime& Other) const
	{
		return Time == Other.Time && Speed == Other.Speed;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess);
};

template <>
struct TStructOpsTypeTraits<FDragRaceTime> : public TStructOpsTypeTraitsBase2<FDragRaceTime>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true
	};
};

// This struct contains partly replicated state info about a single lane
USTRUCT()
struct FDragRaceLane
{
	GENERATED_BODY()
	;

public:
	TWeakObjectPtr<ABrickCharacter> Character;
	TArray<FDragRaceTime> Times;

	// Helper function to reset the lane, returns true if something was changed
	FORCEINLINE bool Reset()
	{
		if (Character != nullptr || Times.Num())
		{
			Character = nullptr;
			Times.Empty();
			return true;
		}

		return false;
	}

	bool operator==(const FDragRaceLane& Other) const
	{
		//UE_LOG(LogTemp, Warning, TEXT("FDragRaceLane::operator=="));
		return Character == Other.Character && Times == Other.Times;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess);
};

template <>
struct TStructOpsTypeTraits<FDragRaceLane> : public TStructOpsTypeTraitsBase2<FDragRaceLane>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true,
		WithIdenticalViaEquality = true,
	};
};

// Enum that represents the current state of a race
UENUM()
enum class EDragRaceState : uint8
{
	None,
	Countdown,
	InProgress,
	Cancelled
};

UCLASS(Abstract)
class BRICKRIGS_API ADragStrip : public AActor
{
	GENERATED_BODY()

public:
	// ~Variables
	FTimerHandle TimerHandle_TimeOut;
	// World time when the race has been started
	float StartTime;

	// The replicate state of the race
	UPROPERTY(ReplicatedUsing = OnRep_RaceState)
	EDragRaceState RaceState;
	UFUNCTION()
	void OnRep_RaceState();
	// The replicated race results for each lane
	UPROPERTY(ReplicatedUsing = OnRep_LeftLaneState)
	FDragRaceLane LeftLaneState;
	UFUNCTION()
	void OnRep_LeftLaneState();
	UPROPERTY(ReplicatedUsing = OnRep_RightLaneState)
	FDragRaceLane RightLaneState;
	UFUNCTION()
	void OnRep_RightLaneState();

	// Material instance used for the tree lights
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* TreeMID;
	// Cached number of lights currently displayed by the MID
	uint8 NumLightsActive;
	// Whether blinking is currently active
	uint8 bBlinkingEnabled : 1;

#if WITH_EDITORONLY_DATA
	UPROPERTY(Transient)
	UBoxComponent* BoundsBox;
#endif
	// ~Variables

	// ~Components
	UPROPERTY(Transient)
	UInteractionComponent* InteractionComponent;
	// ~Components

public:
	// ~Properties
	// The relative location of the start line
	UPROPERTY(EditDefaultsOnly, Category = DragStrip)
	float StartLine;
	// The different offsets of the finish lines, relative to the start line
	UPROPERTY(EditDefaultsOnly, Category = DragStrip)
	TArray<float> FinishLines;
	// Width and height of the area that will register vehicles
	UPROPERTY(EditDefaultsOnly, Category = DragStrip)
	FVector2D TrackBounds;
	UPROPERTY(EditDefaultsOnly, Category = DragStrip)
	int32 NumTreeLights;
	UPROPERTY(EditDefaultsOnly, Category = DragStrip)
	float PerLightDelay;

	// References to the race timers in the world
	UPROPERTY(EditAnywhere, Category = DragStrip)
	ARaceTimer* RaceTimerRight;
	UPROPERTY(EditAnywhere, Category = DragStrip)
	ARaceTimer* RaceTimerLeft;
	// ~Properties

	// ~Constructor
	ADragStrip();

	// ~Super Interface
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void PostInitializeComponents() override;
	virtual void Tick(float DeltaTime) override;
	// ~Super Interface

	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	FVector GetInteractionLocation() const;
	UFUNCTION(BlueprintImplementableEvent)
	UMaterialInstanceDynamic* CreateTreeMID();
	// ~Blueprint Interface

	// Start the race countdown, server only
	void StartRace();

private:
	// Change the current state, server only
	void SetRaceState(EDragRaceState NewState);
	// Get the current state of the race
	EDragRaceState GetRaceState() const;
	// Called whenever the current race state has changed
	void OnRaceStateChanged();
	// Called whenever the lane states have changed
	void OnLeftLaneStateChanged();
	void OnRightLaneStateChanged();
	// Helper function to update a race timer
	void UpdateRaceTimer(ARaceTimer* Timer, const FDragRaceLane& Lane);
	// Helper function for setting the start time to the current time
	void UpdateStartTime();
	// Helper function for getting the time since race or countdown start
	float GetTimeSinceStart() const;
	// Helper function for updating the MID
	void UpdateTreeMID(int32 NumLights, bool bShouldBlink);
	// Helper function for getting the offset of any finish line
	float GetFinishLine(int32 Index) const;

	// Helper function to check if a given character is over a certain finish or start line
	bool CheckCharacterOverLine(ABrickCharacter* Char, float Line, bool bSkipBoundsCheck = false) const;
	// Called from the tick function to update the progress of an individual character, returns true if the race is finished on this lane
	template <typename Func>
	bool UpdateLaneRaceProgress(FDragRaceLane& Lane, Func OnChangedFunc);

	// ~Interaction
	// Callbacks for the interaction component
	void GetInteractionOptions(ABrickPlayerController* PC, FInteractionOptions& OutOptions);
	UFUNCTION()
	void Interact_StartRace(ABrickPlayerController* PC);
	// ~Interaction
};
