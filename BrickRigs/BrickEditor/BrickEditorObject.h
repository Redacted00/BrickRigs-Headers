// Copyright Fluppisoft, 2020

#pragma once

#include "UI/Misc/TooltipContent.h"
#include "BrickEditorTypes.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BrickEditorComponentInterface.h"
#include "BrickEditorObjectPool.h"
#include "Bricks/Misc/BrickUnits.h"
#include "BrickEditorSaveVersion.h"
#include "BrickEditorObjectID.h"
#include "GameplayTags.h"
#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "Misc/BrickDataSingleton.h"
#include "UObject/Object.h"
#include "BrickEditorObject.generated.h"

struct FDeprecatedClassID;
class UBrickEditorMode;
class ABrickEditor;
class UBrickEditorInterfaceComponent;
class UBrickEditorObject;
class UBrickEditorStaticInfo;
class UTexture2D;
struct FBrickEditorObjectPool;
struct FBrickEditorReferenceResolver;

enum class EBrickSelectionState : uint8
{
	Unselected,
	Selected,
	Active
};

enum class EBrickValidityState : uint8
{
	None,
	Neutral,
	Invalid,
	Valid
};

// NOTE: Lower value outlines are drawn on top of higher ones
enum class EBrickOutlineState : uint8
{
	Valid,
	Invalid,
	Neutral,
	Active,
	Selected,
	Unselected
};

// Struct used to pass along parameters more easily
struct FResolveBrickPropertyParams
{
	// ~Variables
	// Archive that can be used to load the data
	FArchive& Ar;
	const FBrickEditorReferenceResolver& ReferenceResolver;

private:
	// The version currently being loaded
	const FBrickRigsSaveVersion Version;
	// The name of the property that failed to be loaded
	const FString PropertyName;
	// Whether the property has been removed entirely, otherwise just the value wasn't accepted
	const bool bPropertyRemoved;
	// ~Variables

public:
	// ~Constructor
	FResolveBrickPropertyParams(FArchive& InAr, const FBrickEditorReferenceResolver& InReferenceResolver, const FBrickRigsSaveVersion& InVersion, const FString& InPropertyName, bool bInPropertyRemoved)
		: Ar(InAr), ReferenceResolver(InReferenceResolver), Version(InVersion), PropertyName(InPropertyName), bPropertyRemoved(bInPropertyRemoved)
	{
	}

	const FBrickRigsSaveVersion& GetVersion() const
	{
		return Version;
	}

	const FString& GetPropertyName() const
	{
		return PropertyName;
	}

	FString GetParentPropertyName() const
	{
		auto DotIndex = int32();
		const auto bSuccess = PropertyName.FindChar('.', DotIndex);
		check(bSuccess);
		return PropertyName.Left(DotIndex);
	}

	// Convenience macro useful for being able to enter the properties directly
#define ResolveInvalidBrickPropertyValue(Property) ResolveInvalidBrickPropertyValueInternal(Property, #Property)

	// NOTE: The property itself is passed as a parameter to provoke a compile error in case it doesn't exist
	template <typename PropertyType>
	bool ResolveInvalidBrickPropertyValueInternal(const PropertyType& Property, const FString& InPropertyName) const
	{
		return !bPropertyRemoved && InPropertyName == PropertyName;
	}

	// Start resolving a property that has been removed entirely
	bool ResolveRemovedBrickProperty(const FString& InPropertyName, bool bSearchSuffix = false) const
	{
		return bPropertyRemoved && (bSearchSuffix ? PropertyName.EndsWith(InPropertyName, ESearchCase::IgnoreCase) : InPropertyName == PropertyName);
	}
};

enum class EBrickEditorObjectContext : uint8
{
	None,
	// None means spawned, not in editor
	Editor,
	ThumbnailRender
};

// List of editor only params instantiated on every object
struct FBrickEditorObjectEditorParams
{
	// The current selection state
	EBrickSelectionState SelectionState = EBrickSelectionState::Unselected;
	// Optional validity state, used for the move mode for example
	EBrickValidityState ValidityState = EBrickValidityState::None;
	// The current outline state/stencil, used to prevent unecessary updates
	EBrickOutlineState OutlineState = EBrickOutlineState::Valid;
	// Object this one is mirrored from
	TWeakObjectPtr<UBrickEditorObject> MoveMirroredObject;
	// Whether the object has been created as mirrored
	uint16 bCreatedAsMirrored : 1;
	// Whether the object is currently hovered in the editor
	uint16 bIsHovered : 1;
	// Flags used for different hidden states
	uint16 bIsHidden : 1;
	uint16 bIsHiddenByUser : 1;
	uint16 bIsHiddenByEditorMode : 1;
	uint16 bIsHiddenByViewMode : 1;
	// Flags used to indicate if the editor visualizations should currently be visible
	uint16 bShowUnselectedVisualization : 1;
	uint16 bShowSelectedVisualization : 1;
	uint16 bShowActiveVisualization : 1;
	// Flags used for the move mode
	uint16 bIsBeingMoved : 1;
	uint16 bIsMoveRoot : 1;
	uint16 bIsMoveMirrored : 1;
};

struct FSetupBrickEditorObjectDefaultsParams
{
	const FBrickRigsSaveVersion& Version;
	const FLegacyBrickEditorObjectClassID& LegacyClassId;
};

UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UBrickEditorObjectStaticInfo : public UObject
{
	GENERATED_BODY()

public:
	// ~Properties
	// The class to create object instances from
	UPROPERTY(EditDefaultsOnly, NoClear, Category = EditorObject)
	TSubclassOf<UBrickEditorObject> ObjectClass;
	// Display name of the object type
	UPROPERTY(EditDefaultsOnly, Category = EditorObject)
	FText ObjectTypeDisplayName;
	// Description of the object
	UPROPERTY(EditDefaultsOnly, Category = EditorObject)
	FText Description;
	// Price of the object
	UPROPERTY(EditDefaultsOnly, Category = EditorObject)
	float Price;

	// The mode used for mirroring in the editor, influences how the brick will be rotated
	UPROPERTY(EditDefaultsOnly, Category = Mirroring)
	EMirrorMode MirrorMode;
	// Optional manual override for the mirrored class
	UPROPERTY(EditDefaultsOnly, Category = Mirroring, meta = (AllowedClasses = "BrickEditorObject"))
	FSoftClassPath MirrorClassOverride;
	// The relative origin the object is rotated around when mirroring, which should be 0 for most cases
	UPROPERTY(EditDefaultsOnly, Category = Mirroring)
	FVector MirrorRotationOrigin;
	// ~Properties

	// ~Super Interface
	virtual void PostLoad() override;
#if WITH_EDITOR
	virtual void PreSave(const ITargetPlatform* TargetPlatform) override;
	virtual void PostTransacted(const FTransactionObjectEvent& TransactionEvent) override;
	virtual void BeginCacheForCookedPlatformData(const ITargetPlatform* TargetPlatform) override;
#endif
	// ~Super Interface

	// Get the display name for the type of the object
	virtual FText GetObjectTypeDisplayName() const
	{
		return ObjectTypeDisplayName;
	}

	// Get the desired display name
	virtual FText GetDisplayName() const;
	// Get tags for filtering in the brick editor
	virtual void GetBrickEditorFilterTags(FGameplayTagContainer& OutTags) const
	{
	}

	// Can be implemented for bricks
	virtual FBrickSize GetBrickEditorObjectSize() const { return FBrickSize(); }
	// Returns the default price of the object
	virtual float GetBrickEditorObjectPrice() const { return Price; }
	// Used to sort the brick editor classes list
	virtual bool SortBrickEditorObject(const UBrickEditorObjectStaticInfo* Other) const;
	// Used to limit the number of objects that can be placed
	virtual int32 GetMaxInstances(UClass*& OutClass) const
	{
		OutClass = nullptr;
		return 0;
	}

	// Used to populate the tooltip shown for the object class
	virtual void GetTooltipContent(FTooltipContent& OutContent) const;

#if WITH_EDITOR

protected:
	// Can be overridden to setup cached data at build time, or when PIE is started
	virtual void BuildCachedData()
	{
	}
#endif
};

/**
 * This is the base class for anything that can be selected and edited in the editor
 */
UCLASS()
class BRICKRIGS_API UBrickEditorObject : public UObject, public IBrickPropertyInterface
{
	GENERATED_BODY()

	// ~Variables
	// The static info this object has been instantiated from
	UPROPERTY(Transient)
	UClass* StaticInfoClass;
	// The unique ID of this object, this is only updated and used before saving
	FBrickEditorObjectID EditorObjectID;
	// Editor only params
	TUniquePtr<FBrickEditorObjectEditorParams> EditorParams;
	// Whether the object has been fully initialized, including post initialization
	uint8 bIsInitialized : 1;
	// Whether the object is being initialized or uninitialized right now
	uint8 bIsBeingInitialized : 1;
	uint8 bIsBeingUninitialized : 1;
	// Whether this object is in the editor, spawned etc.
	EBrickEditorObjectContext EditorContext : 2;

protected:
	// Relative spawn location
	FVector SpawnLocation;
	// Relative spawn rotation
	FRotator SpawnRotation;

	// Components that are being used on this object
	TArray<TSharedRef<FBrickEditorComponentPtr>> Components;
	// The root component that manages attachment, collision etc
	TBrickEditorComponentPtr<UPrimitiveComponent> RootComponent;
	// ~Variables

public:
	// Used to spawn a new editor object
	static UBrickEditorObject* CreateBrickEditorObject(
		UClass* InStaticInfo,
		const FName& InObjectName,
		const FBrickEditorObjectID& InObjectID,
		EBrickEditorObjectContext InEditorContext,
		const TSharedRef<FBrickEditorObjectPool>& InObjectPool,
		const FBrickRigsSaveVersion& Version,
		const FLegacyBrickEditorObjectClassID& LegacyClassId,
		bool bInCreatedAsMirrored,
		bool bDeferPostLoad
	);
	// To be called after properties have been loaded
	static void PostLoadBrickEditorObject(UBrickEditorObject* InObject, FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver = nullptr);
	// To be called after initialization (in the editor after loading, on the vehicle after creating connections)
	template <bool bIsInEditor>
	static void PostInitializeBrickEditorObject(UBrickEditorObject* InObject)
	{
		check(!InObject->bIsInitialized);
		InObject->bIsBeingInitialized = true;
		InObject->bIsInitialized = true;
		InObject->PostInitializeBrickEditorObject();
		if constexpr (bIsInEditor)
		{
			// Initialize the editor visualization
			InObject->UpdateShowEditorVisualization(true);
		}
		InObject->bIsBeingInitialized = false;
	}

