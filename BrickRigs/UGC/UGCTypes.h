#pragma once

#include "BrickEditor/BrickEditorSaveVersion.h"
#include "UI/Misc/PagedListEntry.h"
#include "Misc/Paths.h"
#include "GameFramework/OnlineReplStructs.h"
#include "Properties/BrickPropertyInterface.h"
#include "CoreMinimal.h"
#include "FluUGCTypes.h"
#include "UGCTypes.generated.h"

struct FVehicleSpawnProperties;

UENUM()
enum class EUGCType : uint8
{
	None,
	Vehicle,
	Scenario,
	Mod,
	Max
};

UENUM()
enum class EUGCFileType : uint8
{
	None,
	// File stored locally on disk
	Saved,
	// File stored in the development saved directory
	SavedDevelopment,
	// File stored in the development saved directory
	SavedLegacy,
	// File stored within the content folder of the project or a plugin/mod
	Content,
	// An online workshop item
	Online,
	// A custom item stored in memory,
	Inline,
	Max
};

UENUM()
enum class EUGCContentVersion : uint8
{
	Default,
	AutoSave,
	Backup,
	Max
};

UENUM()
enum class EUGCQueryType : uint8
{
	Local,
	Online,
	Published,
	Subscribed,
	Favorited,
	RecentlyUsed,
	Selection,
	Max
};

UENUM()
enum class EUGCSortMethod : uint8
{
	TitleAsc,
	CreationOrderAsc,
	CreationOrderDesc,
	LastUpdatedDesc,
	SubscriptionDateDesc,
	Trending,
	PlaytimeTrending,
	TotalPlaytime,
	TotalUniqueSubscriptions,
	SearchRelevancy,
	Max
};

UENUM()
enum class EUGCTypeTag : uint8
{
	None,
	Car,
	RaceCar,
	Truck,
	HeavyMachinery,
	Plane,
	Helicopter,
	Tank,
	Train,
	Trailer,
	Prop,
	Building,
	Ship,
	SpaceCraft,
	Bus,
	Motorcycle,
	Max
};

UENUM()
enum class EUGCEraTag : uint8
{
	None,
	_1800s,
	_1900s,
	_2000s,
	Futuristic,
	Ancient,
	WW2,
	WW1,
	Max
};

UENUM()
enum class EUGCDepartmentTag : uint8
{
	None,
	Civil,
	Military,
	Police,
	FireDepartment,
	Max
};

// Used to manage UGC tags in a convenient container
USTRUCT()
struct FUGCTags
{
	GENERATED_BODY()

	// ~Properties
	UPROPERTY()
	EUGCTypeTag Type = EUGCTypeTag::None;
	UPROPERTY()
	EUGCEraTag Era = EUGCEraTag::None;
	UPROPERTY()
	EUGCDepartmentTag Department = EUGCDepartmentTag::None;
	// NOTE: If tags are added they have to be added to the functions below as well!
	// ~Properties

	bool operator==(const FUGCTags& Other) const
	{
		return Type == Other.Type && Era == Other.Era && Department == Other.Department;
	}

	bool HasAnyTag() const
	{
		return Type != EUGCTypeTag::None || Era != EUGCEraTag::None || Department != EUGCDepartmentTag::None;
	}

	void ExportTags(TArray<FString>& OutTags) const
	{
		// Helper lambda used to add tags
		auto AddTag = [&](const UEnum* Enum, auto EnumValue, auto NoneValue)
		{
			if (EnumValue != NoneValue)
			{
				auto TagString = Enum->GetNameStringByIndex(static_cast<int32>(EnumValue));
				// Remove the prefix for numbers
				TagString.RemoveFromStart(TEXT("_"));
				OutTags.Add(TagString);
			}
		};

		AddTag(StaticEnum<EUGCTypeTag>(), Type, EUGCTypeTag::None);
		AddTag(StaticEnum<EUGCEraTag>(), Era, EUGCEraTag::None);
		AddTag(StaticEnum<EUGCDepartmentTag>(), Department, EUGCDepartmentTag::None);
	}

