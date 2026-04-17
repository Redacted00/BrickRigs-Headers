// Copyright Fluppisoft, 2021

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "UI/Menu/MenuSequenceProperties.h"
#include "BrickAssetManager.generated.h"

class UMenuMusic;
class AMenuSequence;
class UBrickDecal;
class UBillboardImage;
class UGameModeInfo;
class UWeatherCondition;
class ULevelInfo;
class UBrickMaterial;
class UPropertyWidget;

// Offers a shorter and safer syntax for handling async loading
struct FSmartStreamableHandle
{
	enum class EState : uint8
	{
		InProgress,
		AlreadyLoaded,
		Finished
	};

	~FSmartStreamableHandle()
	{
		Cancel();
	}

	template <class T, typename FPointer>
	void Load(const FPointer& Asset, const TDelegate<void(T*, EState)>& Delegate)
	{
		// Check if we are already loading something
		if (Handle.IsValid())
		{
			// Check if the same asset is being requested again
			auto RequestedAssets = TArray<FSoftObjectPath>();
			Handle->GetRequestedAssets(RequestedAssets);
			if (RequestedAssets.Contains(Asset.ToSoftObjectPath()))
			{
				// Loading is still in progress, notify the caller and abort
				Delegate.Execute(nullptr, EState::InProgress);
				return;
			}

			// Cancel the previous load
			Handle->CancelHandle();
			Handle.Reset();
		}

		// If the asset is already valid or will never be (is null) call the callback right away
		if (Asset.IsValid() || Asset.IsNull())
		{
			Delegate.Execute(Asset.Get(), EState::AlreadyLoaded);
		}
		else
		{
			// Notify that the loading is starting
			Delegate.Execute(nullptr, EState::InProgress);

			// Start the async load
			Handle = UAssetManager::GetStreamableManager().RequestAsyncLoad(Asset.ToSoftObjectPath(), [this, Delegate]
			{
				if (Handle.IsValid())
				{
					auto* LoadedAsset = Cast<T>(Handle->GetLoadedAsset());
					Handle.Reset();
					// Notify the caller
					Delegate.Execute(LoadedAsset, EState::Finished);
				}
			});
		}
	}

	// Version that binds a weak object lambda delegate
	template <class T, typename FPointer>
	void Load(const FPointer& Asset, UObject* Object, const TFunction<void(T*, EState)>& Lambda)
	{
		Load<T>(Asset, TDelegate<void(T*, EState)>::CreateWeakLambda(Object, Lambda));
	}

	// Version that binds a member function delegate
	template <class T, class U, typename FPointer>
	void Load(const FPointer& Asset, U* Object, typename TMemFunPtrType<false, U, void(T*, EState)>::Type InFunc)
	{
		Load<T>(Asset, TDelegate<void(T*, EState)>::CreateUObject(Object, InFunc));
	}

	void Cancel()
	{
		if (Handle.IsValid())
		{
			Handle->CancelHandle();
			Handle.Reset();
		}
	}

	bool IsLoading() const
	{
		return Handle.IsValid();
	}

private:
	TSharedPtr<FStreamableHandle> Handle;
};

