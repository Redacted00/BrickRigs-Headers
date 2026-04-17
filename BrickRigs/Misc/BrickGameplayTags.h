#pragma once

#include "Bricks/ScalableBrick.h"
#include "Items/StaticInfo/WearableStaticInfo.h"
#include "Items/StaticInfo/AttachmentStaticInfo.h"
#include "GameplayTagsModule.h"
#include "CoreMinimal.h"

struct FBrickGameplayTags
{
private:
	// Needed during construction to keep track of the current tag hierarchy
	static FGameplayTag CurrentTag;

	static FGameplayTag AddTag(const FName& TagName)
	{
		const auto FullTagName = CurrentTag.IsValid() ? FName(*(CurrentTag.ToString() + '.' + TagName.ToString())) : TagName;
		// Assign to the parent tag so subsequent initializers can access it
		CurrentTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FullTagName);
		return CurrentTag;
	}

	// For a tag named Thing this macro creates a struct TagThing derived from the Tag struct
	// Inside the struct children are recursively declared using the same macro
	// After the struct declaration a variable of the new type is declared, for the root tag in the outer scope for children tags in the scope of their parent struct
	// In the constructors things will happen in the following order
	// 1. The parent Tag struct constructor is invoked with a new tag string that is a child of currentTag, currentTag is updated to reflect the newly created tag
	// 2. Children tag variables are initialized and create new tag strings for themselves
	// 3. The function body of the constructor is executed where currentTag is reset to what it was before the constructor was called
#define TAG(Name, Other) \
struct F##Name : public FGameplayTag { \
	F##Name() : FGameplayTag(AddTag(#Name)) { \
		CurrentTag = CurrentTag.RequestDirectParent(); \
	} \
	Other \
}; \
const F##Name Name;

public:
	// Declare input context tags
	TAG(Input,
	    TAG(Game,
		    TAG(Editor,
			    TAG(Camera, )
			    TAG(Default, )
			    TAG(Move, )
			    TAG(PickObjects, )
		    )
		    TAG(Pawn,
			    TAG(Spectator, )
			    TAG(Player,
				    TAG(Character, )
				    TAG(Vehicle,
					    TAG(Driver, )
				    )
			    )
		    )
		    TAG(Projectile, )
	    )
	    TAG(ContextMenu,
		    TAG(Property, )
		    TAG(PlacableObject, )
		    TAG(Editor, )
		    TAG(Message, )
	    )
	);

	// Declare property categories
	TAG(Properties,
	    TAG(Settings,
		    TAG(Video, )
		    TAG(Gameplay, )
		    TAG(UserInterface, )
		    TAG(Input, )
		    TAG(InputMappings, )
		    TAG(Host, )
		    TAG(Match, )
	    )
	    TAG(Editor,
		    TAG(Move, )
	    )
	    TAG(MatchMenu, )
	    TAG(IsMultiplayer, )
	    TAG(IsEditor, )
	    TAG(DisableWeather, )
	    TAG(IsCurrentMatch, )
	    TAG(DefaultValues, )
	);
#undef TAG

	static const FGameplayTag& GetItemTag(EItemTag InTag)
	{
		return Get().ItemTags[InTag];
	}

	static const FGameplayTag& GetAttachmentTag(EAttachmentTag InTag)
	{
		return Get().AttachmentTags[InTag];
	}

	static const FGameplayTag& GetWearableTag(EWearableTag InTag)
	{
		return Get().WearableTags[InTag];
	}

	static const FGameplayTag& GetBrickTag(UClass* InBrickClass)
	{
		return Get().BrickTags[InBrickClass];
	}

	static const FGameplayTag& GetScalableBrickTag(EScalableBrickShape InBrickShape)
	{
		return Get().ScalableBrickTags.Contains(InBrickShape) ? Get().ScalableBrickTags[InBrickShape] : FGameplayTag::EmptyTag;
	}

	// Returns the instance after ensuring that it has been initialized
	static const FBrickGameplayTags& Get();

private:
	// ~Constructor
	FBrickGameplayTags();

	template <typename EnumType>
	void RegisterEnum(const FString& CategoryName, TMap<EnumType, FGameplayTag>& Map)
	{
		UEnum* Enum = StaticEnum<EnumType>();
		for (int32 i = 0; i < Enum->GetMaxEnumValue(); ++i)
		{
			FString EnumName = Enum->GetNameStringByIndex(i);
			EnumName.ReplaceCharInline('_', '.', ESearchCase::CaseSensitive);
			const FString Tag = CategoryName + TEXT(".") + EnumName;
			const FGameplayTag NewTag = UGameplayTagsManager::Get().AddNativeGameplayTag(*Tag);
			Map.Add(EnumType(i), NewTag);
		}
	}

	// Cached tag maps used to store the tags that have been created
	TMap<EItemTag, FGameplayTag> ItemTags;
	TMap<EWearableTag, FGameplayTag> WearableTags;
	TMap<EAttachmentTag, FGameplayTag> AttachmentTags;
	TMap<UClass*, FGameplayTag> BrickTags;
	TMap<EScalableBrickShape, FGameplayTag> ScalableBrickTags;
};