	void ImportTags(const TArray<FString>& InTags)
	{
		// Remove spaces from the tags and add an underscore before numbers
		auto ConvertedTags = InTags;
		for (auto& Tag : ConvertedTags)
		{
			if (Tag.Len() && FChar::IsDigit(Tag[0]))
			{
				Tag.InsertAt(0, "_");
			}
		}

		// Helper lambda used to find the tag for each category
		auto FindTag = [&]<typename T>(const UEnum* Enum, T& OutValue, T Fallback)
		{
			OutValue = Fallback;

			for (const auto& Tag : ConvertedTags)
			{
				// Add an underscore before digits
				auto TagString = Tag;
				if (TagString.Len() && FChar::IsDigit(TagString[0]))
				{
					TagString.InsertAt(0, "_");
				}

				const auto EnumValue = Enum->GetValueByNameString(*TagString);
				if (EnumValue != INDEX_NONE)
				{
					OutValue = static_cast<T>(EnumValue);
					break;
				}
			}
		};

		FindTag(StaticEnum<EUGCTypeTag>(), Type, EUGCTypeTag::None);
		FindTag(StaticEnum<EUGCEraTag>(), Era, EUGCEraTag::None);
		FindTag(StaticEnum<EUGCDepartmentTag>(), Department, EUGCDepartmentTag::None);
	}

	// Outputs a localized list of tags
	FText AsText() const
	{
		auto OutText = FText::GetEmpty();

		// Helper lambda used to add a tag
		auto AddTag = [&](auto EnumValue, auto NoneValue, auto DisplayNameGetter)
		{
			if (EnumValue != NoneValue)
			{
				const auto ValueText = DisplayNameGetter(EnumValue);
				if (OutText.IsEmpty())
				{
					OutText = ValueText;
				}
				else
				{
					OutText = FText::Format(INVTEXT("{0}, {1}"), OutText, ValueText);
				}
			}
		};

		AddTag(Type, EUGCTypeTag::None, GetTypeDisplayName);
		AddTag(Era, EUGCEraTag::None, GetEraDisplayName);
		AddTag(Department, EUGCDepartmentTag::None, GetDepartmentDisplayName);

		return OutText;
	}

	static FText GetTypeDisplayName(EUGCTypeTag Type);
	static FText GetEraDisplayName(EUGCEraTag Era);
	static FText GetDepartmentDisplayName(EUGCDepartmentTag Department);
};

// Reference to a specific UGC item
USTRUCT(BlueprintType)
struct FUGCFileInfo : public FPagedListEntry
{
	GENERATED_BODY()

	constexpr static auto MaxTitleLength = 100;

protected:
	// ~Variables
	// The type of file, i.e. local, online etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUGCFileType FileType;
	// The type of UGC item represented
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUGCType UGCType;
	// Version the file has been saved with
	UPROPERTY()
	FBrickRigsSaveVersion Version;
	// The version of content represented, i.e. auto save, a backup etc.
	UPROPERTY()
	EUGCContentVersion ContentVersion;
	// The directory name of the local item
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocalItemId;
	// Optional online item ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FFluUGCItemIdWrapper OnlineItemId;
	// Raw data of the item
	TArray<uint8> ItemData;
	// Saved thumbnail URL for online items
	FString ThumbnailURL;

public:
	// Display title of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FString Title;
	// Display description
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FString Description;
	// Number of objects/bricks used
	UPROPERTY(Transient)
	uint16 NumObjects;
	// Bounds dimensions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FVector Dimensions;
	// Total mass
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	float Mass;
	// Price of the item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	float Price;
	// Date and time the item has been created
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FDateTime TimeCreated;
	// Last time the item has been updated
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FDateTime TimeUpdated;
	// UGC tags
	UPROPERTY(Transient)
	FUGCTags Tags;