	// Uninitializes an object, removes components etc
	static void UninitializeBrickEditorObject(UBrickEditorObject* InObject);
	// Used to clean up an object once it is no longer needed
	static void DestroyBrickEditorObject(UBrickEditorObject* InObject);

	// ~Super Interface
	virtual UWorld* GetWorld() const override;

	virtual bool IsNameStableForNetworking() const override
	{
		return true;
	}

	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}

	// ~Super Interface

	// ~Static Info
	// Get the casted static info CDO, always returns a valid pointer
	template <class T>
	const T* GetStaticInfo() const
	{
		if (!StaticInfoClass || !StaticInfoClass->IsChildOf<T>())
		{
			ensureMsgf(false, TEXT("Static info %s with object %s can't be converted to %s"), *GetNameSafe(StaticInfoClass), *GetName(), *GetNameSafe(T::StaticClass()));
			return GetDefault<T>();
		}

		return StaticInfoClass->GetDefaultObject<T>();
	}

	// Default version
	UFUNCTION(BlueprintPure)
	const UBrickEditorObjectStaticInfo* GetStaticInfo() const
	{
		return GetStaticInfo<UBrickEditorObjectStaticInfo>();
	}

	// Get the static info class associated with this object
	UFUNCTION(BlueprintPure)
	UClass* GetStaticInfoClass() const
	{
		return StaticInfoClass;
	}

	// ~Static Info

	// ~ID
	// Change the unique ID
	void SetEditorObjectID(const FBrickEditorObjectID& NewID)
	{
		EditorObjectID = NewID;
	}

	// Get the unique ID
	UFUNCTION(BlueprintPure)
	const FBrickEditorObjectID& GetEditorObjectID() const
	{
		return EditorObjectID;
	}

	// Get the index of the object within the owner's object array
	// WARNING: This does not work in the editor
	int32 GetBrickEditorObjectIndex() const
	{
		check(IsSpawned());
		return EditorObjectID.GetObjectIndex();
	}

	// ~ID

	// ~User Interface
	// Gets the current desired display name
	UFUNCTION(BlueprintPure)
	virtual FText GetBrickEditorObjectDisplayName() const
	{
		const auto* StaticInfo = GetStaticInfo();
		return StaticInfo->GetDisplayName();
	}

	// Returns the display name for the type of the object
	UFUNCTION(BlueprintPure)
	FText GetBrickEditorObjectTypeDisplayName() const
	{
		const auto* StaticInfo = GetStaticInfo();
		return StaticInfo->ObjectTypeDisplayName;
	}

	template <typename T>
	static FText StaticBrickEditorObjectTypeDisplayName()
	{
		return GetDefault<T>()->ObjectTypeDisplayName;
	}

	// Returns a human readable name for the object
	FString GetDebugName() const
	{
		return FString::Printf(TEXT("%s_%d"), *GetBrickEditorObjectDisplayName().ToString(), EditorObjectID.GetObjectIndex());
	}

	// ~User Interface

	// ~Editor
	// Returns whether the object is part of an editor
	UFUNCTION(BlueprintPure)
	bool IsInEditor() const
	{
		return EditorContext == EBrickEditorObjectContext::Editor;
	}

	// Whether this object is being used to capture thumbnails
	UFUNCTION(BlueprintPure)
	bool IsInThumbnailRender() const
	{
		return EditorContext == EBrickEditorObjectContext::ThumbnailRender;
	}

	// Whether the object was spawned in the world and is not part of an editor
	UFUNCTION(BlueprintPure)
	bool IsSpawned() const
	{
		return EditorContext == EBrickEditorObjectContext::None;
	}

	// Gets the outer editor interface for this object
	UFUNCTION(BlueprintPure)
	virtual UBrickEditorInterfaceComponent* GetEditorInterface() const;

	// Gets the outer editor
	UFUNCTION(BlueprintPure)
	ABrickEditor* GetBrickEditor() const;

	// Returns the static info from the editor, return value is guaranteed to be valid
	UFUNCTION(BlueprintPure)
	const UBrickEditorStaticInfo* GetBrickEditorStaticInfo() const;

	// Returns the object pool from the outer interface
	const TSharedRef<FBrickEditorObjectPool>& GetObjectPool() const;

	// Returns the current editor mode
	UBrickEditorMode* GetBrickEditorMode() const;

	// Returns the editor view mode currently in use
	EBrickEditorViewMode GetViewMode() const;

	// Whether the object was originally mirrored
	bool WasCreatedAsMirrored() const
	{
		return EditorParams.IsValid() && EditorParams->bCreatedAsMirrored;
	}

	// Can be called from the PostModifyBrickProperty to see if the property was set on the mirrored parent
	bool IsPostModifyBrickPropertyMirrored(const FBrickPropertyChangedEvent& Event) const;

	// Called when the current view mode has changed
	virtual void OnViewModeChanged(EBrickEditorViewMode NewMode);

	// Called when the current editor mode has changed
	virtual void OnEditorModeChanged(UBrickEditorMode* NewMode);

protected:
	// Returns the editor params casted to the given type
	template <typename T = FBrickEditorObjectEditorParams>
	T* GetEditorParams() const
	{
		return EditorParams.IsValid() ? static_cast<T*>(&*EditorParams) : nullptr;
	}

