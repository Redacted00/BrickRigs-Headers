#pragma once

#include "Vehicle/BrickConnection.h"
#include "CoreMinimal.h"
#include "BrickStatics.h"
#include "ScalableBrickConnectorSpacing.generated.h"

// This struct is used to store the compressed per axis spacing enum for scalable bricks
USTRUCT(BlueprintType)
struct FScalableBrickConnectorSpacing
{
	GENERATED_BODY()

	static constexpr auto NumBitsPerAxis = 2;
	static constexpr auto NumBitsPerAxisPair = NumBitsPerAxis * 2;
	static constexpr auto AxisBitmask = UBrickStatics::GenerateIntWithLeadingOnes(NumBitsPerAxis);

	// ~Constructor
	FScalableBrickConnectorSpacing()
		: Data(0)
	{
	}

	void SetSpacing(EAxis::Type Axis, bool bPositiveAxis, EConnectorSpacing NewSpacing)
	{
		const auto AxisOffset = GetAxisOffset(Axis, bPositiveAxis);
		Data &= ~(AxisBitmask << AxisOffset);
		Data |= (static_cast<int32>(NewSpacing) & AxisBitmask) << AxisOffset;
	}

	EConnectorSpacing GetSpacing(EAxis::Type Axis, bool bPositiveAxis) const
	{
		return static_cast<EConnectorSpacing>(Data >> GetAxisOffset(Axis, bPositiveAxis) & AxisBitmask);
	}

	EConnectorSpacing GetVectorSpacing(const FVector& Axis) const
	{
		if (!FMath::IsNearlyZero(Axis.X))
		{
			return GetSpacing(EAxis::X, Axis.X > 0.f);
		}
		if (!FMath::IsNearlyZero(Axis.Y))
		{
			return GetSpacing(EAxis::Y, Axis.Y > 0.f);
		}
		if (!FMath::IsNearlyZero(Axis.Z))
		{
			return GetSpacing(EAxis::Z, Axis.Z > 0.f);
		}
		
		ensure(false);
		return EConnectorSpacing::None;
	}

	bool operator==(const FScalableBrickConnectorSpacing& Other) const
	{
		return Data == Other.Data;
	}

	bool Serialize(FArchive& Ar)
	{
		Ar << Data;
		return true;
	}

	bool ExportTextItem(FString& ValueStr, const FScalableBrickConnectorSpacing& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr += LexToString(Data);
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		auto ImportedString = FString();
		const auto* NewBuffer = FPropertyHelpers::ReadToken(Buffer, ImportedString, false);
		if (!NewBuffer)
		{
			return false;
		}

		LexFromString(Data, *ImportedString);
		Buffer = NewBuffer;

		return true;
	}

private:
	static int32 GetAxisOffset(EAxis::Type Axis, bool bPositiveAxis)
	{
		return (Axis - 1) * NumBitsPerAxisPair + (bPositiveAxis ? NumBitsPerAxis : 0);
	}

	// ~Variables
	uint16 Data;
	// ~Variables
};

template <>
struct TStructOpsTypeTraits<FScalableBrickConnectorSpacing> : TStructOpsTypeTraitsBase2<FScalableBrickConnectorSpacing>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};
