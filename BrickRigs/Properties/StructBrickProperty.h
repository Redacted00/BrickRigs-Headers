#pragma once

#include "BrickProperty.h"

struct FStructBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FStructBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	// ~Super Interface
};
