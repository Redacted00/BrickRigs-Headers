#pragma once

#include "BrickProperty.h"
#include "Misc/FluMathStatics.h"
#include "UI/Misc/NumericValueType.h"

struct FNumericBrickPropertyValue
{
	static constexpr auto MaxNum = 3;

	// ~Constructor
	FNumericBrickPropertyValue() = default;

	// ~Constructor
	FNumericBrickPropertyValue(const float V)
		: Data(V, 0.f, 0.f), NumUsed(1)
	{
	}

	// ~Constructor
	FNumericBrickPropertyValue(const FVector2D& V)
		: Data(V, 0.f), NumUsed(2)
	{
	}

	// ~Constructor
	FNumericBrickPropertyValue(const FVector& V)
		: Data(V), NumUsed(3)
	{
	}

	// ~Constructor
	FNumericBrickPropertyValue(const FRotator& R)
		: Data(R.Roll, R.Pitch, R.Yaw), NumUsed(3)
	{
	}

	operator float() const
	{
		return Data.X;
	}

	operator FVector2D() const
	{
		return {Data.X, Data.Y};
	}

	operator FVector() const
	{
		return Data;
	}

	operator FRotator() const
	{
		return FRotator(Data.Y, Data.Z, Data.X);
	}

	auto Num() const
	{
		return NumUsed;
	}

	auto Get(const int32 Index) const
	{
		check(Index < NumUsed);
		return Data[Index];
	}

	auto GetOr(const int32 Index, const float Fallback) const
	{
		check(Index < MaxNum);
		return Index < NumUsed ? Data[Index] : Fallback;
	}

	auto GetOrFirst(const int32 Index) const
	{
		return Data[FMath::Min(Index, NumUsed - 1)];
	}

	void Set(const int32 Index, const float Value)
	{
		check(Index < MaxNum);
		NumUsed = FMath::Max(static_cast<int32>(NumUsed), Index + 1);
		Data[Index] = Value;
	}

private:
	FVector Data = FVector::ZeroVector;
	uint8 NumUsed = 0;
};

struct FNumericBrickPropertyRange
{
	FNumericBrickPropertyValue Min;
	FNumericBrickPropertyValue Max;

	// ~Constructor
	FNumericBrickPropertyRange()
	{
	}

	// ~Constructor
	FNumericBrickPropertyRange(const FFloatInterval& Interval)
		: Min(Interval.Min), Max(Interval.Max)
	{
	}

	// ~Constructor
	FNumericBrickPropertyRange(const FNumericBrickPropertyValue& Min, const FNumericBrickPropertyValue& Max)
		: Min(Min), Max(Max)
	{
	}
};