	// Steam id of the creator
	// NOTE: We store this as well as the item id, in case the item gets deleted for example (or for local files in multiplayer)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FUniqueNetIdRepl AuthorId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	int32 VotesUp = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	int32 VotesDown = 0;
	// Playtime stats for last week
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FTimespan TimePlayedWeek;
	// Playtime stats for all time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	FTimespan TimePlayedTotal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
	EFluUGCVisibility Visibility;
	// ~Variables

	// ~Constructor
	// IMPORTANT: Setting Mass to -1 means it hasn't been initialized yet, which prevents it from being displayed in the UGC browser as 0
	FUGCFileInfo(EUGCFileType InFileType = EUGCFileType::None, EUGCType InUGCType = EUGCType::None, const FBrickRigsSaveVersion& InVersion = BR_SAVE_VERSION, const FString& InLocalItemId = FString(), const TSharedPtr<FFluUGCItemId>& InOnlineItemId = nullptr, const TArray<uint8>& InItemData = {})
		: FileType(InFileType)
		  , UGCType(InUGCType)
		  , Version(InVersion)
		  , ContentVersion(EUGCContentVersion::Default)
		  , LocalItemId(InLocalItemId)
		  , OnlineItemId(InOnlineItemId)
		  , ItemData(InItemData)
		  , NumObjects(0)
		  , Dimensions(FVector::ZeroVector)
		  , Mass(-1.f)
		  , Price(-1.f)
		  , TimeCreated(FDateTime::UtcNow())
		  , TimeUpdated(FDateTime::UtcNow())
		  , AuthorId(nullptr)
		  , TimePlayedWeek(0)
		  , TimePlayedTotal(0)
		  , Visibility(EFluUGCVisibility::Unlisted)
	{
	}

	// ~Destructor
	virtual ~FUGCFileInfo() override = default;

	// ~Super Interface
	virtual UStruct* GetStruct() const override
	{
		return StaticStruct();
	}

	virtual FText GetDisplayName() const override;

	virtual FVector GetDimensions() const override
	{
		return Dimensions;
	}

	virtual float GetMass() const override
	{
		return Mass;
	}

	virtual float GetPrice(const ABrickPlayerController* PC) const override;

	virtual int32 GetNumObjects() const override
	{
		return NumObjects;
	}

	virtual FVector GetMaxDimensions(const ABrickPlayerController* PC) const override;
	virtual float GetMaxMass(const ABrickPlayerController* PC) const override;
	virtual int32 GetMaxNumObjects(const ABrickPlayerController* PC) const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Tries to convert a directoy name to an item ID, also outputs the name of the item folder
	static uint32 GetItemIdFromDirectoy(const FString& InDirectory, FString& OutFilename)
	{
		// Split the path, since we are only interested in the file name, not the path or extension for legacy files
		FString Path, Extension;
		FPaths::Split(InDirectory, Path, OutFilename, Extension);
		return FCString::Atoi(*OutFilename);
	}

	// ~Factories
	// Creates a new local file info
	static FUGCFileInfo MakeLocalFile(EUGCType InUGCType, const FString& InDirectory, EUGCFileType InFileType);

	// Creates an online file info
	static FUGCFileInfo MakeOnlineFile(EUGCType InUGCType, const TSharedRef<FFluUGCItemId>& InItemId, const FFluUGCQueryResult& QueryResult);
	// ~Factories

	bool IsValid() const
	{
		return FileType != EUGCFileType::None;
	}

	// ~File Type
	EUGCFileType GetExactFileType() const
	{
		return FileType;
	}

	bool IsExactFileType(EUGCFileType InFileType) const
	{
		return FileType == InFileType;
	}

	bool IsLocalFile() const
	{
		return FileType == EUGCFileType::Saved || FileType == EUGCFileType::SavedDevelopment || FileType == EUGCFileType::SavedLegacy || FileType == EUGCFileType::Content;
	}

	bool IsOnlineFile() const
	{
		return FileType == EUGCFileType::Online;
	}

	bool IsInlineFile() const
	{
		return FileType == EUGCFileType::Inline;
	}

