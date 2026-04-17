// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorObject.h"
#include "CoreMinimal.h"
#include "BrickEditorObjectReference.generated.h"

class UBrickEditorObject;

// A struct that holds the ID and a pointer to another brick
template <typename T>
struct TBrickEditorObjectPtr
{
	TBrickEditorObjectPtr()
		: Ptr(nullptr)
	{
	}

	// Initialize the pointer from an object
	TBrickEditorObjectPtr(const T* InObject)
	{
		*this = InObject;
	}

	// Assign an object to this pointer
	void operator=(const T* InObject)
	{
		Ptr = const_cast<T*>(InObject);
		ID = InObject;
	}

	bool operator==(const TBrickEditorObjectPtr& Other) const
	{
		return Ptr == Other.Ptr && ID == Other.ID;
	}

	bool operator==(const T* InObject) const
	{
		return Ptr == InObject;
	}

	bool operator!=(const T* InObject) const
	{
		return Ptr != InObject;
	}

	T* operator->() const
	{
		check(IsValid());
		return Get();
	}

	// Return whether this reference is pointing to a null object
	bool IsNull() const
	{
		return ID.IsNull();
	}

	// Return whether Get would return a valid pointer
	bool IsValid() const
	{
		return Ptr.IsValid();
	}

	// Get the cached pointer to the object
	T* Get() const
	{
		return Ptr.Get();
	}

	const FBrickEditorObjectID& GetID() const
	{
		return ID;
	}

	// Clears the reference
	void Reset()
	{
		Ptr.Reset();
		ID.Reset();
	}

	bool Serialize(FArchive& Ar)
	{
		if (Ar.IsSaving())
		{
			// Update the ID since it is changed before the saving process
			if (Ptr.IsValid())
			{
				ID = Ptr->GetEditorObjectID();
			}
			else
			{
				ID.Reset();
			}
		}

		Ar << ID;
		return true;
	}

	friend FArchive& operator<<(FArchive& Ar, TBrickEditorObjectPtr& Ref)
	{
		Ref.Serialize(Ar);
		return Ar;
	}

private:
	// ~Variables
	// The pointer to the object
	TWeakObjectPtr<T> Ptr;
	// The cached id of the referenced object
	FBrickEditorObjectID ID;
	// ~Variables
};

// UStruct wrapper
USTRUCT()
struct FBrickEditorObjectPtr
{
	GENERATED_BODY()

	// ~Constructor
	FBrickEditorObjectPtr(const UBrickEditorObject* InObject = nullptr)
		: Ptr(InObject)
	{
	}

	bool operator==(const FBrickEditorObjectPtr& Other) const
	{
		return Ptr == Other.Ptr;
	}

	bool IsNull() const
	{
		return Ptr.IsNull();
	}

	bool IsValid() const
	{
		return Ptr.IsValid();
	}

	void Reset()
	{
		Ptr.Reset();
	}

	UBrickEditorObject* Get() const
	{
		return Ptr.Get();
	}

	template <class T>
	T* Get() const
	{
		return Cast<T>(Ptr.Get());
	}

	UBrickEditorObject* operator->() const
	{
		check(IsValid());
		return Get();
	}

	TBrickEditorObjectPtr<UBrickEditorObject>& GetPtr()
	{
		return Ptr;
	}

	const FBrickEditorObjectID& GetID() const
	{
		return Ptr.GetID();
	}

	bool Serialize(FArchive& Ar)
	{
		return Ptr.Serialize(Ar);
	}

	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bSuccess)
	{
		return Ptr.Serialize(Ar);
	}

private:
	// Actual pointer to the object
	TBrickEditorObjectPtr<UBrickEditorObject> Ptr;
};

template <>
struct TStructOpsTypeTraits<FBrickEditorObjectPtr> : TStructOpsTypeTraitsBase2<FBrickEditorObjectPtr>
{
	enum
	{
		WithIdenticalViaEquality = true,
		WithSerializer = true,
		WithNetSerializer = true
		// NOTE: This cannot have WithNetSharedSerialization since an object is being replicated
	};
};
