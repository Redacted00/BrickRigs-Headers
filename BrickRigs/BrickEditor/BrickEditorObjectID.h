#pragma once

#include "BrickEditorObjectID.generated.h"

class UBrickEditorObject;

// A unique ID used to identify objects through loading and saving
// NOTE: This struct should not be used alone to store references to objects in the editor (since IDs might change sometimes), use TBrickEditorObjectPtr instead
USTRUCT(BlueprintType)
struct FBrickEditorObjectID
{
	GENERATED_BODY()

	FBrickEditorObjectID()
		: ID(0)
	{
	}

	FBrickEditorObjectID(int32 Index)
		: ID(Index + 1)
	{
	}

	FBrickEditorObjectID(const UBrickEditorObject* InObject)
	{
		*this = InObject;
	}

	void operator=(const UBrickEditorObject* InObject);

	// Reset the ID to null
	void Reset()
	{
		ID = 0;
	}

	int32 GetObjectIndex() const
	{
		return ID - 1;
	}

	// Whether this ID *potentially* points to a valid brick
	bool IsValid() const
	{
		return ID != 0;
	}

	bool IsNull() const
	{
		return ID == 0;
	}

	FString ToString() const
	{
		return LexToString(ID);
	}

	void SetFromString(const FString& Str)
	{
		LexFromString(ID, *Str);
	}

	bool operator==(const FBrickEditorObjectID& Other) const
	{
		return ID == Other.ID;
	}

	bool operator!=(const FBrickEditorObjectID& Other) const
	{
		return ID != Other.ID;
	}

	// Used for sorting
	bool operator<(const FBrickEditorObjectID& Other) const
	{
		return ID < Other.ID;
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		Ar << *this;
		return true;
	}

	friend FArchive& operator<<(FArchive& Ar, FBrickEditorObjectID& ObjectID)
	{
		return Ar << ObjectID.ID;
	}

private:
	// ~Variables
	// The unique ID, 0 means invalid
	uint16 ID;
	// ~Variables
};

template <>
struct TStructOpsTypeTraits<FBrickEditorObjectID> : TStructOpsTypeTraitsBase2<FBrickEditorObjectID>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithNetSerializer = true
	};
};