private:
	// Can be overridden for custom editor params classes
	virtual TUniquePtr<FBrickEditorObjectEditorParams> CreateEditorParams() const
	{
		return MakeUnique<FBrickEditorObjectEditorParams>();
	}

	// ~Editor

public:
	// ~Initialization
	// Returns whether the object has been initialized yet
	UFUNCTION(BlueprintPure)
	bool IsBrickEditorObjectInitialized() const
	{
		return bIsInitialized;
	}

	// Whether the object is being initialized right now
	UFUNCTION(BlueprintPure)
	bool IsBrickEditorObjectBeingInitialized() const
	{
		return bIsBeingInitialized;
	}

	// Whether the object is being uninitialized right now
	UFUNCTION(BlueprintPure)
	bool IsBrickEditorObjectBeingUninitialized() const
	{
		return bIsBeingUninitialized;
	}

	// Sets up the default values on the CDO
	void SetupCDODefaults(UClass* InStaticInfoClass);

private:
	// Initializes the object
	void InitializeBrickEditorObject(UClass* InStaticInfoClass, const FBrickEditorObjectID& InObjectID, EBrickEditorObjectContext InEditorContext, const FBrickRigsSaveVersion& Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, bool bInHasBeenRecycled, bool bInCreatedAsMirrored);

	// Uninitializes and cleans up the object
	void UninitializeBrickEditorObjectWrapper();

	// Gives the object a chance to set up its default values, called before it is loaded
	virtual void SetupBrickEditorObjectDefaults(const FSetupBrickEditorObjectDefaultsParams& Params)
	{
	}

	// Called after the object has been created and properties have been loaded
	virtual void PostLoadBrickEditorObject(FBrickRigsSaveVersion Version, const FLegacyBrickEditorObjectClassID& LegacyClassId, const FBrickEditorReferenceResolver* ReferenceResolver)
	{
	}

	// This is called after loading and after such initialization as creating brick connections
	virtual void PostInitializeBrickEditorObject()
	{
	}

	// Called to clean up the object before destruction
	virtual void UninitializeBrickEditorObject()
	{
	}

	// Called upon recycling, allows objects to clear up custom data
	virtual void RecycleBrickEditorObject()
	{
	}

	// ~Initialization

public:
	// ~Components
	// Public accessor to the root component
	UFUNCTION(BlueprintPure)
	UPrimitiveComponent* GetRootComponent() const
	{
		return RootComponent.Get();
	}

	// Get all sub components of the given type
	template <class T>
	void GetComponents(TArray<T*>& OutComponents) const
	{
		OutComponents.Reserve(Components.Num());
		for (const auto& CompPtr : Components)
		{
			if (CompPtr->IsValid())
			{
				if (CompPtr->Get()->IsA<T>())
				{
					OutComponents.Add(CastChecked<T>(CompPtr->Get()));
				}
			}
			else
			{
				ensure(false);
			}
		}
		OutComponents.Shrink();
	}

	// Get the total number of components
	int32 GetNumComponents() const
	{
		return Components.Num();
	}

	// Returns whether this object contains the given componentt
	bool HasComponent(UActorComponent* Comp) const
	{
		for (const auto& CompPtr : Components)
		{
			if (CompPtr->Get() == Comp)
			{
				return true;
			}
		}

		return false;
	}

protected:
	// Sets required parameters on a root component to spawn
	virtual void SetupCreateRootComponentParams(FBrickEditorPrimitiveComponentParams& Params);

	// Gets the custom depth values that should be applied to the root component
	void GetRootComponentCustomDepth(bool& bOutEnable, uint8& OutStencil) const;

	// Adds a new component to this object
	template <class ComponentType, typename ParamType>
	TBrickEditorComponentPtr<ComponentType> CreateBrickEditorComponent(ParamType& Params, const FName& Name = NAME_None)
	{
		check(bIsInitialized);
		// Let the pool create a new component
		bool bHasBeenRecycled;
		ComponentType* NewComponent = GetObjectPool()->FindOrCreateObject<ComponentType>(Params.GetComponentClass(), Name, bHasBeenRecycled);
		// Initialize the component interface
		IBrickEditorComponentInterface* ComponentInterface = Cast<IBrickEditorComponentInterface>(NewComponent);
		if (ComponentInterface)
		{
			ensure(ComponentInterface->GetBrickEditorObject() == nullptr);
			ComponentInterface->SetBrickEditorObject(this);
		}
		// Let the params initialize the component
		Params.InitializeComponent(NewComponent);

		// Register the new component
		if (!Params.bDeferRegistration)
		{
			ensure(!NewComponent->IsRegistered());
			NewComponent->RegisterComponent();
		}

		// Add a pointer to the components array
		const TSharedRef<FBrickEditorComponentPtr> NewCompPtr = MakeShared<FBrickEditorComponentPtr>(NewComponent);
		Components.Add(NewCompPtr);
		return TBrickEditorComponentPtr<ComponentType>(NewCompPtr);
	}

	// Removes and pooles a component
	template <class ComponentType>
	void RemoveBrickEditorComponent(const TBrickEditorComponentPtr<ComponentType>& CompPtr)
	{
		check(CompPtr.IsValid());
		const int32 Index = Components.Find(CompPtr.GetComponentPtr().ToSharedRef());
		check(Index != INDEX_NONE);
		RemoveBrickEditorComponentAtIndex(Index);
	}

	// Removes a component at a specific index in the components array
	void RemoveBrickEditorComponentAtIndex(int32 Index, bool bAllowShrinking = true)
	{
		check(Components.IsValidIndex(Index));

		// NOTE: Get a copy, since we remove the entry from the list right aray
		const auto CompPtr = Components[Index];
		// Remove the reference from the list of components
		Components.RemoveAt(Index, 1, bAllowShrinking);

		if (CompPtr->IsValid())
		{
			// NOTE: We have to detach the component manually, so subsequent calls of SetupAttachment can be done
			if (auto* SceneComponent = Cast<USceneComponent>(CompPtr->Get()))
			{
				SceneComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			}
			// Unregister the component
			CompPtr->Get()->UnregisterComponent();
			// Clear the object association
			if (auto* ComponentInterface = Cast<IBrickEditorComponentInterface>(CompPtr->Get()))
			{
				ComponentInterface->SetBrickEditorObject(nullptr);
			}
			// Add the component back to the pool
			GetObjectPool()->RemoveObject(CompPtr->Get());
			// Reset the shared weak pointer, so if the object is holding a reference to it it will be reset as well
			CompPtr->Reset();
		}
	}

