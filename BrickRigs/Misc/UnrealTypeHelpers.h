#pragma once

#include "CoreMinimal.h"

namespace FArrayHelpers
{
	template <typename T>
	FORCEINLINE void GetMutuallyExclusiveElements(const TArray<T>& A, const TArray<T>& B, TArray<T>& Out)
	{
		Out.Reserve(A.Num() + B.Num());
		for (const auto& Elem : A)
		{
			if (!B.Contains(Elem))
			{
				Out.Add(Elem);
			}
		}
		for (const auto& Elem : B)
		{
			if (!A.Contains(Elem))
			{
				Out.Add(Elem);
			}
		}
		Out.Shrink();
	}
}

namespace FEnumHelpers
{
	template <typename T>
	FORCEINLINE FString EnumToString(T Value)
	{
		const auto* Enum = StaticEnum<T>();
		return Enum->GetNameStringByValue(static_cast<int64>(Value));
	}

	template <typename T>
	FORCEINLINE T StringToEnum(const FString& Str)
	{
		const auto* Enum = StaticEnum<T>();
		return static_cast<T>(Enum->GetValueByNameString(Str));
	}

	template <typename T>
	FORCEINLINE void StringToEnum(const FString& Str, T& OutValue)
	{
		const auto* Enum = StaticEnum<T>();
		OutValue = static_cast<T>(Enum->GetValueByNameString(Str));
	}

	template <typename T>
	FORCEINLINE T CycleEnumValue(const T Value, const int32 Step = 1)
	{
		return static_cast<T>((static_cast<int32>(Value) + Step) % static_cast<int32>(T::Max));
	}
}
