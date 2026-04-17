#pragma once

#include "CoreMinimal.h"
#include "BrickEditorSaveVersion.generated.h"

USTRUCT()
struct FBrickRigsSaveVersion
{
	GENERATED_BODY()

private:
	uint8 Version;

public:
	// ~Constructor
	FBrickRigsSaveVersion(uint8 InVersion = 0)
		: Version(InVersion)
	{
	}

	FString ToString() const
	{
		return LexToString(Version);
	}

	void FromString(const FString& InString)
	{
		LexFromString(Version, *InString);
	}

	bool operator==(const FBrickRigsSaveVersion& Other) const
	{
		return Version == Other.Version;
	}

	bool operator!=(const FBrickRigsSaveVersion& Other) const
	{
		return Version != Other.Version;
	}

	bool operator>(const FBrickRigsSaveVersion& Other) const
	{
		return Version > Other.Version;
	}

	bool operator>=(const FBrickRigsSaveVersion& Other) const
	{
		return Version >= Other.Version;
	}

	bool operator<(const FBrickRigsSaveVersion& Other) const
	{
		return Version < Other.Version;
	}

	bool operator<=(const FBrickRigsSaveVersion& Other) const
	{
		return Version <= Other.Version;
	}

	friend FArchive& operator<<(FArchive& Ar, FBrickRigsSaveVersion& InVersion)
	{
		Ar << InVersion.Version;
		return Ar;
	}

	bool ExportTextItem(FString& ValueStr, const FBrickRigsSaveVersion& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
	{
		ValueStr += FString::FromInt(Version);
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

		FromString(ImportedString);
		Buffer = NewBuffer;

		return true;
	}
};

template <>
struct TStructOpsTypeTraits<FBrickRigsSaveVersion> : TStructOpsTypeTraitsBase2<FBrickRigsSaveVersion>
{
	enum
	{
		WithExportTextItem = true,
		WithImportTextItem = true
	};
};

// Current save version, the legacy UBrickStatics version ended at 6
const static FBrickRigsSaveVersion BR_SAVE_VERSION = 14;
// Version where brick units started to be saved as a float instead of uint16
const static FBrickRigsSaveVersion BR_SAVE_BRICK_UNITS_FLOAT_VERSION = 14;
// Version where wheel meshes were moved so their connectors would line up with the origin
const static FBrickRigsSaveVersion BR_SAVE_ALIGNED_WHEEL_ORIGIN_VERSION = 14;
// Prior to this version GetNameStringByIndex was used instead of GetNameStringByIndex in FEnumBrickPropertyBase::SerializeProperty
const static FBrickRigsSaveVersion BR_SAVE_FIXED_ENUM_SERIALIZATION_VERSION = 13;
// Version where input channel indices where replaced by brick references
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_REFERENCE_VERSION = 13;
// Version where item mass and price started to be saved to the meta data
const static FBrickRigsSaveVersion BR_SAVE_MASS_AND_PRICE = 12;
// Version where min and max sensor input values were replaced by universal input channel parameters
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_CURVE_VERSION = 11;
// Version where the min and max actuation limit properties have been inverted
const static FBrickRigsSaveVersion BR_SAVE_ACTUATOR_LIMITS_VERSION = 10;
// Version where suspension damping was made player controllable
const static FBrickRigsSaveVersion BR_SAVE_SUSPENSION_DAMPING_VERSION = 9;
// Version where the tags were added to the meta data
const static FBrickRigsSaveVersion BR_SAVE_TAGS_VERSION = 8;
// Version where we transition from the UBrickStatics save functions to the UBrickEditorSaveInterface
const static FBrickRigsSaveVersion BR_SAVE_INTERFACE_VERSION = 7;
// Version before the save interface was implemented
const static FBrickRigsSaveVersion BR_SAVE_LAST_LEGACY_VERSION = 6;
// Version where input channels were introduced and the input axes enum has been changed
const static FBrickRigsSaveVersion BR_SAVE_INPUT_CHANNEL_VERSION = 6;
// Legacy version where the brick location accuracy was increased
const static FBrickRigsSaveVersion BR_SAVE_SMALLER_STEPS_VERSION = 4;
// Version where the saved element size has been fixed
const static FBrickRigsSaveVersion BR_SAVE_FIXED_ELEMENT_SIZE_VERSION = 3;