public:
	// Called from components when they are finished and want to be removed automatically
	void AutoRemoveBrickEditorComponent(const UActorComponent* InComponent)
	{
		for (auto i = 0; i < Components.Num(); ++i)
		{
			if (Components[i]->Get() == InComponent)
			{
				RemoveBrickEditorComponentAtIndex(i);
				return;
			}
		}
		ensure(false);
	}

	// ~Components

	// ~Transform
	// Get the relative bounding box extent
	UFUNCTION(BlueprintPure)
	virtual bool GetBrickEditorObjectLocalBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const
	{
		if (RootComponent.IsValid())
		{
			const auto LocalBounds = RootComponent->CalcLocalBounds();
			OutBoundsMin = LocalBounds.Origin - LocalBounds.BoxExtent;
			OutBoundsMax = LocalBounds.Origin + LocalBounds.BoxExtent;
			return true;
		}

		OutBoundsMin = FVector::ZeroVector;
		OutBoundsMax = FVector::ZeroVector;
		return false;
	}

	// Get the bounding extremas in an arbitrary reference transform
	void GetBrickEditorObjectBounds(const FTransform& RefTransform, FVector& OutBoundsMin, FVector& OutBoundsMax) const
	{
		FVector BoundsMin;
		FVector BoundsMax;
		if (GetBrickEditorObjectLocalBounds(BoundsMin, BoundsMax))
		{
			for (const auto X : {BoundsMin.X, BoundsMax.X})
			{
				for (const auto Y : {BoundsMin.Y, BoundsMax.Y})
				{
					for (const auto Z : {BoundsMin.Z, BoundsMax.Z})
					{
						const auto Point = RefTransform.TransformPositionNoScale(FVector(X, Y, Z));
						OutBoundsMin.X = FMath::Min(OutBoundsMin.X, Point.X);
						OutBoundsMin.Y = FMath::Min(OutBoundsMin.Y, Point.Y);
						OutBoundsMin.Z = FMath::Min(OutBoundsMin.Z, Point.Z);
						OutBoundsMax.X = FMath::Max(OutBoundsMax.X, Point.X);
						OutBoundsMax.Y = FMath::Max(OutBoundsMax.Y, Point.Y);
						OutBoundsMax.Z = FMath::Max(OutBoundsMax.Z, Point.Z);
					}
				}
			}
		}
	}

	// Get the bounding extremas in spawn relative transform
	void GetBrickEditorObjectSpawnRelativeBounds(FVector& OutBoundsMin, FVector& OutBoundsMax) const;

	// Returns the world origin of the bounds
	UFUNCTION(BlueprintPure)
	FVector GetBrickEditorObjectBoundsOrigin() const
	{
		FVector BoundsMin;
		FVector BoundsMax;
		GetBrickEditorObjectLocalBounds(BoundsMin, BoundsMax);
		return GetWorldTransform().TransformPosition((BoundsMax + BoundsMin) * 0.5f);
	}

	// Returns the transform of the editor root relative to this brick's world transform
	FTransform GetBrickEditorRootSpawnTransform() const
	{
		return GetBrickEditorObjectSpawnTransform().Inverse() * GetWorldTransform();
	}

	// Returns the spawn transform in world space, relative to the editor component
	FTransform GetBrickEditorObjectSpawnWorldTransform() const;

	// Get the editor relative spawn transform
	UFUNCTION(BlueprintPure)
	FTransform GetBrickEditorObjectSpawnTransform() const
	{
		return FTransform(SpawnRotation, SpawnLocation);
	}

	UFUNCTION(BlueprintPure)
	FVector GetBrickEditorObjectSpawnLocation() const
	{
		return SpawnLocation;
	}

	UFUNCTION(BlueprintPure)
	FRotator GetBrickEditorObjectSpawnRotation() const
	{
		return SpawnRotation;
	}

	// Returns the transform of the physics body, which might differ slightly from the component transform when custom projection is used
	UFUNCTION(BlueprintPure)
	FTransform GetBodyTransform() const;

	UFUNCTION(BlueprintPure)
	FTransform GetWorldTransform() const
	{
		// NOTE: If the root component hasn't been created yet, use the editor interface relative transform instead
		// This is important if bricks need to know other bricks' transform while spawning for example
		return RootComponent.IsValid() ? RootComponent->GetComponentTransform() : GetBrickEditorObjectSpawnWorldTransform();
	}

	FTransform GetRelativeTransform() const
	{
		ensure(RootComponent.IsValid());
		return RootComponent.IsValid() ? RootComponent->GetRelativeTransform() : FTransform::Identity;
	}

	// Returns the relative transform if attached to anything, otherwise identity
	FTransform GetParentRelativeTransform() const
	{
		ensure(RootComponent.IsValid());
		return RootComponent.IsValid() && RootComponent->GetAttachParent() ? RootComponent->GetRelativeTransform() : FTransform::Identity;
	}

	FVector GetWorldLocation() const
	{
		return GetWorldTransform().GetLocation();
	}

	FVector GetRelativeLocation() const
	{
		return RootComponent.IsValid() ? RootComponent->GetRelativeLocation() : FVector::ZeroVector;
	}

	FRotator GetWorldRotation() const
	{
		return GetWorldTransform().Rotator();
	}

	FRotator GetRelativeRotation() const
	{
		return RootComponent.IsValid() ? RootComponent->GetRelativeRotation() : FRotator::ZeroRotator;
	}

	FQuat GetWorldQuat() const
	{
		return GetWorldTransform().GetRotation();
	}

	FVector GetRelativeScale3D() const
	{
		return RootComponent.IsValid() ? RootComponent->GetRelativeScale3D() : FVector(1.f);
	}

	// Mirrors the object transform and properties
	// NOTE: This has to be called BEFORE the object is initialized
	void MirrorBrickEditorObject(EBrickEditorMirrorMode MirrorMode);

