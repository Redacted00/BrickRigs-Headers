// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/DecalComponent.h"
#include "ImpactDecalComponent.generated.h"

/**
 * Used for projectile impacts (bullet holes)
 */
UCLASS()
class BRICKRIGS_API UImpactDecalComponent : public UDecalComponent
{
	GENERATED_BODY()

private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnDecalFadedOut, UImpactDecalComponent*);

public:
	// ~Delegates
	FOnDecalFadedOut OnDecalFinishedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	virtual void OnAttachmentChanged() override;
	virtual void OnVisibilityChanged() override;
	virtual void OnHiddenInGameChanged() override;
	virtual void LifeSpanCallback() override;
	// ~Super Interface
};
