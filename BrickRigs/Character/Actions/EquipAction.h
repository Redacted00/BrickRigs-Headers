#pragma once

#include "ItemAction.h"
#include "EquipAction.generated.h"

UCLASS()
class UEquipAction : public UItemAction
{
	GENERATED_BODY()

public:
	// ~Constructor
	UEquipAction();

	// ~Super Interface
	virtual void OnActionFinished(bool bSuccess) override;
	// ~Super Interface
};