	bool IsContentFile() const
	{
		return FileType == EUGCFileType::Content;
	}

	// Whether changes are allowed to be made to this file
	bool IsWritableFile() const
	{
		return !LocalItemId.IsEmpty();
	}

private:
	void SetFileType(EUGCFileType InFileType)
	{
		FileType = InFileType;
	}

	// ~File Type

public:
	// ~UGC Type
	EUGCType GetUGCType() const
	{
		return UGCType;
	}

	void SetUGCType(EUGCType InUGCType)
	{
		UGCType = InUGCType;
	}

	// ~UGC Type

	// ~Content Version
	EUGCContentVersion GetContentVersion() const
	{
		return ContentVersion;
	}

	void SetContentVersion(EUGCContentVersion NewVersion)
	{
		ContentVersion = NewVersion;
	}

	// Whether an auto save younger than the content file exist
	bool HasNewerAutoSave() const;

	// Whether a backup file exists
	bool HasBackupFile() const;

	// Whether the file has been saved as an auto save
	bool IsDedicatedAutoSaveFile() const;
	// ~Content Version

	// ~Conversion
	// Converts the file to a local file
	void ConvertToLocalFile();

	// Converts the file to a new local file
	void ConvertToNewLocalFile();

	// Converts a local file to an auto save version
	void ConvertToAutoSaveFile();

	// Converts a local file to a dedicated auto save file
	void ConvertToDedicatedAutoSaveFile();

	// Converts a local file to a backup version
	void ConvertToBackupFile();

	// Converts a local file to an inline file
	void ConvertToInlineFile();

	// Version that takes the already loaded data
	void ConvertToInlineFile(TArray<uint8>& InData);
	// ~Conversion

	// ~File
	// Whether this file points to the same item as the other one
	bool IsSameFile(const FUGCFileInfo& Other) const
	{
		return FileType == Other.FileType && UGCType == Other.UGCType && ContentVersion == Other.ContentVersion && LocalItemId == Other.LocalItemId && FFluUGCItemIdWrapper::IsSameId(OnlineItemId, Other.OnlineItemId) && ItemData == Other.ItemData;
	}

	const auto& GetVersion() const
	{
		return Version;
	}

	void SetVersion(const FBrickRigsSaveVersion& NewVersion)
	{
		Version = NewVersion;
	}

	auto IsLegacyFile() const
	{
		return Version <= BR_SAVE_LAST_LEGACY_VERSION;
	}

	FString GetLocalItemId() const
	{
		return LocalItemId;
	}

	void SetLocalItemId(const FString& InItemId)
	{
		LocalItemId = InItemId;
	}

	const FFluUGCItemIdWrapper& GetOnlineItemId() const
	{
		return OnlineItemId;
	}

	void SetOnlineItemId(const TSharedPtr<FFluUGCItemId>& InItemId)
	{
		OnlineItemId = InItemId;
	}

	// Gets the inline item data
	void GetUncompressedItemData(TArray<uint8>& OutData) const;

	// Whether the file has valid item data
	bool HasInlineData() const
	{
		return ItemData.Num() > 0;
	}

	// Get the local path or URL of the thumbnail
	FString GetThumbnailPath(bool& bOutIsURL) const;

	// Returns the name of the item folder
	FString GetLocalFilename() const
	{
		auto Filename = FString();
		SplitContentItemPath(nullptr, &Filename);
		return Filename;
	}

	// This should be called on files received from remote players to clear the local file reference
	void SanitizeRemoteItemId()
	{
		if (!IsContentFile())
		{
			SetLocalItemId(FString());
		}
	}

	// Returns the name of the plugin this file is stored in
	FString GetPluginName() const
	{
		auto PluginName = FString();
		SplitContentItemPath(&PluginName, nullptr);
		return PluginName;
	}

private:
	// Splits the path of a content item into the plugin name and filename
	void SplitContentItemPath(FString* OutPluginName, FString* OutFilename) const
	{
		if (!LocalItemId.Split("/", OutPluginName, OutFilename) && OutFilename)
		{
			// The local item id is the filename if it doesn't contain a slash
			*OutFilename = LocalItemId;
		}
	}

public:
	// ~File
	// ~Meta Data
	FText GetDescriptionText() const
	{
		return Description.IsEmpty() ? NSLOCTEXT("Misc", "NoDescription", "No description") : FText::AsCultureInvariant(Description);
	}

