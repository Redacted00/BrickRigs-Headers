#pragma once

#include "CoreMinimal.h"
#include "Misc/BrickRigsCustomVersion.h"
#include "BrickUnits.generated.h"

USTRUCT(BlueprintType)
struct FBrickUnits
{
	GENERATED_BODY()

	static constexpr auto NumSubUnitsPerUnit = 3;
	static constexpr auto NumUnitsPerSubUnit = 1.f / 3.f;
	static constexpr auto NumFractionalUnitsPerSubUnit = 100;
	static constexpr auto SubUnitsToUnrealUnits = 10.f;

	// ~Constructors
	FBrickUnits()
	{
	}

private:
	explicit FBrickUnits(const float InSubUnits)
		: SubUnits(InSubUnits)
	{
	}

	// ~Constructors

public:
	static FBrickUnits FromSubUnits(const float InUnits)
	{
		return FBrickUnits(InUnits);
	}

	static FBrickUnits FromUnits(const float InUnits)
	{
		return FBrickUnits(InUnits * NumSubUnitsPerUnit);
	}

	static FBrickUnits FromUnrealUnits(const float InUnits)
	{
		return FBrickUnits(InUnits / SubUnitsToUnrealUnits);
	}

	// Reads units from the legacy 1s notation
	static FBrickUnits FromLegacyString(FString Str)
	{
		const auto bSubUnits = Str.RemoveFromEnd("s");
		const auto Value = FCString::Atoi(*Str);
		return bSubUnits ? FromSubUnits(Value) : FromUnits(Value);
	}

	static FBrickUnits Zero()
	{
		return FBrickUnits();
	}

	float GetSubUnits() const
	{
		return SubUnits;
	}

	void SetSubUnits(const float InUnits)
	{
		*this = FromSubUnits(InUnits);
	}

	// Returns total sub units as a fraction of full units, i.e. 3s would return 1
	float GetUnits() const
	{
		return SubUnits * NumUnitsPerSubUnit;
	}

	// Set units as a full units
	void SetUnits(const float InUnits)
	{
		*this = FromUnits(InUnits);
	}

	// Returns units converted to distance in cm
	float GetUnrealUnits() const
	{
		return SubUnits * SubUnitsToUnrealUnits;
	}

	// Set units as distance in cm
	void SetUnrealUnits(const float InUnits)
	{
		*this = FromUnrealUnits(InUnits);
	}

	FText ToText(const bool bZeroAsUndetermined) const
	{
		if (bZeroAsUndetermined && IsZero())
		{
			return FText::AsCultureInvariant("?");
		}

		const auto FractionalSubUnits = FMath::Fmod(SubUnits, NumSubUnitsPerUnit);

		auto OutText = FText::GetEmpty();

		// Helper lambda to generate rounded 1/3 and 2/3 display texts
		const auto TestSpecialCase = [&](const float Value, const TCHAR* Suffix = nullptr)
		{
			if (FMath::IsNearlyEqual(FractionalSubUnits, Value))
			{
				OutText = FText::AsCultureInvariant(Suffix);

				const auto FullUnits = FMath::FloorToInt(GetUnits());
				if (FullUnits > 0)
				{
					OutText = FText::Format(FText::AsCultureInvariant("{0}{1}"), FText::AsNumber(FullUnits), OutText);
				}

				return true;
			}

			return false;
		};

		// Test special cases first
		if (!TestSpecialCase(1.f, UTF8_TO_TCHAR("\u2153")) && !TestSpecialCase(2.f, UTF8_TO_TCHAR("\u2154")))
		{
			auto FormattingOptions = FNumberFormattingOptions();
			FormattingOptions.UseGrouping = false;
			FormattingOptions.MaximumFractionalDigits = 4.f;
			OutText = FText::AsNumber(GetUnits(), &FormattingOptions);
		}

		return OutText;
	}

	FString ToString() const
	{
		return LexToString(SubUnits);
	}

	void FromString(const FString& Str)
	{
		LexFromString(SubUnits, *Str);
	}

#define IMPLEMENT_OPERATOR(op) \
	bool operator##op(const FBrickUnits& Other) const {\
		return SubUnits ##op Other.SubUnits; }

	IMPLEMENT_OPERATOR(==);
	IMPLEMENT_OPERATOR(!=);
	IMPLEMENT_OPERATOR(<);
	IMPLEMENT_OPERATOR(<=);
	IMPLEMENT_OPERATOR(>);
	IMPLEMENT_OPERATOR(>=);

#undef IMPLEMENT_OPERATOR

