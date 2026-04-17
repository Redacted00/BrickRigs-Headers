#pragma once

#include "CoreMinimal.h"

struct FBrickEditorObjectPool
{
	// ~Constructor
	FBrickEditorObjectPool(UObject* InOuter = nullptr)
		: Outer(InOuter)
	{
	}

	// Returns an object from the pool or a new one
	template <class T>
	T* FindOrCreateObject(UClass* ObjectClass, const FName& ObjectName, bool& bOutHasBeenRecycled)
	{
		check(Outer.IsValid());

		T* OutObject = nullptr;

		// Try to find a pooled object
		if (auto* ObjectArray = ObjectMap.Find(ObjectClass))
		{
			// Iterate backwards to make removals more performant
			for (auto i = ObjectArray->Num() - 1; i >= 0; --i)
			{
				// Save a weak reference and remove the entry from the pool
				auto WeakPtr = (*ObjectArray)[i];
				ObjectArray->RemoveAt(i, 1, false);

				if (WeakPtr.IsValid())
				{
					// IMPORTANT: If a specific name is required, only reuse objects with the same name
					// This has to be done to prevent object 0 to reuse SMC_1 for example, then object 1 creates SMC_1 for itself, deleting and recreating the already used component
					if (ObjectName.IsNone() || WeakPtr->GetFName() == ObjectName)
					{
						OutObject = CastChecked<T>(WeakPtr.Get());
						bOutHasBeenRecycled = true;
						break;
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("FBrickEditorObjectPool::FindOrCreateObject: Found an invalid object, removing it."));
				}
			}
			ObjectArray->Shrink();
		}

		// Create a new object if needed
		if (!OutObject)
		{
			OutObject = NewObject<T>(Outer.Get(), ObjectClass, ObjectName, RF_Transient);
			bOutHasBeenRecycled = false;
		}

		return OutObject;
	}

	// Adds an object back to the pool
	void RemoveObject(UObject* InObject)
	{
		check(InObject);
		auto& ObjectArray = ObjectMap.FindOrAdd(InObject->GetClass());
		check(!ObjectArray.Contains(InObject));
		ObjectArray.Add(InObject);
	}

	// Empties the pool and destroys all objects
	void DestroyObjects()
	{
		for (const auto& Pair : ObjectMap)
		{
			for (const auto& WeakPtr : Pair.Value)
			{
				if (WeakPtr.IsValid())
				{
					if (WeakPtr->IsA<UActorComponent>())
					{
						auto* Component = CastChecked<UActorComponent>(WeakPtr.Get());
						Component->DestroyComponent();
					}
					else
					{
						WeakPtr->MarkPendingKill();
					}
				}
			}
		}
		ObjectMap.Empty();
	}

private:
	// ~Variables
	// The object to use as the outer for new objects
	const TWeakObjectPtr<> Outer;
	// Maps an array of pooled objects to each class
	TMap<UClass*, TArray<TWeakObjectPtr<>>> ObjectMap;
	// ~Variables
};
