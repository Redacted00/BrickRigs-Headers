#pragma once

#include "GameFramework/OnlineReplStructs.h"
#include "BrickProperty.h"
#include "Online.h"

struct FUserIdBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FUserIdBrickProperty);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FUniqueNetIdRepl>(A, B);
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override
	{
		auto Value = FUniqueNetIdRepl();
		if (GetValue(Container, Value))
		{
			OutValue = FText::AsCultureInvariant(Value.ToString());
			return true;
		}

		return false;
	}

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		auto NewId = FUniqueNetIdRepl();

		if (const auto IdentityInterface = Online::GetIdentityInterface())
		{
			NewId = IdentityInterface->CreateUniquePlayerId(NewValue.ToString());
		}

		return SetValue(Container, NewId);
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
