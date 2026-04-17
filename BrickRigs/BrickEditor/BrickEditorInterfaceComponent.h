// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorObject.h"
#include "BrickEditorObjectReference.h"
#include "UGC/UGCTypes.h"
#include "BrickEditor/BrickEditorObjectPool.h"
#include "BrickEditorSaveVersion.h"
#include "CoreMinimal.h"
#include "FluUGCAsyncTask.h"
#include "Components/SceneComponent.h"
#include "Misc/BrickDataSingleton.h"
#include "BrickEditorInterfaceComponent.generated.h"

class ABrickEditor;

// This struct is used to serialize shared header information, like the number of objects, version etc.
struct FBrickEditorSaveHeader
{
	FBrickRigsSaveVersion Version;
	uint16 NumObjects;
	uint16 NumClasses;
	uint16 NumProperties;

	FBrickEditorSaveHeader()
		: Version(0), NumObjects(0), NumClasses(0), NumProperties(0)
	{
	}

	friend FArchive& operator<<(FArchive& Ar, FBrickEditorSaveHeader& Header)
	{
		Ar << Header.Version;
		Ar << Header.NumObjects;

		if (Header.Version >= BR_SAVE_INTERFACE_VERSION)
		{
			Ar << Header.NumClasses;
			Ar << Header.NumProperties;
		}

		return Ar;
	}
};

enum class EBrickEditorObjectCreationContext : uint8
{
	Load,
	Import,
	Duplicate,
	DuplicateMirrored,
	Mirror,
	Place
};

// The reference resolver is used to efficiently manage addresses during loading
struct FBrickEditorReferenceResolver
{
	FBrickEditorReferenceResolver(uint16 NumObjects)
	{
		Objects.SetNumZeroed(NumObjects);
	}

	int32 GetNumObjects() const
	{
		return Objects.Num();
	}

	void SetObject(uint16 ObjectIndex, UObject* Object)
	{
		check(Objects.IsValidIndex(ObjectIndex));
		Objects[ObjectIndex] = Object;
	}

	UObject* GetObject(uint16 ObjectIndex) const
	{
		return Objects.IsValidIndex(ObjectIndex) ? Objects[ObjectIndex] : nullptr;
	}

	// Map the saved ID to the new ID and resolve the object, to be used right after loading
	template <class T>
	T* Resolve(TBrickEditorObjectPtr<T>& Ptr) const
	{
		if (Ptr.IsNull())
		{
			Ptr.Reset();
			return nullptr;
		}
		const int32 Index = Ptr.GetID().GetObjectIndex();
		T* Object = Cast<T>(GetObject(Index));
		// Update the object and ID, since it might have changed
		Ptr = Object;
		return Object;
	}

	UBrickEditorObject* Resolve(FBrickEditorObjectPtr& Ptr) const
	{
		return Resolve(Ptr.GetPtr());
	}

private:
	// The ordered array of objects at their associated indices
	TArray<UObject*> Objects;
};

enum class EBrickEditorLoadResult
{
	Success,
	GenericFailure,
	Corrupt,
	ValidationFailure,
	IncompatibleVersion
};

/**
 * A subclass of this component has to be implemented for every editor type, it is used as the interface for saving, loading, spawning objects etc. between the editor and the world
 */
UCLASS()
class BRICKRIGS_API UBrickEditorInterfaceComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// We need a hard limit for the amount of objects, since the their ID's are saved as uint16 which only goes up to 65k
	static constexpr auto MaxNumBrickEditorObjects = 50000;
	// Maximum number of properties that a BEO can have, this is just to sanity check save files and can be increased if needed
	static constexpr auto MaxPropertiesPerBEO = 32;

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnUGCDownloadComplete, bool);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUGCItemLoaded, EBrickEditorLoadResult, TArray<uint8>*);
	DECLARE_DELEGATE_RetVal_OneParam(bool, FOnValidateUGCHeader, const FBrickEditorSaveHeader&);

	// ~Variables
	// All currently used editor objects
	UPROPERTY(Transient)
	TArray<UBrickEditorObject*> BrickEditorObjects;
	// Pool used for creating and reusing sub objects
	TUniquePtr<FBrickEditorObjectPool> ObjectPool;
	// Download task handle
	FFluUGCAsyncTaskHandle UGCTask_Download;
	// Current download progress
	float DownloadProgress;
	// Whether the item that is currently being downloaded should be imported
	uint8 bImportDownloadingItem : 1;
	// Indicates if this component is part of the editor, spawned etc.
	EBrickEditorObjectContext EditorContext;
	// ~Variables

