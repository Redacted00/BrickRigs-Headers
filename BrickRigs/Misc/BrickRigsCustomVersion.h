#pragma once

#include "CoreMinimal.h"
#include "Misc/Guid.h"

struct FBrickRigsCustomVersion
{
	enum Type
	{
		BeforeCustomVersionWasAdded = 0,
		BrickUnitsAsFloats,
		// Add new versions above this line
		VersionPlusOne,
		LatestVersion = VersionPlusOne - 1
	};

	static const FGuid GUID;
};