private:
	// Allows sub classes to mirror additonal data manually
	virtual void OnMirrorBrickEditorObject(EBrickEditorMirrorMode MirrorMode)
	{
	}

	// ~Transform

public:
	// ~Physics
	// Can be implemented to return the calculated mass
	virtual void CalcBrickEditorObjectMass(float& OutMass, FVector& OutCenterOfMass) const
	{
	}

	// Returns the size of the object in brick units
	virtual FBrickSize GetBrickEditorObjectSize() const
	{
		return GetStaticInfo()->GetBrickEditorObjectSize();
	}

	// Calculates the cost of an object
	virtual float CalcBrickEditorObjectPrice() const
	{
		return GetStaticInfo()->GetBrickEditorObjectPrice();
	}

	// Whether the root component is simulating physics
	bool IsSimulatingPhysics() const
	{
		return RootComponent.IsValid() && RootComponent->IsSimulatingPhysics();
	}

	// Wakes up the root component
	void WakeRootComponent()
	{
		if (RootComponent.IsValid())
		{
			RootComponent->WakeAllRigidBodies();
		}
	}

	// Get the total mass of the object
	float GetMass() const
	{
		return RootComponent.IsValid() ? RootComponent->GetMass() : 0.f;
	}

	// Get world location of the center of mass
	FVector GetCenterOfMass() const
	{
		return RootComponent.IsValid() ? RootComponent->GetCenterOfMass() : FVector::ZeroVector;
	}

	// Get the physics inertia tensor
	FVector GetInertiaTensor() const
	{
		return RootComponent.IsValid() ? RootComponent->GetInertiaTensor() : FVector::ZeroVector;
	}

	// Get the current linear velocity
	FVector GetPhysicsLinearVelocity() const
	{
		return RootComponent.IsValid() ? RootComponent->GetPhysicsLinearVelocity() : FVector::ZeroVector;
	}

	// Get the linear velocity at a specific point in space
	FVector GetPhysicsLinearVelocityAtPoint(const FVector& Location) const
	{
		return RootComponent.IsValid() ? RootComponent->GetPhysicsLinearVelocityAtPoint(Location) : FVector::ZeroVector;
	}

	// Get the current angular velocity in rads per second
	FVector GetPhysicsAngularVelocityInRadians() const
	{
		return RootComponent.IsValid() ? RootComponent->GetPhysicsAngularVelocityInRadians() : FVector::ZeroVector;
	}

	// Get the current angular velocity in degrees per second
	FVector GetPhysicsAngularVelocityInDegrees() const
	{
		return RootComponent.IsValid() ? RootComponent->GetPhysicsAngularVelocityInDegrees() : FVector::ZeroVector;
	}

	// Add physics force to the root component
	void AddForce(const FVector& InForce, const FName& InBoneName = NAME_None, bool bInAccelChange = false)
	{
		ensure(!InForce.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddForce(InForce, InBoneName, bInAccelChange);
		}
	}

	// Add physics force to the root component at a certain world location
	void AddForceAtLocation(const FVector& InForce, const FVector& InLocation, const FName& InBoneName = NAME_None)
	{
		ensure(!InForce.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddForceAtLocation(InForce, InLocation, InBoneName);
		}
	}

	// Add a physics impulse to the root component
	void AddImpulse(const FVector& InImpulse, const FName& InBoneName = NAME_None, bool bVelChange = false)
	{
		ensure(!InImpulse.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddImpulse(InImpulse, InBoneName, bVelChange);
		}
	}

	// Add physics impulse to the root component at a certain world location
	void AddImpulseAtLocation(const FVector& InImpulse, const FVector& InLocation, const FName& InBoneName = NAME_None)
	{
		ensure(!InImpulse.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddImpulseAtLocation(InImpulse, InLocation, InBoneName);
		}
	}

	// Adds torque to the root component
	void AddTorqueInRadians(const FVector& InTorque, const FName& InBoneName = NAME_None, bool bInAccelChange = false)
	{
		ensure(!InTorque.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddTorqueInRadians(InTorque, InBoneName, bInAccelChange);
		}
	}

	// Adds an angular impulse to the root component
	void AddAngularImpulseInRadians(const FVector& Impulse, const FName& InBoneName = NAME_None, bool bVelChange = false)
	{
		ensure(!Impulse.ContainsNaN());
		if (RootComponent.IsValid())
		{
			RootComponent->AddAngularImpulseInRadians(Impulse, InBoneName, bVelChange);
		}
	}

	// Returns the physics volume the object is inside of, always returns a valid pointer
	APhysicsVolume* GetPhysicsVolume() const
	{
		return RootComponent.IsValid() ? RootComponent->GetPhysicsVolume() : GetWorld()->GetDefaultPhysicsVolume();
	}

	// ~Physics

	// ~Selection
	// Set the current selection state
	void SetSelectionState(EBrickSelectionState NewState)
	{
		if (EditorParams.IsValid() && NewState != EditorParams->SelectionState)
		{
			EditorParams->SelectionState = NewState;
			UpdateOutlineState();
			UpdateShowEditorVisualization();
			OnSelectionStateChanged(EditorParams->SelectionState);
		}
	}

	// Can be implemented to achieve custom behavior
	virtual void OnSelectionStateChanged(EBrickSelectionState NewState)
	{
	}

	// Get the current selection state
	EBrickSelectionState GetSelectionState() const
	{
		return EditorParams.IsValid() ? EditorParams->SelectionState : EBrickSelectionState::Unselected;
	}

	// Whether this brick is selected, either active or just selected
	bool IsBrickSelected() const
	{
		return EditorParams.IsValid() && EditorParams->SelectionState != EBrickSelectionState::Unselected;
	}

	// Sets the hovered state for the editor
	void SetIsHovered(bool bNewHovered)
	{
		if (EditorParams.IsValid() && bNewHovered != EditorParams->bIsHovered)
		{
			EditorParams->bIsHovered = bNewHovered;
			// NOTE: The outline state is not affected by the hovered state, only the custom depth
			UpdateCustomDepth();
		}
	}

	// Sets the editor validity state, used for special outline colors when moving for example
	void SetValidityState(EBrickValidityState NewState)
	{
		if (EditorParams.IsValid() && NewState != EditorParams->ValidityState)
		{
			EditorParams->ValidityState = NewState;
			UpdateOutlineState();
		}
	}

	// Get the current validity state
	EBrickValidityState GetValidityState() const
	{
		return EditorParams.IsValid() ? EditorParams->ValidityState : EBrickValidityState::None;
	}

	// ~Selection

	// ~Outline
	// Allows the editor to access the outline state
	EBrickOutlineState GetOutlineState() const
	{
		return EditorParams.IsValid() ? EditorParams->OutlineState : EBrickOutlineState::Unselected;
	}