public:
	// ~Delegates
	FSimpleMulticastDelegate OnUGCDownloadStartedDelegate;
	FOnUGCDownloadComplete OnUGCDownloadCompleteDelegate;
	FOnUGCItemLoaded OnUGCItemLoadedDelegate;
	FOnValidateUGCHeader OnValidateUGCHeaderDelegate;
	// ~Delegates

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	// ~Super Interface

	// Gets the outer brick editor, or null
	ABrickEditor* GetBrickEditor() const;
	// Sets the editor context member
	void SetEditorContext(EBrickEditorObjectContext InContext)
	{
		EditorContext = InContext;
	}

	// Return whether this component is part of an editor
	bool IsInEditor() const
	{
		return EditorContext == EBrickEditorObjectContext::Editor;
	}

	// Whether this component is being used to capture thumbnails
	bool IsInThumbnailCapture() const
	{
		return EditorContext == EBrickEditorObjectContext::ThumbnailRender;
	}

	// Whether the component was spawned in the world and is not part of an editor
	bool IsSpawned() const
	{
		return EditorContext == EBrickEditorObjectContext::None;
	}

	// Public accessor to the object pool
	FBrickEditorObjectPool* GetObjectPool() const
	{
		return ObjectPool.Get();
	}

	// Get the type of mod associated with the interface
	virtual EUGCType GetUGCType() const
	{
		checkNoEntry();
		return EUGCType::None;
	}

	// Whether any modded objects are used
	bool HasModdedBrickEditorObjects() const;

	// Save the item locally on disk
	bool SaveItem(const FUGCFileInfo& FileInfo);
	// Save the creation to a specific file
	bool SaveToFile(const FString& FileName);
	// Save this creation with all of it's properties and sub objects
	virtual bool SaveToArchive(FArchive& Ar);

private:
	// Saves the given list of objects to an archive
	bool SaveObjectsToArchive(const TArray<UBrickEditorObject*>& Objects, FArchive& Ar, FBrickEditorSaveHeader& OutHeader, bool bIsDuplicating);

public:
	// Load a UGC item, either from disk or from modio
	EBrickEditorLoadResult LoadItem(const FUGCFileInfo& FileInfo, bool bImport);
	// Load the creation from disk
	EBrickEditorLoadResult LoadFromFile(const FString& FileName, bool bImport);
	// Load the creation from the given archive
	EBrickEditorLoadResult LoadFromArchive(FArchive& Ar, TArray<uint8>& Data, bool bImport, bool bFromUndo);

private:
	// Called after successfully loading all objects and properties
	virtual EBrickEditorLoadResult PostLoadFromArchive(FArchive& Ar, const FBrickEditorSaveHeader& Header, bool bImport, const FBrickEditorReferenceResolver& ReferenceResolver)
	{
		return EBrickEditorLoadResult::Success;
	}

	// Loads objects contained in the archive
	EBrickEditorLoadResult LoadObjectsFromArchive(TArray<UBrickEditorObject*>& OutObjects, FArchive& Ar, const FBrickEditorSaveHeader& Header, EBrickEditorObjectCreationContext CreationContext, EAxis::Type MirrorAxis);
	// Internal version used to optimize the legacy and non legacy file paths
	template <bool bIsLegacyFile>
	EBrickEditorLoadResult LoadObjectsFromArchiveInternal(TArray<UBrickEditorObject*>& OutObjects, FArchive& Ar, const FBrickEditorSaveHeader& Header, EBrickEditorObjectCreationContext CreationContext, EAxis::Type MirrorAxis);