	bool IsNearlyEqual(const FBrickUnits& Other, const float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return FMath::IsNearlyEqual(SubUnits, Other.SubUnits, Tolerance);
	}

	bool IsZero() const
	{
		return FMath::IsNearlyZero(SubUnits);
	}

	bool Serialize(FArchive& Ar)
	{
		Ar.UsingCustomVersion(FBrickRigsCustomVersion::GUID);
		return SerializeInternal(Ar, Ar.IsLoading() && Ar.CustomVer(FBrickRigsCustomVersion::GUID) < FBrickRigsCustomVersion::BrickUnitsAsFloats);
	}

	bool SerializeInternal(FArchive& Ar, const bool bOutdated)
	{
		if (bOutdated)
		{
			// Sub units used to be stored and serialized as uint16
			uint16 IntSubUnits = 0;
			Ar << IntSubUnits;
			SubUnits = IntSubUnits;
		}
		else
		{
			Ar << SubUnits;
		}

		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickUnits& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr += ToString();
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto ImportedString = FString();
		const auto* NewBuffer = FPropertyHelpers::ReadToken(Buffer, ImportedString, true);
		if (!NewBuffer)
		{
			return false;
		}

		FromString(ImportedString);
		Buffer = NewBuffer;

		return true;
	}

private:
	// ~Variables
	float SubUnits = 0.f;
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

	// NOTE: Use x instead of a comma, since the latter can cause issues when parsing text
	static constexpr auto ComponentDelimiterChar = 'x';
	static constexpr auto ComponentDelimiter = TEXT("x");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits Y;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBrickUnits Z;

	// ~Constructors
	explicit FBrickSize(const FBrickUnits& InUniform = FBrickUnits())
		: X(InUniform), Y(InUniform), Z(InUniform)
	{
	}

	FBrickSize(const FBrickUnits& InX, const FBrickUnits& InY, const FBrickUnits& InZ)
		: X(InX), Y(InY), Z(InZ)
	{
	}

	// ~Constructors

	static FBrickSize FromUnrealVector(const FVector& InVector)
	{
		return {FBrickUnits::FromUnrealUnits(InVector.X), FBrickUnits::FromUnrealUnits(InVector.Y), FBrickUnits::FromUnrealUnits(InVector.Z)};
	}

	// Reads a brick size vector from the legacy 1x1x1s notation
	static FBrickSize FromLegacyString(const FString& Str)
	{
		auto Result = FBrickSize();
		auto CompIdx = 0;
		for (auto i = 0; i < Str.Len() && CompIdx < 3; ++i)
		{
			auto End = i;

			// Find the next delimiter
			while (End < Str.Len() && Str[End] != ComponentDelimiterChar && (Str[End] == 's' || FChar::IsDigit(Str[End])))
			{
				++End;
			}

			// Invalid token, abort
			if (End == i)
			{
				break;
			}

			Result[CompIdx] = FBrickUnits::FromLegacyString(Str.Mid(i, End - i));

			i = End;
			++CompIdx;
		}

		return Result;
	}

	FVector ToVector() const
	{
		return FVector(X.GetUnits(), Y.GetUnits(), Z.GetUnits());
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

	bool IsNearlyEqual(const FBrickSize& Other, const float Tolerance = KINDA_SMALL_NUMBER) const
	{
		return X.IsNearlyEqual(Other.X, Tolerance) && Y.IsNearlyEqual(Other.Y, Tolerance) && Z.IsNearlyEqual(Other.Z, Tolerance);
	}

	// Returns the smallest component
	FBrickUnits GetMin() const
	{
		return FMath::Min3(X, Y, Z);
	}

	// Returns the largest component
	FBrickUnits GetMax() const
	{
		return FMath::Max3(X, Y, Z);
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
		ValueStr += ComponentDelimiter;
		Y.ExportTextItem(ValueStr, FBrickUnits(), Parent, PortFlags, ExportRootScope);
		ValueStr += ComponentDelimiter;
		Z.ExportTextItem(ValueStr, FBrickUnits(), Parent, PortFlags, ExportRootScope);

		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto ImportedString = FString();
		const auto* NewBuffer = FPropertyHelpers::ReadToken(Buffer, ImportedString, true);
		if (!NewBuffer)
		{
			return false;
		}

		// Split into components
		auto Elements = TArray<FString>();
		ImportedString.ParseIntoArray(Elements, ComponentDelimiter);

		for (auto i = 0; i < 3; ++i)
		{
			(*this)[i].FromString(Elements.IsValidIndex(i) ? Elements[i] : FString());
		}

		Buffer = NewBuffer;

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