	// Copies the editable meta data only from the other file
	void CopyMetaData(const FUGCFileInfo& InFileInfo)
	{
		Title = InFileInfo.Title;
		Description = InFileInfo.Description;
		Visibility = InFileInfo.Visibility;
		Tags = InFileInfo.Tags;
	}

	// Saves the meta data to disk
	bool SaveMetaData(bool bUpdatedContentFile);

	// Serializes the meta data only
	bool SerializeMetaData(FArchive& Ar);

	// Returns vehicle spawn related properties
	FVehicleSpawnProperties GetVehicleSpawnProperties() const;
	// ~Meta Data

	// ~Serialization
	// Used for sending vehicle info over the network
	bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess);

	void ExportOnlineKeyValueTags(TMap<FString, FString>& OutKeyValueTags) const
	{
		// Add the UGC type tag
		const auto* UGCTypeEnum = StaticEnum<EUGCType>();
		OutKeyValueTags.Add("Type", UGCTypeEnum->GetNameStringByIndex(static_cast<int32>(UGCType)));
	}

	void ImportOnlineKeyValueTags(const TMap<FString, FString>& InKeyValueTags)
	{
		// Try to get the type value
		const auto* UGCTypeEnum = StaticEnum<EUGCType>();
		const auto* TypeString = InKeyValueTags.Find("Type");
		const auto TypeValue = TypeString ? UGCTypeEnum->GetValueByNameString(*TypeString) : NAME_None;
		const auto NewType = TypeValue != INDEX_NONE ? static_cast<EUGCType>(TypeValue) : EUGCType::None;

		// NOTE: Don't change the default type of the key value tag isn't set, because none items can't be loaded
		if (NewType != EUGCType::None)
		{
			UGCType = NewType;
		}
	}

	void ExportOnlineTags(TArray<FString>& OutTags) const
	{
		Tags.ExportTags(OutTags);
	}

	void ImportOnlineTags(const TArray<FString>& InTags)
	{
		Tags.ImportTags(InTags);
	}

	FString ExportOnlineMetaData() const
	{
		FString Result;
		constexpr auto Delimiter = static_cast<TCHAR>(';');
		Result += Version.ToString() + Delimiter;
		Result += FString::FromInt(NumObjects) + Delimiter;
		Result += Dimensions.ToString() + Delimiter;
		Result += FString::SanitizeFloat(Mass) + Delimiter;
		Result += FString::SanitizeFloat(Price);
		return Result;
	}

	void ImportOnlineMetaData(const FString& InMetaData)
	{
		TArray<FString> Array;
		InMetaData.ParseIntoArray(Array, TEXT(";"));

		if (Array.IsValidIndex(0))
		{
			Version.FromString(Array[0]);
		}
		if (Array.IsValidIndex(1))
		{
			NumObjects = FCString::Atoi(*Array[1]);
		}
		if (Array.IsValidIndex(2))
		{
			Dimensions.InitFromString(Array[2]);
		}
		if (Array.IsValidIndex(3))
		{
			Mass = FCString::Atof(*Array[3]);
		}
		if (Array.IsValidIndex(4))
		{
			Price = FCString::Atof(*Array[4]);
		}
	}

	// ~Serialization

private:
	// Property Callbacks
	static void GetUGCVisibilityItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
	static void GetDimensionsValueRange(const void*, FVector& OutMin, FVector& OutMax);
};

template <>
struct TStructOpsTypeTraits<FUGCFileInfo> : TStructOpsTypeTraitsBase2<FUGCFileInfo>
{
	enum
	{
		WithNetSerializer = true,
		WithNetSharedSerialization = true
	};
};

