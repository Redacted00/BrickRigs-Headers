#pragma once

#include "CoreMinimal.h"
#include "AlphabeticId.generated.h"

UENUM(BlueprintType)
enum class EAlphabeticId : uint8
{
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	Max
};

namespace FAlphabeticId
{
	FORCEINLINE FText ToText(EAlphabeticId InId)
	{
		static UEnum* Enum = FindObjectChecked<UEnum>(ANY_PACKAGE, TEXT("EAlphabeticId"), true);
		return FText::AsCultureInvariant(Enum->GetNameStringByValue(static_cast<int32>(InId)));
	}

	FORCEINLINE EAlphabeticId IndexToId(int32 InIndex)
	{
		return FMath::Min(static_cast<EAlphabeticId>(InIndex), EAlphabeticId::Max);
	}
}
