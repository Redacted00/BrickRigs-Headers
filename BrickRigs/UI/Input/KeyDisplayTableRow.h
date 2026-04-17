#pragma once

#include "Engine/DataTable.h"
#include "KeyDisplayTableRow.generated.h"

class UTexture2D;

USTRUCT(BlueprintType)
struct FKeyDisplayTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	// The displayed name of the key, not needed when there is an icon
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
	// Specific icon that can be used instead of the display name
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IconIndex;

	FKeyDisplayTableRow()
		: IconIndex(INDEX_NONE)
	{
	}
};