protected:
	// Updates the current outline state and custom depth stencil if required
	void UpdateOutlineState(bool bInitializing = false);

	// Calls the UpdateCustomDepth with the required parameters
	void UpdateCustomDepth()
	{
		bool bEnableCustomDepth;
		uint8 CustomDepthStencil;
		GetRootComponentCustomDepth(bEnableCustomDepth, CustomDepthStencil);
		UpdateCustomDepth(bEnableCustomDepth, CustomDepthStencil);
	}

	// Updates the custom depth setup of the root component
	virtual void UpdateCustomDepth(bool bEnable, uint8 Stencil);
	// ~Outline

public:
	// ~Visibility
	// Whether the object should be hidden in the editor
	bool IsHiddenInEditor() const
	{
		return EditorParams.IsValid() && EditorParams->bIsHidden;
	}

	bool IsHiddenByEditorMode() const
	{
		return EditorParams.IsValid() && EditorParams->bIsHiddenByEditorMode;
	}

	bool IsHiddenByViewMode() const
	{
		return EditorParams.IsValid() && EditorParams->bIsHiddenByViewMode;
	}

	// Returns whether the object is currently hidden by the user
	bool IsHiddenByUser() const
	{
		return EditorParams.IsValid() && EditorParams->bIsHiddenByUser;
	}

	// Shows or hides the object
	void SetIsHiddenByUser(bool bNewHidden)
	{
		if (EditorParams.IsValid())
		{
			EditorParams->bIsHiddenByUser = bNewHidden;
			UpdateIsHiddenInEditor();
		}
	}

	// Get the components that should be hidden with the object
	void GetComponentsToIgnore(FCollisionQueryParams& Params) const;