public:
	// Loads the header only for a given save file
	static bool LoadHeaderFromArchive(FArchive& Ar, FBrickEditorSaveHeader& OutHeader);
	// Destroys all placed objects and resets the item state
	bool ClearItem(bool bCallPostDestroy = true);

private:
	// Allows subclasses to do some cleanup
	virtual void PostClearItem()
	{
	}

public:
	// Whether the UGC item is currently being downloaded
	bool IsDownloadingItem() const;
	// Get the current progress of the item download if a download is in progress
	TOptional<float> GetItemDownloadProgress() const;

	// ~Num Objects
	// Get the number of spawned objects
	int32 GetNumBrickEditorObjects() const
	{
		return BrickEditorObjects.Num();
	}

	// Get the number of objects that fulfill the given predicate
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	int32 GetNumBrickEditorObjectsByPredicate(const TFunction<bool(C*)>& Predicate) const
	{
		auto OutNum = 0;
		ForEachBrickEditorObject<B, C>([&](auto* Object)
		{
			if (Predicate(Object))
			{
				OutNum++;
			}
		});
		return OutNum;
	}

	// Get the number of editor objects matching the given class
	template <class B = UBrickEditorObject, class C>
	int32 GetNumBrickEditorObjectsByClass() const
	{
		return GetNumBrickEditorObjectsByPredicate<B, C>(true);
	}

	// Version that uses a class pointer
	template <class B = UBrickEditorObject>
	int32 GetNumBrickEditorObjectsByClass(const UClass* InClass) const
	{
		return GetNumBrickEditorObjectsByPredicate<B, B>([&](const auto* Object)
		{
			return Object->IsA(InClass);
		});
	}

	// Get the number of objects that are of the given static info class
	int32 GetNumBrickEditorObjectsByStaticInfoClass(const UClass* InClass) const
	{
		auto OutNum = 0;
		ForEachBrickEditorObject<UBrickEditorObject, UBrickEditorObject>([&](auto* Object)
		{
			if (Object->GetStaticInfoClass()->IsChildOf(InClass))
			{
				OutNum++;
			}
		});
		return OutNum;
	}

	// ~Num Objects

	// ~Get Object
	// Returns the first object that matches the predicate
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	C* GetBrickEditorObjectByPredicate(const TFunction<bool(C*)>& Predicate) const
	{
		for (auto* Object : BrickEditorObjects)
		{
			if (std::is_same_v<B, C>)
			{
				if (Predicate(CastChecked<C>(Object)))
				{
					return CastChecked<C>(Object);
				}
			}
			else
			{
				auto* Casted = Cast<C>(Object);
				if (Casted && Predicate(Casted))
				{
					return Casted;
				}
			}
		}

		return nullptr;
	}

	// Returns the object with the given ID
	// NOTE: This relies on the object IDs not changing, so it's not safe to when objects are going to be added or removed during the lifetime
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	C* GetBrickEditorObjectByID(const FBrickEditorObjectID& ID) const
	{
		const auto Index = ID.GetObjectIndex();
		auto* Object = BrickEditorObjects.IsValidIndex(Index) ? BrickEditorObjects[Index] : nullptr;
		if (std::is_same_v<B, C>)
		{
			return Object ? CastChecked<C>(Object) : nullptr;
		}
		return Cast<C>(Object);
	}

	// Returns the first object of the given class
	template <class B = UBrickEditorObject, class C>
	C* GetBrickEditorObjectByClass() const
	{
		for (auto* Object : BrickEditorObjects)
		{
			if (std::is_same_v<B, C>)
			{
				return CastChecked<C>(Object);
			}
			auto* Casted = Cast<C>(Object);
			if (Casted)
			{
				return Casted;
			}
		}

		return nullptr;
	}

	// WARNING: This cannot be used in the brick editor, also unsafe to use if bricks are being added or removed at play time
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	C* GetBrickEditorObjectByIndex(int32 Index) const
	{
		check(IsSpawned());
		auto* Object = BrickEditorObjects.IsValidIndex(Index) ? BrickEditorObjects[Index] : nullptr;
		if (std::is_same_v<B, C>)
		{
			return Object ? CastChecked<C>(Object) : nullptr;
		}
		return Cast<C>(Object);
	}

	// Searches for the mirrored object in the given array (object itself may be in it as well)
	UBrickEditorObject* GetMirroredBrickEditorObject(const UBrickEditorObject* Object, const EAxis::Type MirrorAxis, const TArray<UBrickEditorObject*>& Objects) const;
	// ~Get Object

	// ~Get Objects
	// Get the editor objects as a straight reference
	template <class B = UBrickEditorObject>
	const auto& GetBrickEditorObjects() const
	{
		return reinterpret_cast<const TArray<B*>&>(BrickEditorObjects);
	}

	// Get all objects matching a certain condition
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	void GetBrickEditorObjectsByPredicate(TArray<C*>& OutObjects, const TFunction<bool(C*)>& Predicate) const
	{
		OutObjects.Reserve(BrickEditorObjects.Num());
		for (auto* Object : BrickEditorObjects)
		{
			if (std::is_same_v<B, C>)
			{
				if (Predicate(CastChecked<C>(Object)))
				{
					OutObjects.Add(CastChecked<C>(Object));
				}
			}
			else
			{
				auto* Casted = Cast<C>(Object);
				if (Casted && Predicate(Casted))
				{
					OutObjects.Add(Casted);
				}
			}
		}
		OutObjects.Shrink();
	}

	// Get all objects of the given class
	template <class B = UBrickEditorObject, class C>
	void GetBrickEditorObjectsByClass(TArray<C*>& OutObjects) const
	{
		GetBrickEditorObjectsByPredicate<B, C>(OutObjects, [](auto*) { return true; });
	}

	// Version that uses a class pointer instead of a template argument
	template <class B = UBrickEditorObject>
	void GetBrickEditorObjectsByClass(const UClass* InClass, TArray<B*>& OutObjects) const
	{
		GetBrickEditorObjectsByPredicate<B, B>(OutObjects, [&](auto* Object)
		{
			return Object->IsA(InClass);
		});
	}

	// Find all objects with the same static info class
	template <class B = UBrickEditorObject>
	void GetBrickEditorObjectsByStaticInfoClass(const UClass* InClass, TArray<UBrickEditorObject*>& OutObjects) const
	{
		GetBrickEditorObjectsByPredicate<B, B>(OutObjects, [&](auto* Object)
		{
			return Object->GetStaticInfoClass() == InClass;
		});
	}

	// Gets all objects that have the same property with an identical value
	void GetBrickEditorObjectsByPropertyValue(const FBrickPropertyInstance& Property, const UBrickEditorObject* SelectedObject, TArray<UBrickEditorObject*>& OutObjects) const;
	// ~Get Objects

	// ~Get ID
	// Returns the ID of the first object with the given predicate
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	FBrickEditorObjectID GetBrickEditorObjectIDByPredicate(const TFunction<bool(C*)>& Predicate) const
	{
		return FBrickEditorObjectID(GetBrickEditorObjectByPredicate<B, C>(Predicate));
	}

	// Returns the ID of the first object of the given class
	template <class B = UBrickEditorObject, class C>
	FBrickEditorObjectID GetBrickEditorObjectIDByClass() const
	{
		return GetBrickEditorObjectIDByPredicate<B, C>(true);
	}

	// ~Get ID

	// ~Get IDs
	// Returns the IDs of all objects that match the given predicate
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	void GetBrickEditorObjectIDsByPredicate(TArray<FBrickEditorObjectID>& OutIDs, const TFunction<bool(C*)>& Predicate) const
	{
		OutIDs.Reserve(BrickEditorObjects.Num());
		for (auto* Object : BrickEditorObjects)
		{
			if (std::is_same_v<B, C>)
			{
				if (Predicate(CastChecked<C>(Object)))
				{
					OutIDs.Add(Object->GetEditorObjectID());
				}
			}
			else
			{
				auto* Casted = Cast<C>(Object);
				if (Casted && Predicate(Casted))
				{
					OutIDs.Add(Object->GetEditorObjectID());
				}
			}
		}
		OutIDs.Shrink();
	}

	// Returns the IDs of all objects of the given class
	template <class B = UBrickEditorObject, class C>
	void GetBrickEditorObjectIDsByClass(TArray<FBrickEditorObjectID>& OutIDs) const
	{
		GetBrickEditorObjectIDsByPredicate<B, C>(OutIDs, [](auto* Object) { return true; });
	}

	// ~Get IDs

	// ~For Each
	// Executes a function on each object of a matching class
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	void ForEachBrickEditorObject(const TFunction<void(C*)>& Func) const
	{
		for (auto* Object : BrickEditorObjects)
		{
			if (std::is_same_v<B, C>)
			{
				Func(CastChecked<C>(Object));
			}
			else
			{
				auto* Casted = Cast<C>(Object);
				if (Casted)
				{
					Func(Casted);
				}
			}
		}
	}

	// Allows iterating objects in a specific range
	template <class B = UBrickEditorObject, class C = UBrickEditorObject>
	void ForEachBrickEditorObjectInRange(int32 StartIdx, int32 EndIdx, const TFunction<bool(C*, int32)>& Func) const
	{
		for (auto i = StartIdx; i <= EndIdx; ++i)
		{
			auto* Object = BrickEditorObjects[i];
			if (std::is_same_v<B, C>)
			{
				if (!Func(CastChecked<C>(Object), i))
				{
					break;
				}
			}
			else
			{
				if (auto* Casted = Cast<C>(Object))
				{
					if (!Func(Casted, i))
					{
						break;
					}
				}
			}
		}
	}

	// ~For Each

	// Implement to get a list of all object classes that can be spawned
	virtual const TArray<UClass*>& GetBrickEditorObjectLibrary() const;
	// Get the class that should be placed when the brick is mirrored
	UClass* GetBrickEditorObjectMirroredClass(UClass* InStaticInfoClass) const;
	// Used to determine if an object can be placed
	virtual bool CanCreateBrickEditorObject(UClass* InStaticInfoClass) const;
	// Can be used to place a single object, optionally also a mirrored counterpart
	// NOTE: This should not be used when multiple objects are added/duplicated for performance reasons
	UBrickEditorObject* CreateSingleBrickEditorObject(UClass* InStaticInfoClass, const FVector& InLocation, const FRotator& InRotation, EAxis::Type MirrorAxis = EAxis::None);

