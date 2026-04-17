#pragma once

#include "CoreMinimal.h"
#include "NumericValueType.generated.h"

UENUM(BlueprintType)
enum class ENumericValueType : uint8
{
	Float,
	Integer,
	Percent,
	Angle,
	RPM,
	Time,
	Seconds,
	Minutes,
	Hours,
	Days,
	Money,
	Gear,
	SpeedAuto,
	SpeedMetric,
	SpeedImperial,
	DistanceAuto,
	DistanceMetric,
	DistanceImperial,
	DistanceAutoOrBrickUnits,
	DistanceBrickUnits,
	DistanceZeroAsUnlimited,
	VolumeAuto,
	VolumeMetric,
	VolumeImperial,
	IntZeroAsUnlimited,
	Bricks,
	BricksZeroAsUnlimited,
	MassAuto,
	MassMetric,
	MassImperial,
	MassZeroAsUnlimited,
	ForceAuto,
	ForceMetric,
	ForceImperial,
};
