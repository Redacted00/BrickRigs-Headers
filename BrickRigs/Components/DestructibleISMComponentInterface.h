#pragma once

#include "Physics/ContactModifyInterface.h"
#include "UObject/Interface.h"
#include "DestructibleISMComponentInterface.generated.h"

class UDestructibleInstancesComponent;

// This interface is used to share functionality between the normal and hierarchical component
UINTERFACE()
class UDestructibleISMComponentInterface : public UInterface
{
	GENERATED_BODY()
};

class IDestructibleISMComponentInterface
{
	GENERATED_IINTERFACE_BODY()

private:
	// ~Variables
	// The destructible instance component that has spawned this component
	TWeakObjectPtr<UDestructibleInstancesComponent> ParentComponent;
	// Indicates if the instanced can ever be damaged
	bool bCanEverBeDamaged = true;
	// ~Variables

public:
	// Initializes the component after spawning
	void InitializeDestructibleISM(UDestructibleInstancesComponent* Parent, bool bInCanEverBeDamaged, float InMaxDrawDist);

protected:
	// Shared events that are called from HISM and ISM components
	void ReceiveComponentDamageInternal(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void OnContactModifyInternal(const FContactModifyParams& Params, int32 ActorIdx) const;
	void PostContactModifyInternal(const FPostContactModifyParams& Params, int32 ActorIdx);
};
