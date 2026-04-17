#pragma once

#include "CoreMinimal.h"
#include "BrickUnits.generated.h"

USTRUCT(BlueprintType)
struct FBrickUnits
{
	GENERATED_BODY()

	// ~Constructor
	FBrickUnits(int32 InUnits = 0)
		: TotalSubUnits(InUnits)
	{
	}

	int32 GetTotalSubUnits() const
	{
		return TotalSubUnits;
	}

	void SetTotalSubUnits(int32 InUnits)
	{
		TotalSubUnits = InUnits;
	}

	// Get the full units part
	int32 GetUnits() const
	{
		return FMath::DivideAndRoundDown<uint16>(TotalSubUnits, 3);
	}

	// Set the full units only
	void SetUnits(int32 InUnits)
	{
		const int32 SubUnits = GetSubUnits();
		TotalSubUnits = InUnits * 3 + SubUnits;
	}

	// Get the sub units only
	int32 GetSubUnits() const
	{
		return TotalSubUnits % 3;
	}

	// Set the sub units only
	void SetSubUnits(int32 InSubUnits)
	{
		const int32 Units = GetUnits();
		TotalSubUnits = Units * 3 + InSubUnits;
	}

	float GetFractionalUnits() const
	{
		return TotalSubUnits / 3.f;
	}

	float GetUnrealUnits() const
	{
		return TotalSubUnits * 10.f;
	}

	FText ToText(bool bZeroAsUndetermined) const
	{
		if (bZeroAsUndetermined && IsZero())
		{
			return FText::AsCultureInvariant("?");
		}
		const int32 Units = GetUnits();
		const int32 SubUnits = GetSubUnits();

		FText OutText;
		if (Units > 0 || SubUnits == 0)
		{
			OutText = FText::AsNumber(Units);
		}
		if (SubUnits > 0)
		{
			// Show sub units as 1/3 and 2/3, which is more intuitive than the 1s, 2s notation
			const FText SubUnitsText = FText::AsCultureInvariant(SubUnits == 1 ? UTF8_TO_TCHAR("\u2153") : UTF8_TO_TCHAR("\u2154"));
			OutText = Units > 0 ? FText::Format(FText::AsCultureInvariant("{0}{1}"), OutText, SubUnitsText) : SubUnitsText;
		}
		return OutText;
	}

	FString ToString() const
	{
		return LexToString(TotalSubUnits);
	}

#define IMPLEMENT_OPERATOR(op) \
	bool operator##op(const FBrickUnits& Other) const {\
		return TotalSubUnits ##op Other.TotalSubUnits; }

	IMPLEMENT_OPERATOR(==);
	IMPLEMENT_OPERATOR(!=);
	IMPLEMENT_OPERATOR(<);
	IMPLEMENT_OPERATOR(<=);
	IMPLEMENT_OPERATOR(>);
	IMPLEMENT_OPERATOR(>=);

#undef IMPLEMENT_OPERATOR

	bool IsZero() const
	{
		return TotalSubUnits == 0;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << TotalSubUnits;
		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickUnits& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr += LexToString(TotalSubUnits);
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, class UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		LexFromString(TotalSubUnits, Buffer);
		return true;
	}

private:
	// ~Variables
	uint16 TotalSubUnits;
	// ~Variables
};

template <>
struct TStructOpsTypeTraits<FBrickUnits> : TStructOpsTypeTraitsBase2<FBrickUnits>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};

USTRUCT(BlueprintType)
struct FBrickSize
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits Z;

	// ~Constructor
	FBrickSize(const FBrickUnits& InUniform = 0)
		: X(InUniform), Y(InUniform), Z(InUniform)
	{
	}

	// ~Constructor
	FBrickSize(const FBrickUnits& InX, const FBrickUnits& InY, const FBrickUnits& InZ)
		: X(InX), Y(InY), Z(InZ)
	{
	}

	// ~Constructor
	FBrickSize(const FVector& InVector)
		: X(InVector.X), Y(InVector.Y), Z(InVector.Z)
	{
	}

	FVector ToVector() const
	{
		return FVector(X.GetFractionalUnits(), Y.GetFractionalUnits(), Z.GetFractionalUnits());
	}

	FVector ToUnrealVector() const
	{
		return FVector(X.GetUnrealUnits(), Y.GetUnrealUnits(), Z.GetUnrealUnits());
	}

	FText ToText(bool bZeroAsUndetermined) const
	{
		return FText::Format(FText::AsCultureInvariant("{0}x{1}x{2}"), X.ToText(bZeroAsUndetermined), Y.ToText(bZeroAsUndetermined), Z.ToText(bZeroAsUndetermined));
	}

	bool IsZero() const
	{
		return X.IsZero() && Y.IsZero() && Z.IsZero();
	}

	const FBrickUnits& operator[](int32 Index) const
	{
		check(Index >= 0 && Index < 3);
		return (&X)[Index];
	}

	FBrickUnits& operator[](int32 Index)
	{
		check(Index >= 0 && Index < 3);
		return (&X)[Index];
	}

	bool operator==(const FBrickSize& Other) const
	{
		return X == Other.X && Y == Other.Y && Z == Other.Z;
	}

	bool operator!=(const FBrickSize& Other) const
	{
		return !(*this == Other);
	}

	// Whether ANY component is >=
	bool operator>=(const FBrickSize& Other) const
	{
		return X >= Other.X || Y >= Other.Y || Z >= Other.Z;
	}

	bool Serialize(FArchive& Ar)
	{
		X.Serialize(Ar);
		Y.Serialize(Ar);
		Z.Serialize(Ar);
		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickSize& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		X.ExportTextItem(ValueStr, FBrickUnits(), Parent, PortFlags, ExportRootScope);
		ValueStr += ",";
		Y.ExportTextItem(ValueStr, FBrickUnits(), Parent, PortFlags, ExportRootScope);
		ValueStr += ",";
		Z.ExportTextItem(ValueStr, FBrickUnits(), Parent, PortFlags, ExportRootScope);

		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, class UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto Elements = TArray<FString>();
		FString(Buffer).ParseIntoArray(Elements, TEXT(","));

		for (auto i = 0; i < 3; ++i)
		{
			auto ElementBuffer = Elements.IsValidIndex(i) ? *Elements[i] : TEXT("");
			(*this)[i].ImportTextItem(ElementBuffer, PortFlags, OwnerObject, ErrorText);
		}

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FBrickSize> : TStructOpsTypeTraitsBase2<FBrickSize>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};
