#pragma once

#include "GameplayTags.h"
#include "CoreMinimal.h"
#include "InventorySpecialSlot.generated.h"

class AInventoryItem;

USTRUCT(BlueprintType)
struct FInventorySpecialSlot
{
	GENERATED_BODY()

protected:
	// ~Properties
	// Display name of this slot
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
	// List of item tags this slot requires
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ItemTags;
	// Socket to attach the housed item to
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	// ~Properties

public:
	// ~Constructor
	FInventorySpecialSlot(const FGameplayTagContainer& InTags = FGameplayTagContainer::EmptyContainer, const FName& InSocketName = NAME_None)
		: ItemTags(InTags), SocketName(InSocketName)
	{
	}

	// Return whether the given item can reside in this slot
	bool SupportsItem(const AInventoryItem* InItem) const;
	// Returns whether this slot has the given tag
	bool HasItemTag(const FGameplayTag& Tag) const;
	// Sets a custom display name
	void SetDisplayName(const FText& InName);
	// Get the name of this special slot, for example "Primary" or "Sight"
	FText GetDisplayName() const;
	// Get the attach socket name
	FName GetSocketName() const;
};
