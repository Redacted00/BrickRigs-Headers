#pragma once

#include "ItemAction.h"
#include "UnequipAction.generated.h"

UCLASS()
class UUnequipAction : public UItemAction
{
	GENERATED_BODY()

public:
	// ~Constructor
	UUnequipAction();

	// ~Super Interface
	virtual bool CanStartOtherAction(const UCharacterAction* OtherAction) const override;
	virtual void OnActionFinished(bool bSuccess) override;
	// ~Super Interface
};
