#pragma once

#include "VehicleDashboardType.generated.h"

UENUM(BlueprintType)
enum class EVehicleDashboardType : uint8
{
	None,
	Car,
	RaceCar,
	Aircraft,
	Advanced,
	Max,
};
