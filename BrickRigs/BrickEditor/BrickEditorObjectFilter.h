// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Misc/BrickUnits.h"
#include "GameplayTags.h"
#include "UObject/Object.h"
#include "BrickEditorObjectFilter.generated.h"

class UBrickEditorObject;
class UBrickEditorObjectStaticInfo;

USTRUCT(BlueprintType)
struct FBrickEditorObjectFilterParams
{
	GENERATED_BODY()

	// ~Variables
	// List of objects included in this filter
	UPROPERTY(Transient)
	TArray<UClass*> IncludedStaticInfoClasses;
	// ~Variables

	// ~Properties
	// Optional custom display name for the filter
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter)
	FText DisplayName;
	// Explicit class to use for the thumbnail
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter, meta = (MetaClass = "BrickEditorObjectStaticInfo"))
	FSoftClassPath ThumbnailOverrideClass;
	// Tags to filter the objects with
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter)
	FGameplayTagContainer FilterTags;
	// Brick size to filters, used as OR
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter)
	TArray<FBrickSize> BrickSizes;
	// ~Properties

	// Whether the given class is included in this filter, only to be called via the filter ref
	bool IsStaticInfoClassIncludedInternal(UClass* InStaticInfoClass) const;
	// Get the desired display name for the filter
	FText GetDisplayName() const
	{
		// Use the custom name if it is set
		if (!DisplayName.IsEmpty())
		{
			return DisplayName;
		}

		// Get all size tags
		FText OutDisplayName;
		for (auto i = 0; i < BrickSizes.Num(); ++i)
		{
			const auto SizeText = BrickSizes[i].ToText(true);
			if (i == 0)
			{
				OutDisplayName = SizeText;
			}
			else
			{
				OutDisplayName = FText::Format(FText::AsCultureInvariant("{0}, {1}"), OutDisplayName, SizeText);
			}
		}

		return OutDisplayName;
	}
};

// Used to uniquely identify a filter or sub filter
struct FBrickEditorObjectFilterRef
{
private:
	// ~Variables
	TWeakObjectPtr<UClass> FilterClass;
	int32 SubFilterIndex;
	// ~Variables

public:
	// ~Constructor
	FBrickEditorObjectFilterRef(UClass* InClass = nullptr, int32 InSubFilterIndex = INDEX_NONE)
		: FilterClass(InClass), SubFilterIndex(InSubFilterIndex)
	{
	}

	// Whether this filter is valid and can be used
	bool IsValid() const
	{
		return FilterClass.IsValid();
	}

	// Whether stepping back in the hierarchy is possible
	bool CanStepBack() const;
	// Step back in the filter hierarchy, returns true if the filter has changed
	bool StepBack();
	// Creates a new filter ref for a sub filter
	FBrickEditorObjectFilterRef CreateSubFilter(int32 Index) const;
	// Get the number of sub filters within this one
	int32 GetNumSubFilters() const;
	// Get the desired display name for the filter
	FText GetDisplayName() const
	{
		return GetFilterParams().GetDisplayName();
	}

	// Get all classes included with this filter
	const TArray<UClass*>& GetIncludedStaticInfoClasses() const;
	// Returns whether the given class is included in this filter
	bool IncludesStaticInfoClass(UClass* InClass) const
	{
		return GetIncludedStaticInfoClasses().Contains(InClass);
	}

	// Get the class that should be displayed as the thumbnail
	FSoftClassPath GetThumbnailStaticInfoClass() const;

private:
	// Get the params of the currently active filter or subfilter
	const FBrickEditorObjectFilterParams& GetFilterParams() const;
};

/**
 * This class is used as filters or folders for placables in the brick editor
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UBrickEditorObjectFilter : public UObject
{
	GENERATED_BODY()

protected:
	// ~Properties
	// Whether all classes that are not included in other categories should be displayed in this one
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter)
	bool bIncludeUncategorizedObjects;
	// Filter parameters for this class
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter, meta = (EditConidition = "!bIncludeUncategorizedObjects"))
	FBrickEditorObjectFilterParams FilterParams;
	// List of optional sub filters contained in this one
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Filter, meta = (EditConidition = "!bIncludeUncategorizedObjects"))
	TArray<FBrickEditorObjectFilterParams> SubFilters;
	// ~Properties

public:
	// ~Super Interface
	virtual FPrimaryAssetId GetPrimaryAssetId() const override;
	// ~Super Interface

	// Get all included objects and add them to the lists
	void BuildIncludedStaticInfoClasses(const TArray<UClass*>& StaticInfoClasses, TArray<UClass*>& InOutRemainingClasses);
	// Allows the filter to add uncategorized classes if desired
	void PostBuildUncategorizedStaticInfoClasses(const TArray<UClass*>& UncategorizedClasses);
	// Used for sorting the filters
	bool SortFilter(const UBrickEditorObjectFilter* Other) const;

	// Get the own or sub filter params
	FBrickEditorObjectFilterParams& GetFilterParams(int32 SubFilterIndex = INDEX_NONE);
	// Const version
	const FBrickEditorObjectFilterParams& GetFilterParams(int32 SubFilterIndex = INDEX_NONE) const;
	// Get the number of available sub filters
	int32 GetNumSubFilters() const;
	// Whether uncategorized objects should be included in this filter
	bool ShouldIncludeUncategorizedObjects() const;
};