struct FUGCQuerySettings
{
	EUGCQueryType QueryType;
	EUGCType UGCType;
	EUGCSortMethod SortMethod;
	// Optional list of online items to query details for
	TArray<TSharedRef<FFluUGCItemId>> ItemIds;
	TSharedPtr<const FUniqueNetId> UserId;
	FString SearchString;
	FUGCTags RequiredTags;
	bool bIncludeOnlyWritable;
	bool bIncludeDevelopmentItems;
	bool bIncludeLegacyItems;
	int32 Page;
	int32 NumItemsPerPage;
};

struct FUGCQueryResult
{
	FUGCFileInfo FileInfo;

	// ~Constructor
	FUGCQueryResult(const FUGCFileInfo& InFileInfo)
		: FileInfo(InFileInfo)
	{
	}

	// Functions used to sort local query results
	using FSortFunction = int32(FUGCQueryResult::*)(const FUGCQueryResult&) const;

	bool Sort(const FUGCQueryResult& Other, const FSortFunction& SortFunction) const
	{
		// Sort by file type
		if (FileInfo.GetExactFileType() != Other.FileInfo.GetExactFileType())
		{
			return FileInfo.GetExactFileType() > Other.FileInfo.GetExactFileType();
		}

		// Show the auto save file first
		if (FileInfo.IsDedicatedAutoSaveFile() != Other.FileInfo.IsDedicatedAutoSaveFile())
		{
			return FileInfo.IsDedicatedAutoSaveFile();
		}

		const auto SortResult = (this->*SortFunction)(Other);
		if (SortResult != -1)
		{
			return SortResult > 0;
		}

		// As a last resort, sort by ID
		return FileInfo.GetLocalItemId() < Other.FileInfo.GetLocalItemId();
	}

	// Functions used to sort local query results
	int32 SortByTitleAsc(const FUGCQueryResult& Other) const
	{
		if (FileInfo.Title == Other.FileInfo.Title)
		{
			return -1;
		}
		// NOTE: Sort untitled files after everything else
		if (FileInfo.Title.IsEmpty())
		{
			return 0;
		}
		if (Other.FileInfo.Title.IsEmpty())
		{
			return 1;
		}
		return FileInfo.Title < Other.FileInfo.Title ? 1 : 0;
	}

	int32 SortByCreationOrderAsc(const FUGCQueryResult& Other) const
	{
		return SortHelper<FDateTime, &FUGCFileInfo::TimeCreated>(Other, true);
	}

	int32 SortByCreationOrderDesc(const FUGCQueryResult& Other) const
	{
		return SortHelper<FDateTime, &FUGCFileInfo::TimeCreated>(Other, false);
	}

	int32 SortByLastUpdatedDesc(const FUGCQueryResult& Other) const
	{
		return SortHelper<FDateTime, &FUGCFileInfo::TimeUpdated>(Other, false);
	}

private:
	template <typename T, T FUGCFileInfo::*Property>
	int32 SortHelper(const FUGCQueryResult& Other, bool bAscening) const
	{
		const auto& A = FileInfo.*Property;
		const auto& B = Other.FileInfo.*Property;

		if (A == B)
		{
			return -1;
		}
		return A < B ? bAscening : !bAscening;
	}
};

struct FUGCPaths
{
	inline static const auto NoPluginName = FString();

	// Helper function to get the content directory of a plugin or the project
	static FString GetPluginOrProjectContentDir(const FString& PluginName);

	static FString GetUGCBaseDir(EUGCFileType FileType, const FString& PluginName)
	{
		switch (FileType)
		{
		case EUGCFileType::SavedDevelopment:
			return FPaths::Combine(FPaths::ProjectDir(), TEXT("SavedRemastered/"));
		case EUGCFileType::SavedLegacy:
			return FPaths::Combine(FPlatformProcess::UserSettingsDir(), TEXT("BrickRigs/Saved/"));
		case EUGCFileType::Content:
			return GetPluginOrProjectContentDir(PluginName);
		default:
			return FPaths::ProjectSavedDir();
		}
	}

