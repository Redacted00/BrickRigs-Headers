#pragma once

#include "CoreMinimal.h"
#include "InventoryLoadout.generated.h"

class AInventoryItem;

USTRUCT(BlueprintType)
struct FInventoryLoadoutSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowedClasses = "InventoryItem"))
	UClass* ItemClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0))
	int32 Amount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowedClasses = "InventoryItem"))
	TArray<UClass*> Attachments;

	bool IsSameSlot(const FInventoryLoadoutSlot& Other) const
	{
		return ItemClass == Other.ItemClass && Attachments == Other.Attachments;
	}
};

// This struct can be used to build a loadout via blueprints
USTRUCT(BlueprintType)
struct FInventoryLoadout
{
	GENERATED_BODY()
	;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryLoadoutSlot> Slots;

	void AddOrMergeSlot(const FInventoryLoadoutSlot& InSlot)
	{
		// Find and existing slot
		for (auto& OtherSlot : Slots)
		{
			if (OtherSlot.IsSameSlot(InSlot))
			{
				OtherSlot.Amount += InSlot.Amount;
				return;
			}
		}

		// Add a new slot
		Slots.Add(InSlot);
	}
};