enum class ELoadAssetLibrariesMode : uint8
{
	Scan,
	Load,
	Unload
};

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickAssetManager : public UAssetManager
{
	GENERATED_BODY()

	struct FPropertyWidgetClassParams
	{
		TSoftClassPtr<UPropertyWidget> WidgetClass;
		// The property types supported by the widget class
		TArray<FName> SupportedTypes;
		// Whether the widget class can also be displayed for read only properties
		bool bSupportsReadOnly;
	};

	// ~Variables
	// Current load mode
	ELoadAssetLibrariesMode LoadMode;

	UPROPERTY(Transient)
	TArray<UClass*> ModHookClasses;
	UPROPERTY(Transient)
	TArray<UClass*> BrickStaticInfoClasses;
	UPROPERTY(Transient)
	TArray<UClass*> BrickFilterClasses;
	UPROPERTY(Transient)
	TArray<UBrickMaterial*> BrickMaterials;
	UPROPERTY(Transient)
	TArray<UClass*> BrickPatternClasses;
	UPROPERTY(Transient)
	TArray<UBrickDecal*> BrickDecals;
	UPROPERTY(Transient)
	TArray<UClass*> SirenSequenceClasses;
	UPROPERTY(Transient)
	TArray<UClass*> SirenTypeClasses;
	UPROPERTY(Transient)
	TArray<UClass*> ExplosiveMaterialClasses;
	UPROPERTY(Transient)
	TArray<UClass*> InventoryItemClasses;
	UPROPERTY(Transient)
	TArray<UClass*> UIStyleClasses;
	UPROPERTY(Transient)
	TArray<UWeatherCondition*> WeatherConditions;
	UPROPERTY(Transient)
	TArray<ULevelInfo*> LevelInfos;
	UPROPERTY(Transient)
	TArray<UGameModeInfo*> GameModeInfos;
	UPROPERTY(Transient)
	TArray<UBillboardImage*> BillboardImages;
	UPROPERTY(Transient)
	TArray<UMenuMusic*> MenuMusicAssets;
	TMap<TSoftClassPtr<AMenuSequence>, FMenuSequenceProperties> MenuSequences;
	// Generated array of the property widgets
	TArray<FPropertyWidgetClassParams> PropertyWidgetClasses;
	// NOTE: This only exists to force load the widget on startup, since it's needed to add sub properties
	TSubclassOf<UPropertyWidget> StructPropertyWidgetClass;
	// Property widget to use for property types that don't have a specific widget class
	TSoftClassPtr<UPropertyWidget> GenericPropertyWidgetClass;
	// ~Variables

public:
	// ~Super Interface
	virtual void PostInitialAssetScan() override;
#if WITH_EDITOR
	virtual void RefreshPrimaryAssetDirectory(bool bForceRefresh) override;
#endif
	// ~Super Interface

	// Returns the custom asset manager instance if valid
	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Asset Manager"))
	static UBrickAssetManager* Get();

	// Converts a class to a primary asset type
	static FPrimaryAssetType CreatePrimaryAssetType(const UClass* Class)
	{
		return FPrimaryAssetType(Class->GetFName());
	}

	// Templated version
	template <typename T>
	static FPrimaryAssetType CreatePrimaryAssetType()
	{
		return CreatePrimaryAssetType(T::StaticClass());
	}

	// Generates the primary asset id for an asset
	template <typename T>
	static FPrimaryAssetId CreatePrimaryAssetId(const T* Asset)
	{
		// IMPORTANT: Use the static class of the asset type, otherwise child classes would have a different asset type
		return FPrimaryAssetId(CreatePrimaryAssetType<T>(), *FPackageName::GetShortName(Asset->GetOutermost()));
	}

	// Combined function to scan for assets, load and unload them
	void LoadAssetLibraries(ELoadAssetLibrariesMode Mode);

	const auto& GetModHooks() const
	{
		return ModHookClasses;
	}

	const auto& GetBricks() const
	{
		return BrickStaticInfoClasses;
	}

	const auto& GetBrickFilters() const
	{
		return BrickFilterClasses;
	}

	const auto& GetBrickMaterials() const
	{
		return BrickMaterials;
	}

	const auto& GetBrickPatterns() const
	{
		return BrickPatternClasses;
	}

	const auto& GetBrickDecals() const
	{
		return BrickDecals;
	}

	const auto& GetSirenSequences() const
	{
		return SirenSequenceClasses;
	}

	const auto& GetSirenTypes() const
	{
		return SirenTypeClasses;
	}

	const auto& GetExplosives() const
	{
		return ExplosiveMaterialClasses;
	}

	const auto& GetInventoryItems() const
	{
		return InventoryItemClasses;
	}

	const auto& GetUIStyles() const
	{
		return UIStyleClasses;
	}

	const auto& GetBillboardImages() const
	{
		return BillboardImages;
	}

	const auto& GetWeatherConditions() const
	{
		return WeatherConditions;
	}

	const auto& GetLevelInfos() const
	{
		return LevelInfos;
	}

	const auto& GetMenuSequences() const
	{
		return MenuSequences;
	}

	const auto& GetGameModeInfos() const
	{
		return GameModeInfos;
	}

	const auto& GetMenuMusicAssets() const
	{
		return MenuMusicAssets;
	}

	// Returns the UI style class that should be used by default
	UClass* GetDefaultUIStyle() const;

	// Tries to find info for the current level
	ULevelInfo* GetCurrentLevelInfo(const UObject* WorldContextObject) const;
	// Tries to find the associated level info by name
	ULevelInfo* FindLevelInfo(const FString& InLevelName) const;
	// Version that tries to find the level info given the level asset path
	ULevelInfo* FindLevelInfo(const FSoftObjectPath& InLevel) const;
	// Version that takes the short level asset name
	ULevelInfo* FindLevelInfoByLevelName(const FString& InLevelName) const;

	// Find a game mode by name
	UGameModeInfo* FindGameModeInfo(const FString& InGameModeName) const;
	// Versions that finds the info associated to a game mode class
	UGameModeInfo* FindGameModeInfo(const UClass* InGameModeClass) const;

	// Return the property widget class that should be displayed for the given property
	TSoftClassPtr<UPropertyWidget> GetWidgetForBrickProperty(const TArray<FName>& InPropertyTypeHierarchy, bool bIsReadOnly) const;

protected:
	// Adds search roots for the game and mods, returns true if they changed
	bool UpdateAssetSearchRoots();
	// Converts the path that should be used on when scanning into a virtual path
	FString GetScanPath(const FString& Path) const;
	// Scans the given relative path for primary assets, the path can either be absolute or relative to the game and mod directories
	void ScanPath(const FString& Path, UClass* BaseClass, bool bIsBlueprintClass);

	// Load asset ids from the given path, if the path doesn't start with / it is appended to the game and all plugin content paths
	void LoadAssetIdsFromPath(TArray<FPrimaryAssetId>& OutAssetIds, const FString& Path, UClass* BaseClass, bool bIsBlueprintClass, bool bIsWidgetBlueprintClass = false);
	// Loads assets as soft pointers from the given path
	template <class T>
	void LoadSoftAssetsFromPath(TArray<TSoftObjectPtr<T>>& OutAssets, const FString& Path);
	// Synchronously load all assets from the given path into an array
	template <class T>
	void LoadAssetsFromPath(TArray<T*>& OutAssets, const FString& Path)
	{
		LoadAssetsFromPathInternal(OutAssets, Path, T::StaticClass(), false);
	}

	// Synchronously load all blueprints from the given path into an array
	template <class T>
	void LoadBlueprintsFromPath(TArray<UClass*>& OutClasses, const FString& Path)
	{
		LoadAssetsFromPathInternal(OutClasses, Path, T::StaticClass(), true);
	}

private:
	// Internal version used to load assets or blueprints
	template <class T>
	void LoadAssetsFromPathInternal(TArray<T*>& OutAssets, const FString& Path, UClass* BaseClass, bool bIsBlueprintClass);

	// Builds the game mode specific sub levels
	void BuildGameModeSpecificLevels();
	// Used to build the included objects for the editor filter classes
	void BuildBrickEditorObjects(TArray<UClass*>& StaticInfoClasses, TArray<UClass*>& FilterClasses);
};
