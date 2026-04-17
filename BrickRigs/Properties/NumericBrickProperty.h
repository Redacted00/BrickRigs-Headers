#pragma once

#include "BrickProperty.h"
#include "UI/Misc/NumericValueType.h"

struct FNumericBrickPropertyBase : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FNumericBrickPropertyBase);

	DECLARE_DELEGATE_RetVal_OneParam(FFloatInterval, FGetValueRange, const FBrickPropertyContainer&);

protected:
	// ~Variables
	const TAttribute<ENumericValueType> ValueType;
	const FFloatInterval ValueRange;
	const FGetValueRange ValueRangeDelegate;
	// ~Variables

public:
	// ~Constructor
	FNumericBrickPropertyBase(const FFloatInterval& InValueRange, const FGetValueRange& InValueRangeDelegate, const TAttribute<ENumericValueType>& InValueType)
		: ValueType(InValueType), ValueRange(InValueRange), ValueRangeDelegate(InValueRangeDelegate)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		checkNoEntry();
		return false;
	}

	virtual bool GetValueAsText(const FBrickPropertyContainer& Container, FText& OutValue) const override;

	virtual bool SetValueAsText(const FBrickPropertyContainer& Container, const FText& NewValue) const override
	{
		return SetValue(Container, FCString::Atof(*NewValue.ToString()));
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = 0.f;
		GetValue(Container, Value);
		return LexToString(Value);
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		auto NewValue = 0.f;
		LexFromString(NewValue, Buffer);
		return SetValue(Container, NewValue);
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, float& OutValue) const = 0;
	virtual bool SetValue(const FBrickPropertyContainer& Container, float NewValue) const = 0;

	float ClampValue(const FBrickPropertyContainer& Container, float NewValue) const
	{
		const FFloatInterval ContainerValueRange = GetValueRange(Container);
		return FMath::Clamp(NewValue, ContainerValueRange.Min, ContainerValueRange.Max);
	}

	FFloatInterval GetValueRange(const FBrickPropertyContainer& Container) const
	{
		if (ValueRangeDelegate.IsBound())
		{
			return ValueRangeDelegate.Execute(Container);
		}

		return ValueRange;
	}

	ENumericValueType GetValueType() const
	{
		return ValueType.Get();
	}
};

template <typename NumericType>
struct FNumericBrickProperty : FNumericBrickPropertyBase
{
	DECLARE_BRICK_PROP(FNumericBrickProperty, FNumericBrickPropertyBase);

	// ~Constructor
	FNumericBrickProperty(const FFloatInterval& InValueRange = FFloatInterval(0.f, 1.f), const TAttribute<ENumericValueType>& InValueType = ENumericValueType::Percent)
		: FNumericBrickPropertyBase(InValueRange, FGetValueRange(), InValueType)
	{
	}

	FNumericBrickProperty(const FGetValueRange& InValueRangeDelegate, const TAttribute<ENumericValueType>& InValueType = ENumericValueType::Percent)
		: FNumericBrickPropertyBase(FFloatInterval(), InValueRangeDelegate, InValueType)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<NumericType>(A, B);
	}

	virtual bool GetValue(const FBrickPropertyContainer& Container, float& OutValue) const override
	{
		NumericType ActualValue;
		if (GetValueInternal<NumericType>(Container, ActualValue))
		{
			OutValue = ActualValue;
			return true;
		}
		return false;
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, float NewValue) const override
	{
		const NumericType ActualValue = ClampValue(Container, NewValue);
		return SetValueInternal<NumericType>(Container, ActualValue);
	}

	// ~Super Interface
};

DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<float>, float);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<int32>, int32);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint32>, uint32);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint16>, uint16);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint8>, uint8);
