#pragma once

#include "Misc/FluMathStatics.h"
#include "Misc/ConstExpressions.h"
#include "Vehicle/BrickConnection.h"
#include "CoreMinimal.h"
#include "ScalableBrickConnectorSpacing.generated.h"

// Flags indicate if the axes are visible/editable, the order is +X, -X, +Y, -Y, +Z, -Z
using FConnectorSpacingAxisFlags = uint8;

// Stores the compressed per axis spacing enum for scalable bricks
USTRUCT(BlueprintType)
struct FScalableBrickConnectorSpacing
{
	GENERATED_BODY()

	static constexpr auto NumBitsPerAxis = 2;
	static constexpr auto NumBitsPerAxisPair = NumBitsPerAxis * 2;
	static constexpr auto AxisBitmask = FConstExpressions::GenerateIntWithLeadingOnes(NumBitsPerAxis);

	// ~Constructor
	FScalableBrickConnectorSpacing()
		: Data(0)
	{
	}

	void SetSpacing(const EFluAxisSigned Axis, const EConnectorSpacing NewSpacing)
	{
		const auto AxisOffset = GetAxisOffset(Axis);
		Data &= ~(AxisBitmask << AxisOffset);
		Data |= (static_cast<int32>(NewSpacing) & AxisBitmask) << AxisOffset;
	}

	void SetAll(const EConnectorSpacing NewSpacing)
	{
		Data = 0;
		const auto Value = static_cast<int32>(NewSpacing) & AxisBitmask;
		for (auto i = 0; i < 6; ++i)
		{
			Data |= Value << NumBitsPerAxis * i;
		}
	}

	EConnectorSpacing GetSpacing(const EFluAxisSigned Axis) const
	{
		return static_cast<EConnectorSpacing>(Data >> GetAxisOffset(Axis) & AxisBitmask);
	}

	EConnectorSpacing GetVectorSpacing(const FVector& Axis) const
	{
		if (!FMath::IsNearlyZero(Axis.X))
		{
			return GetSpacing(UFluMathStatics::MakeAxisSigned(EAxis::X, Axis.X > 0.f));
		}
		if (!FMath::IsNearlyZero(Axis.Y))
		{
			return GetSpacing(UFluMathStatics::MakeAxisSigned(EAxis::Y, Axis.Y > 0.f));
		}
		if (!FMath::IsNearlyZero(Axis.Z))
		{
			return GetSpacing(UFluMathStatics::MakeAxisSigned(EAxis::Z, Axis.Z > 0.f));
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
	static int32 GetAxisOffset(const EFluAxisSigned Axis)
	{
		const auto [AxisUnsigned, bPositiveAxis] = UFluMathStatics::SplitAxisSigned(Axis);
		const auto AxisIdx = UFluMathStatics::AxisToIndex(AxisUnsigned);
		return AxisIdx * NumBitsPerAxisPair + (bPositiveAxis ? NumBitsPerAxis : 0);
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
