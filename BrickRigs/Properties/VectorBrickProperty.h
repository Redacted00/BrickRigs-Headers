#pragma once

#include "CoreMinimal.h"
#include "BrickProperty.h"
#include "UI/Misc/NumericValueType.h"

struct FVectorBrickPropertyBase : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FVectorBrickPropertyBase);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = FVector::ZeroVector;
		GetValue(Container, Value);
		return Value.ToString();
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		auto NewValue = FVector::ZeroVector;
		NewValue.InitFromString(Buffer);
		return SetValue(Container, NewValue);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, FVector& OutValue) const
	{
		return GetValueInternal(Container, OutValue);
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, FVector NewValue) const
	{
		return SetValueInternal(Container, NewValue);
	}

	virtual void GetValueRange(const FBrickPropertyContainer& Container, FVector& OutMin, FVector& OutMax) const = 0;

	virtual ENumericValueType GetValueType(const FBrickPropertyContainer& Container) const
	{
		return ENumericValueType::Float;
	}
};

struct FVectorBrickProperty : FVectorBrickPropertyBase
{
	DECLARE_BRICK_PROP(FVectorBrickProperty, FVectorBrickPropertyBase);

	DECLARE_DELEGATE_ThreeParams(FGetValueRange, const FBrickPropertyContainer&, FVector&, FVector&);

private:
	// ~Variables
	FGetValueRange GetValueRangeDelegate;
	// ~Variables

public:
	// ~Constructor
	FVectorBrickProperty(const FGetValueRange& InValueRangeDelegate)
		: GetValueRangeDelegate(InValueRangeDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FVector>(A, B);
	}

	// ~Super Interface

	virtual void GetValueRange(const FBrickPropertyContainer& Container, FVector& OutMin, FVector& OutMax) const override
	{
		GetValueRangeDelegate.ExecuteIfBound(Container, OutMin, OutMax);
	}

	virtual ENumericValueType GetValueType(const FBrickPropertyContainer& Container) const override
	{
		return ENumericValueType::DistanceAuto;
	}
};

DECLARE_BRICK_PROP_TYPE(FVectorBrickProperty, FVector);

struct FRotatorBrickProperty : FVectorBrickPropertyBase
{
	DECLARE_BRICK_PROP(FRotatorBrickProperty, FVectorBrickPropertyBase);

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FRotator>(A, B);
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, FVector& OutValue) const override
	{
		FRotator OutRotator;
		if (GetValueInternal(Container, OutRotator))
		{
			OutValue = FVector(OutRotator.Roll, OutRotator.Pitch, OutRotator.Yaw);
			return true;
		}

		return false;
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, FVector NewValue) const override
	{
		// NOTE: Don't normalize, would swap -180 and 180 for example
		const auto NewRotator = FRotator(NewValue.Y, NewValue.Z, NewValue.X);
		return SetValueInternal(Container, NewRotator);
	}

	virtual void GetValueRange(const FBrickPropertyContainer& Container, FVector& OutMin, FVector& OutMax) const override
	{
		OutMin = FVector(-180.f);
		OutMax = FVector(180.f);
	}

	virtual ENumericValueType GetValueType(const FBrickPropertyContainer& Container) const override
	{
		return ENumericValueType::Angle;
	}
};

DECLARE_BRICK_PROP_TYPE(FRotatorBrickProperty, FRotator);