struct FNumericBrickPropertyBase : FBrickProperty
{
	DECLARE_BRICK_PROP_BASE(FNumericBrickProperty);

protected:
	// ~Variables
	const TBrickPropAttribute<ENumericValueType> ValueType;
	const TBrickPropAttribute<FNumericBrickPropertyRange> ValueRange;
	const TBrickPropAttribute<EFluAxisLock> AxisLock;
	// ~Variables

public:
	// ~Constructor
	FNumericBrickPropertyBase(const TBrickPropAttribute<ENumericValueType>& ValueType = ENumericValueType::Percent, const TBrickPropAttribute<FNumericBrickPropertyRange>& ValueRange = FNumericBrickPropertyRange{0.f, 1.f}, const TBrickPropAttribute<EFluAxisLock>& AxisLock = EFluAxisLock::None)
		: ValueType(ValueType), ValueRange(ValueRange), AxisLock(AxisLock)
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
		return SetValue(Container, ValueFromString(NewValue.ToString()));
	}

	virtual FString ExportProperty(const FBrickPropertyContainer& Container) const override
	{
		auto Value = FNumericBrickPropertyValue();
		GetValue(Container, Value);

		auto Result = FString();
		for (auto i = 0; i < Value.Num(); ++i)
		{
			const auto Val = LexToString(Value.Get(i));
			Result = i == 0 ? Val : FString::Printf(TEXT("%s,%s"), *Result, *Val);
		}

		return Result;
	}

	virtual bool CanExportProperty(const FBrickPropertyContainer& Container) const override
	{
		return true;
	}

	virtual bool ImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return SetValue(Container, ValueFromString(Buffer));
	}

	virtual bool CanImportProperty(const FBrickPropertyContainer& Container, const TCHAR* Buffer) const override
	{
		return true;
	}

	// ~Super Interface

	virtual bool GetValue(const FBrickPropertyContainer& Container, FNumericBrickPropertyValue& OutValue) const = 0;

	virtual bool SetValue(const FBrickPropertyContainer& Container, const FNumericBrickPropertyValue& NewValue) const = 0;

	FNumericBrickPropertyValue SanitizeValue(const FBrickPropertyContainer& Container, const FNumericBrickPropertyValue& NewValue) const
	{
		const auto Range = GetValueRange(Container);
		auto OutValue = NewValue;
		const auto ActualAxisLock = GetAxisLock(Container);
		for (auto i = 0; i < OutValue.Num(); ++i)
		{
			// Remap the index according to the axis lock
			const auto ValueIndex = UFluMathStatics::RemapAxisLockIndex(ActualAxisLock, i);
			OutValue.Set(i, FMath::Clamp(OutValue.Get(ValueIndex), Range.Min.GetOrFirst(ValueIndex), Range.Max.GetOrFirst(ValueIndex)));
		}

		return OutValue;
	}

	ENumericValueType GetValueType(const FBrickPropertyContainer& Container) const
	{
		return ValueType.Get(Container, ENumericValueType::Percent);
	}

	virtual FNumericBrickPropertyRange GetValueRange(const FBrickPropertyContainer& Container) const
	{
		return ValueRange.Get(Container, FNumericBrickPropertyRange{0.f, 1.f});
	}

	EFluAxisLock GetAxisLock(const FBrickPropertyContainer& Container) const
	{
		return AxisLock.Get(Container).Get(EFluAxisLock::None);
	}

private:
	static FNumericBrickPropertyValue ValueFromString(const FString& String)
	{
		auto NewValues = FNumericBrickPropertyValue();
		auto ValueStart = 0;
		// NOTE: Iterate one over the string length, so we can process the last number
		for (auto i = 0; i <= String.Len(); ++i)
		{
			if (i == String.Len() || String[i] == ',' || FChar::IsWhitespace(String[i]))
			{
				// Process the last value
				const auto Num = i - ValueStart;
				if (Num > 0)
				{
					NewValues.Set(NewValues.Num(), FCString::Atof(*String.Mid(ValueStart, Num)));

					// Abort if we filled the data
					if (NewValues.Num() >= NewValues.MaxNum)
					{
						break;
					}
				}

				ValueStart = i + 1;
			}
		}

		return NewValues;
	}
};

template <typename FNumericType>
struct FNumericBrickProperty : FNumericBrickPropertyBase
{
	DECLARE_BRICK_PROP(FNumericBrickProperty, FNumericBrickPropertyBase);

	// ~Constructor
	FNumericBrickProperty(const TBrickPropAttribute<ENumericValueType>& ValueType = ENumericValueType::Percent, const TBrickPropAttribute<FNumericBrickPropertyRange>& ValueRange = FNumericBrickPropertyRange{0.f, 1.f}, const TBrickPropAttribute<EFluAxisLock>& AxisLock = EFluAxisLock::None)
		: Super(ValueType, ValueRange, AxisLock)
	{
	}

	// ~Super Interface
	virtual bool ComparePropertyValues(const void* A, const void* B) const override
	{
		return CompareInternal<FNumericType>(A, B);
	}

	virtual bool SetValue(const FBrickPropertyContainer& Container, const FNumericBrickPropertyValue& NewValue) const override
	{
		const auto ActualValue = SanitizeValue(Container, NewValue);
		return SetValueInternal<FNumericType>(Container, ActualValue);
	}

	virtual bool GetValue(const FBrickPropertyContainer& Container, FNumericBrickPropertyValue& OutValue) const override
	{
		FNumericType ActualValue;
		if (GetValueInternal<FNumericType>(Container, ActualValue))
		{
			OutValue = ActualValue;
			return true;
		}

		return false;
	}

	// ~Super Interface
};

DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<float>, float);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<int32>, int32);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint32>, uint32);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint16>, uint16);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<uint8>, uint8);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<FVector2D>, FVector2D);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<FVector>, FVector);
DECLARE_BRICK_PROP_TYPE(FNumericBrickProperty<FRotator>, FRotator);
