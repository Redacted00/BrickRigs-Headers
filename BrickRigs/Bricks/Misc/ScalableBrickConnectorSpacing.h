#pragma once

#include "Vehicle/BrickConnection.h"
#include "CoreMinimal.h"
#include "ScalableBrickConnectorSpacing.generated.h"

// This struct is used to store the compressed per axis spacing enum for scalable bricks
USTRUCT(BlueprintType)
struct FScalableBrickConnectorSpacing
{
	GENERATED_BODY()
	;

public:
	// ~Constructor
	FScalableBrickConnectorSpacing()
		: Data(0)
	{
	}

	void SetSpacing(EAxis::Type Axis, bool bPositiveAxis, EConnectorSpacing NewSpacing)
	{
		const int32 AxisOffset = GetAxisOffset(Axis, bPositiveAxis);
		Data &= ~(3 << AxisOffset);
		Data |= static_cast<int32>(NewSpacing) << AxisOffset;
	}

	EConnectorSpacing GetSpacing(EAxis::Type Axis, bool bPositiveAxis) const
	{
		return static_cast<EConnectorSpacing>((Data >> GetAxisOffset(Axis, bPositiveAxis)) & 3);
	}

	EConnectorSpacing GetVectorSpacing(const FVector& Axis) const
	{
		if (Axis.X > 0.f)
		{
			return GetSpacing(EAxis::X, true);
		}
		if (Axis.X < 0.f)
		{
			return GetSpacing(EAxis::X, false);
		}
		if (Axis.Y > 0.f)
		{
			return GetSpacing(EAxis::Y, true);
		}
		if (Axis.Y < 0.f)
		{
			return GetSpacing(EAxis::Y, false);
		}
		if (Axis.Z > 0.f)
		{
			return GetSpacing(EAxis::Z, true);
		}
		if (Axis.Z < 0.f)
		{
			return GetSpacing(EAxis::Z, false);
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
		ValueStr = LexToString(Data);
		return true;
	}

	bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, class UObject* OwnerObject, FOutputDevice* ErrorText)
	{
		LexFromString(Data, Buffer);
		return true;
	}

private:
	int32 GetAxisOffset(EAxis::Type Axis, bool bPositiveAxis) const
	{
		return (Axis - 1) * 4 + (bPositiveAxis ? 2 : 0);
	}

	// ~Variables
	uint16 Data;
	// ~Variables
};

template <>
struct TStructOpsTypeTraits<FScalableBrickConnectorSpacing> : public TStructOpsTypeTraitsBase2<FScalableBrickConnectorSpacing>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};
