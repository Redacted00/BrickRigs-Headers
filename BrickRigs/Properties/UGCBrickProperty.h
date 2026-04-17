#pragma once

#include "BrickProperty.h"

struct FUGCFileInfo;

struct FUGCBrickProperty : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FUGCBrickProperty);

private:
	// ~Variables
	const int32 MaxNum;
	// ~Variables

public:
	// ~Constructor
	FUGCBrickProperty(int32 InMaxNum = 0)
		: MaxNum(InMaxNum)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, TArray<FUGCFileInfo>& OutFiles) const;

	virtual bool SetValue(const FBrickPropertyContainer& Container, const TArray<FUGCFileInfo>& NewArray) const;

	bool IsUGCArray() const
	{
		check(Property);
		return Property->IsA<FArrayProperty>();
	}

	int GetMaxNum() const
	{
		return MaxNum;
	}
};

DECLARE_BRICK_PROP_TYPE(FUGCBrickProperty, FUGCFileInfo);
DECLARE_BRICK_PROP_TYPE(FUGCBrickProperty, TArray<FUGCFileInfo>);