	static FString GetUGCTypeDirName(EUGCType Type)
	{
		switch (Type)
		{
		case EUGCType::Vehicle:
			return TEXT("Vehicles");
		case EUGCType::Scenario:
			return TEXT("Scenarios");
		default:
			ensure(false);
			return "";
		}
	}

	static FString GetUGCTypeDir(EUGCType Type, EUGCFileType FileType, const FString& PluginName)
	{
		// Example: Saved/Vehicles/
		return FPaths::Combine(GetUGCBaseDir(FileType, PluginName), GetUGCTypeDirName(Type));
	}

	static FString GetItemDir(const FUGCFileInfo& FileInfo)
	{
		// Example: Saved/Vehicles/MyTruck/
		return FPaths::Combine(GetUGCTypeDir(FileInfo.GetUGCType(), FileInfo.GetExactFileType(), FileInfo.GetPluginName()), FileInfo.GetLocalFilename());
	}

	static FString GetContentFilename(EUGCType UGCType, EUGCContentVersion ContentVersion)
	{
		FString OutFilename;
		FString OutExtension;
		switch (UGCType)
		{
		case EUGCType::Vehicle:
			OutFilename = "Vehicle";
			OutExtension = ".brv";
			break;
		case EUGCType::Scenario:
			OutFilename = "Scenario";
			OutExtension = ".brs";
			break;
		default:
			ensure(false);
		}

		switch (ContentVersion)
		{
		case EUGCContentVersion::AutoSave:
			OutFilename = "Autosave";
			break;
		case EUGCContentVersion::Backup:
			OutFilename = "Backup";
			break;
		default:
			break;
		}

		return OutFilename + OutExtension;
	}

	static FString GetContentPath(const FUGCFileInfo& FileInfo)
	{
		return GetContentPath(FileInfo, FileInfo.GetContentVersion());
	}

	// Version that allows overriding the auto save flag
	static FString GetContentPath(const FUGCFileInfo& FileInfo, EUGCContentVersion ContentVersion)
	{
		// Example: Saved/Vehicles/12345/Vehicle.brv
		return FPaths::Combine(GetItemDir(FileInfo), GetContentFilename(FileInfo.GetUGCType(), ContentVersion));
	}

	static FString GetTempUploadDir()
	{
		// Example Saved/Temp/
		return FPaths::Combine(FPaths::ProjectSavedDir(), TEXT("Temp"));
	}

	static FString GetContentUploadDir(const FUGCFileInfo& FileInfo)
	{
		// Example: Saved/Temp/Vehicles/12345/
		return FPaths::Combine(GetTempUploadDir(), GetUGCTypeDirName(FileInfo.GetUGCType()), FileInfo.GetLocalFilename());
	}

	static FString GetContentUploadPath(const FUGCFileInfo& FileInfo)
	{
		// Example: Saved/Temp/Vehicles/12345/Vehicle.brv
		return FPaths::Combine(GetContentUploadDir(FileInfo), GetContentFilename(FileInfo.GetUGCType(), EUGCContentVersion::Default));
	}

	static FString GetThumbnailPath(const FUGCFileInfo& FileInfo)
	{
		check(FileInfo.IsWritableFile());

		if (FileInfo.GetExactFileType() == EUGCFileType::SavedLegacy)
		{
			return FPaths::Combine(FPaths::Combine(GetUGCBaseDir(EUGCFileType::SavedLegacy, FileInfo.GetPluginName()), TEXT("Thumbnails")), FileInfo.GetLocalFilename() + TEXT(".png"));
		}
		return FPaths::Combine(GetItemDir(FileInfo), TEXT("Preview.png"));
	}

	static FString GetMetaDataPath(const FUGCFileInfo& FileInfo)
	{
		// Example: Saved/Temp/Vehicles/MyTruck/MetaData.brm
		return FPaths::Combine(GetItemDir(FileInfo), TEXT("MetaData.brm"));
	}
};