private:
	// Creates a new editor object, optionally also a mirrored counterpart
	UBrickEditorObject* CreateBrickEditorObject(UClass* InStaticInfoClass, EAxis::Type MirrorAxis, const FBrickRigsSaveVersion& Version, const FLegacyBrickEditorObjectClassID& LegacyClassId);
	// To be called BEFORE creating any objects
	void PreCreateBrickEditorObjects(int32 NumObjects);
	// Called after new objects have been created
	void PostCreateBrickEditorObjects(const TArray<UBrickEditorObject*>& NewObjects, EBrickEditorObjectCreationContext CreationContext, FBrickRigsSaveVersion Version = BR_SAVE_VERSION, const FBrickEditorReferenceResolver* ReferenceResolver = nullptr, const TArray<FLegacyBrickEditorObjectClassID>* LegacyClassIds = nullptr);
	// Called before PostLoad on objects
	virtual void PostLoadBrickEditorObject(const TArray<UBrickEditorObject*>& NewObjects, FBrickRigsSaveVersion Version, const FBrickEditorReferenceResolver* ReferenceResolver)
	{
	}

public:
	// Creates a duplicate of all provided objects, optionally mirroring them along an arbitrary axis
	void DuplicateBrickEditorObjects(const TArray<UBrickEditorObject*>& ObjectsToDuplicate, TArray<UBrickEditorObject*>& OutNewObjects, EAxis::Type MirrorAxis = EAxis::None, bool bDestroyOriginalObjects = false);
	// Destroy an array of editable objects, we use an array instead of a single object so implementations can make array removals and resizes more efficient
	bool DestroyBrickEditorObjects(const TArray<UBrickEditorObject*>& InObjects, bool bCallPostDestroy = true);

