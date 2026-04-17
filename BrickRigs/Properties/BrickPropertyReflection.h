#pragma once

#include "GameplayTagsModule.h"
#include "BrickPropertyEditInfo.h"
#include "UObject/UnrealType.h"
#include "CoreMinimal.h"
#include "BrickPropertyReflection.generated.h"

struct FBrickStructPropertyInterface;
struct FBrickProperty;

struct FBrickPropertyCategory
{
	FText DisplayName;
};

USTRUCT(BlueprintType)
struct FBrickPropertyReflectionFilter
{
	GENERATED_BODY()

	// Custom tags that can be used to pass data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer Tags;
};

struct FBrickPropertyReflection
{
private:
	// ~Variables
	// Whether properties are being collected for serialization
	const bool bIsSerializing;
	// Property container objects
	const TArray<TWeakObjectPtr<UObject>> ContainerObjects;
	// Optional reflection filters
	FBrickPropertyReflectionFilter Filter;

	// List of all brick properties, used for serialization
	TArray<FBrickPropertyInstance> BrickProperties;
	// List of all property infos mapped to their category indices, used for reflection
	TArray<TPair<TSharedRef<FBrickPropertyEditInfo>, int32>> BrickPropertyEditInfos;

	// List of all display categories
	TArray<FBrickPropertyCategory> Categories;
	// Currently opened category index
	int32 CurrentCategoryIndex = INDEX_NONE;
	// Whether the parent struct property category should be used
	bool bUseParentCategory = false;

	// Transient chain of parent struct properties that are currently being added
	TArray<FStructProperty*> ParentPropertyChain;
	// ~Variables

public:
	// ~Constructor
	FBrickPropertyReflection(bool bInIsSerializing = true, const TArray<TWeakObjectPtr<UObject>>& InContainerObjects = TArray<TWeakObjectPtr<UObject>>(), const FBrickPropertyReflectionFilter& InFilter = FBrickPropertyReflectionFilter())
		: bIsSerializing(bInIsSerializing), ContainerObjects(InContainerObjects), Filter(InFilter)
	{
	}

	void ResetReflection(bool bShrink)
	{
		CurrentCategoryIndex = INDEX_NONE;
		BrickPropertyEditInfos.Empty(bShrink ? 0 : BrickPropertyEditInfos.Num());
		BrickProperties.Empty(bShrink ? 0 : BrickProperties.Num());
		ParentPropertyChain.Empty(bShrink ? 0 : ParentPropertyChain.Num());
	}

	// Adds a new display category
	void AddCategoryInternal(const FText& DisplayName)
	{
		// Abort if the parent category should be used
		if (bUseParentCategory)
		{
			return;
		}

		// Check if the category already exists
		CurrentCategoryIndex = Categories.IndexOfByPredicate([&](const FBrickPropertyCategory& Category)
		{
			return Category.DisplayName.IdenticalTo(DisplayName, ETextIdenticalModeFlags::DeepCompare);
		});

		// Add a new category if needed
		if (CurrentCategoryIndex == INDEX_NONE)
		{
			CurrentCategoryIndex = Categories.Emplace(DisplayName);
		}
	}

#define BRICK_PROPERTY_CATEGORY(DisplayName) \
	const auto EnsureCategoryDeclared = true; \
	if (Params.IsReflecting()) \
		Params.AddCategoryInternal(DisplayName);

	bool HasTag(const FGameplayTag& Tag) const
	{
		return !Filter.Tags.IsEmpty() && Filter.Tags.HasTagExact(Tag);
	}

	const auto& GetCategories() const
	{
		return Categories;
	}

	const auto* GetCategory(int32 Index) const
	{
		return Categories.IsValidIndex(Index) ? &Categories[Index] : nullptr;
	}

	const auto& GetProperties() const
	{
		return BrickProperties;
	}

	int32 GetNumProperties() const
	{
		return BrickProperties.Num();
	}

	const auto& GetPropertyEditInfos() const
	{
		return BrickPropertyEditInfos;
	}

	const FBrickPropertyInstance* FindPropertyByName(const FString& PropertyName, int32& OutIndex) const
	{
		for (int32 i = 0; i < BrickProperties.Num(); ++i)
		{
			if (BrickProperties[i].GetFullPropertyName() == PropertyName)
			{
				OutIndex = i;
				return &BrickProperties[i];
			}
		}

		return nullptr;
	}

	bool ContainsBrickProperty(const FString& FullPropertyName) const
	{
		const auto FullNameLength = FullPropertyName.Len();
		for (const auto& Pair : BrickPropertyEditInfos)
		{
			const auto& PropertyInfo = Pair.Key;
			const auto PropertyName = PropertyInfo->GetFullPropertyName();
			const auto PropertyNameLength = PropertyName.Len();

			// Only match if the names are identical, or the passed in name is a parent property
			if (PropertyName.StartsWith(FullPropertyName) && (PropertyNameLength <= FullNameLength || PropertyName[FullNameLength] == '.'))
			{
				return true;
			}
		}

		return false;
	}

	UObject* GetSelectedObject() const
	{
		if (ContainerObjects.Num())
		{
			return ContainerObjects[0].Get();
		}

		return nullptr;
	}

	bool ContainsObject(UObject* InObject) const
	{
		return ContainerObjects.Contains(InObject);
	}

	bool IsSerializing() const
	{
		return bIsSerializing;
	}

