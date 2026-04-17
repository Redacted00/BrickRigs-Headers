#pragma once

#include "GameFramework/OnlineReplStructs.h"
#include "BrickProperty.h"

struct FUserIdBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FUserIdBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FUniqueNetIdRepl>(A, B);
	}

	// ~Super Interface

	bool GetValue(const FBrickPropertyContainer& Container, FUniqueNetIdRepl& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	bool SetValue(const FBrickPropertyContainer& Container, FUniqueNetIdRepl NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}
};

DECLARE_BRICK_PROP_TYPE(FUserIdBrickProperty, FUniqueNetIdRepl);