private:
	// Can be implemented to be notified about destroyed objects
	virtual void PreDestroyBrickEditorObjects(const TArray<UBrickEditorObject*>& DestroyedObjects, bool bIsDestroyingAllObjects)
	{
	}

	// To be called after objects have been destroyed, clears the object pool
	void PostDestroyBrickEditorObjects() const;

	// Updates the IDs of all objects
	void UpdateBrickEditorObjectIDs() const;

public:
	// Notifies the interface before and after objects are being initialized
	virtual void PreInitializeBrickEditorObjects()
	{
	}

	virtual void PostInitializeBrickEditorObjects()
	{
	}

	// Rearranges the order of an object within its class
	bool ReorderBrickEditorObject(UBrickEditorObject* Object, const UClass* ObjectClass, const int32 NewOrder);

	// Returns the index of an object among objects of its class
	int32 GetBrickEditorObjectOrder(const UBrickEditorObject* Object, const UClass* ObjectClass) const;

	// Reorders the given objects to the given indices
	void ReorderBrickEditorObjects(const TArray<int32>& Indices, const TArray<UBrickEditorObject*>& Objects);

	// Tries to find the best position for a camera to encapsulate all given objects
	TOptional<FVector> FocusCameraOnBrickEditorObjects(const TArray<UBrickEditorObject*>& Objects, const FTransform& CameraTransform, const FVector2D& FOV, float Margin) const;

	// Calculates the spawn relative bounds of all objects
	void CalcBrickEditorObjectsBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const
	{
		CalcBrickEditorObjectsBounds(BrickEditorObjects, OutBoundsMin, OutBoundsMax);
	}

	// Calculates the combined bounds from a list of objects
	// Template so an array with any subclass of editor objects can be used
	template <class T>
	void CalcBrickEditorObjectsBounds(const TArray<T*>& Objects, FVector& OutBoundsMin, FVector& OutBoundsMax) const
	{
		if (Objects.Num())
		{
			OutBoundsMin = FVector(FLT_MAX);
			OutBoundsMax = FVector(-FLT_MAX);

			// Go through all object to find the minimum and maximum bounds
			for (auto* Object : Objects)
			{
				Object->GetBrickEditorObjectSpawnRelativeBounds(OutBoundsMin, OutBoundsMax);
			}
		}
		else
		{
			OutBoundsMin = FVector::ZeroVector;
			OutBoundsMax = FVector::ZeroVector;
		}
	}

	// Calculates the mass of all objects
	float CalcBrickEditorObjectsMass() const
	{
		return CalcBrickEditorObjectsMass(BrickEditorObjects);
	}

	// Calculates the mass of a list of object
	float CalcBrickEditorObjectsMass(const TArray<UBrickEditorObject*>& Objects) const;

	// Calculates the price of all objects
	float CalcBrickEditorObjectsPrice() const;

	// Can be implemented to count objects with aerodynamics enabled
	virtual int32 GetNumBrickEditorObjectsWithAerodynamics() const
	{
		return 0;
	}

	// Get the current transform of the spawn root
	FTransform GetBrickEditorSpawnRootTransform() const
	{
		return GetComponentTransform();
	}

	// Captures a new thumbnail for the represented item
	void CaptureItemThumbnail(const FUGCFileInfo& FileInfo, bool bFocusItem, bool bShowOnlyItem, bool bShowNotification);
};