	bool IsReflecting() const
	{
		return !bIsSerializing;
	}

private:
	// NOTE: We use the LIKELY macro to make loading and saving faster, the display info is only generated for one or few objects at once so it doesn't really matter
#define REFLECT_BRICK_PROPERTY_INTERNAL(DisplayName, BrickProperty, Property, PropertyName, FullPropertyName, bAddFProperty) \
		EnsureCategoryDeclared; \
		const static auto BrickProp##PropertyName = Params.InitializeBrickProperty(BrickProperty, Property, #PropertyName); \
		if (LIKELY(Params.IsSerializing())) { \
		if (bAddFProperty) \
			Params.AddBrickProperty(BrickProp##PropertyName, FullPropertyName); } \
		else \
			Params.AddBrickPropertyDisplayInfo(BrickProp##PropertyName, FullPropertyName, DisplayName)
	// Internal macro used for inline properties
#define REFLECT_BRICK_PROPERTY_INLINE(PropertyName, BrickProperty, DisplayName) \
		REFLECT_BRICK_PROPERTY_INTERNAL(DisplayName, BrickProperty, StaticStruct()->FindPropertyByName(#PropertyName), PropertyName, #PropertyName, true)

public:
	// Reflects a normal inline property
#define REFLECT_BRICK_PROPERTY(PropertyName, DisplayName, ...) \
		REFLECT_BRICK_PROPERTY_INLINE(PropertyName, AutoConstructBrickProp(&PropertyName, ## __VA_ARGS__), DisplayName)
	// Reflects a normal property with a custom property type
#define REFLECT_BRICK_PROPERTY_CUSTOM(PropertyName, PropertyType, DisplayName, ...) \
		REFLECT_BRICK_PROPERTY_INLINE(PropertyName, FBrickProperty::ConstructBrickProperty<PropertyType>(__VA_ARGS__), DisplayName)

	// Reflect an external property, such as the scalability settings
	// NOTE: Only add display info, external properties can't be serialized
#define REFLECT_BRICK_PROPERTY_EXTERNAL(PropertyType, DisplayName, ...) \
		REFLECT_BRICK_PROPERTY_INTERNAL(DisplayName, FBrickProperty::ConstructBrickProperty<PropertyType>(__VA_ARGS__), nullptr, PropertyType, PropertyType::StaticType().ToString(), false)

	// Default simple version
#define REFLECT_STRUCT_PROPERTIES(Struct, DisplayName) \
		REFLECT_STRUCT_PROPERTIES_FULL(Struct, DisplayName, {})
	// Version that allows specifying filter params
#define REFLECT_STRUCT_PROPERTIES_FULL(Struct, DisplayName, Filter) \
		static FStructProperty* Property##Struct = CastFieldChecked<FStructProperty>(StaticStruct()->FindPropertyByName(#Struct)); \
		REFLECT_STRUCT_PROPERTIES_INTERNAL(Property##Struct, Struct, DisplayName, Filter)
	// Internal version
#define REFLECT_STRUCT_PROPERTIES_INTERNAL(StructProperty, Struct, DisplayName, Filter) \
	const auto bShow##StructProperty = !Params.IsSerializing() && !DisplayName.IsEmpty();\
	if (bShow##StructProperty) { \
	REFLECT_BRICK_PROPERTY_CUSTOM(Struct, FStructBrickProperty, DisplayName); } \
	if (!bShow##StructProperty) \
	Params.AddStructPropertyChildren(StructProperty, Struct, Filter); \
	else \
	Params.AddStructPropertyChildrenEditInfo(StructProperty, Struct, Filter)

	template <typename PropertyType>
	static const TSharedRef<PropertyType>& InitializeBrickProperty(const TSharedRef<PropertyType>& InBrickProperty, FProperty* InProperty, const FName& InPropertyName)
	{
		InBrickProperty->SetPropertyParams(InProperty, InPropertyName);
		return InBrickProperty;
	}

	void AddBrickProperty(const TSharedRef<FBrickProperty>& InBrickProperty, const FString& InFullPropertyName)
	{
		ensure(IsSerializing() && InBrickProperty->GetProperty());
		BrickProperties.Add(FBrickPropertyInstance(InBrickProperty, InFullPropertyName, ParentPropertyChain));
	}

	FBrickPropertyEditInfo& AddBrickPropertyDisplayInfo(const TSharedRef<FBrickProperty>& InBrickProperty, const FString& InFullPropertyName, const FText& InDisplayName)
	{
		ensure(IsReflecting());
		const auto Index = BrickPropertyEditInfos.Emplace(MakeShared<FBrickPropertyEditInfo>(FBrickPropertyInstance(InBrickProperty, InFullPropertyName, ParentPropertyChain), InDisplayName, ContainerObjects), CurrentCategoryIndex);
		return BrickPropertyEditInfos[Index].Key.Get();
	}

	// Adds the children properties of a struct property
	void AddStructPropertyChildren(FStructProperty* InProperty, const FBrickStructPropertyInterface& InStruct, const FBrickPropertyReflectionFilter& InFilter);

	// Reflection version that returns the struct property edit info
	FBrickPropertyEditInfo& AddStructPropertyChildrenEditInfo(FStructProperty* InProperty, const FBrickStructPropertyInterface& InStruct, const FBrickPropertyReflectionFilter& InFilter);
};