protected:
	// Whether the object should currently be hidden
	bool ShouldBeHiddenInEditor() const
	{
		return IsHiddenByUser() || IsHiddenByEditorMode() || IsHiddenByViewMode();
	}

	void UpdateIsHiddenInEditor()
	{
		const auto bNewHidden = ShouldBeHiddenInEditor();
		if (EditorParams.IsValid() && bNewHidden != EditorParams->bIsHidden)
		{
			EditorParams->bIsHidden = bNewHidden;
			OnIsHiddenInEditorChanged();
			UpdateShowEditorVisualization();
			UpdateCustomDepth();
		}
	}

	// Called when the object was hidden or unhidden
	virtual void OnIsHiddenInEditorChanged()
	{
	}

	// Whether the object should be hidden for the given mode
	bool ShouldBeHiddenByEditorMode(UBrickEditorMode* Mode) const;

public:
	void UpdateIsHiddenByEditorMode(UBrickEditorMode* Mode)
	{
		const auto bNewHidden = ShouldBeHiddenByEditorMode(Mode);
		if (EditorParams.IsValid())
		{
			EditorParams->bIsHiddenByEditorMode = bNewHidden;
			UpdateIsHiddenInEditor();
		}
	}

protected:
	// Whether the object should be hidden in the given view mode
	virtual bool ShouldBeHiddenByViewMode(EBrickEditorViewMode ViewMode) const
	{
		return false;
	}

	void UpdateIsHiddenByViewMode(EBrickEditorViewMode ViewMode)
	{
		const auto bNewHidden = ShouldBeHiddenByViewMode(ViewMode);
		if (EditorParams.IsValid())
		{
			EditorParams->bIsHiddenByViewMode = bNewHidden;
			UpdateIsHiddenInEditor();
		}
	}

	// ~Visibility

public:
	// ~Visualization
	// Update whether debug info should be shown in the editor
	void UpdateShowEditorVisualization(bool bForceUpdate = false);

	// Can be implemented to show, hide or update the property visualization
	virtual void UpdateEditorVisualization()
	{
	}

	// ~Visualization

	// ~Moving
	// Lets the object know it is about to be moved
	void BeginEditorMove(const UBrickEditorObject* MoveRoot, const bool bIsMirrored, UBrickEditorObject* InMirrorObject);

	// Lets the object know that moving has finished
	void EndEditorMove(bool bReverted);

	// Whether the object is being moved with the move mode
	bool IsBeingMovedInEditor() const
	{
		return EditorParams.IsValid() && EditorParams->bIsBeingMoved;
	}

	// Whether this object is the root of the current move
	bool IsEditorMoveRoot() const
	{
		return EditorParams.IsValid() && EditorParams->bIsMoveRoot;
	}

	// Whether this object is being moved mirrored
	bool IsEditorMoveMirrored() const
	{
		return EditorParams.IsValid() && EditorParams->bIsMoveMirrored;
	}

	// Returns the object providing this one is being mirrored from
	auto* GetEditorMoveMirroredObject() const
	{
		return EditorParams.IsValid() ? EditorParams->MoveMirroredObject.Get() : nullptr;
	}

	// Sets the spawn location and rotation but does NOT update the transform
	void SetSpawnTransform(const FVector& NewLocation, const FRotator& NewRotation);

	// Moves the object to the spawn transform
	void ApplySpawnTransform();

	// Updates the current transform of the move root
	void SetMoveTransform(const FTransform& NewTransform);

	// Returns true if the property value is mirrored from the other object
	virtual bool IsBrickPropertyMirroredFrom(const UBrickEditorObject* OtherObject, const FBrickPropertyInstance& Property, const EBrickEditorMirrorMode MirrorMode) const;

	// Get the transform to apply when the brick is mirrored
	void GetMirroredSpawnTransform(FVector& OutLocation, FQuat& OutRotation, EBrickEditorMirrorMode MirrorMode) const
	{
		MirrorTransform(SpawnLocation, SpawnRotation.Quaternion(), OutLocation, OutRotation, MirrorMode);
	}

	// Mirror an arbitrary transform
	void MirrorTransform(const FVector& Location, const FQuat& Rotation, FVector& OutLocation, FQuat& OutRotation, const EBrickEditorMirrorMode MirrorMode) const;

protected:
	// Get the axes that need to be flipped or exchanged to mirror the object
	void GetMirrorAxes(int32& OutXAxis, int32& OutYAxis, int32& OutZAxis) const;

	virtual void OnEditorMoveCommitted()
	{
	}

	virtual void OnEditorMoveCancelled()
	{
	}

public:
	// Called when the object has been moved during the move mode
	virtual void OnEditorMoveUpdate(const FTransform& NewTransform)
	{
	}

	// Returns whether the object is at the given mirrored
	bool IsAtMirroredTransform(const FVector& InLocation, const FQuat& InRotation) const;
	// ~Moving

	// ~Serialization
	// NOTE: This should not be made virtual since it can be used on totally different classes during editor mirroring
	// Save this object's custom data to an archive
	void SaveBrickEditorObject(FArchive& Ar);

	// NOTE: This should not be made virtual since it can be used on totally different classes during editor mirroring
	// Load custom data  from an archive
	void LoadBrickEditorObject(FArchive& Ar, FBrickRigsSaveVersion Version);

	// Gives the object a chance to recycle data from a removed property or removed property value
	virtual bool ResolveDeprecatedBrickProperty(const FResolveBrickPropertyParams& Params)
	{
		return false;
	}

	// ~Serialization

	// ~IBrickPropertyInterface
	virtual void UpdateFocusedBrickProperty(const FBrickPropertyFocusEvent& Event) override;
	// ~IBrickPropertyInterface
};
