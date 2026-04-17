// Copyright Fluppisoft, 2020

#pragma once

#include "BrickEditorComponentInterface.h"
#include "CoreMinimal.h"
#include "Particles/ParticleSystemComponent.h"
#include "BrickEditorParticleComponent.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UBrickEditorParticleComponent : public UParticleSystemComponent, public IBrickEditorComponentInterface
{
	GENERATED_BODY()

public:
	// ~Constructor
	UBrickEditorParticleComponent();

	// ~Super Interface
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	// ~Super Interface

private:
	// Callback for our own delegate
	UFUNCTION()
	void OnSystemFinishedCallback(UParticleSystemComponent* InParticleComponent);
};

struct FBrickEditorParticleSystemComponentParams : public FBrickEditorPrimitiveComponentParams
{
	UParticleSystem* Template;

	FBrickEditorParticleSystemComponentParams(const UBrickEditorParticleComponent* Default = GetDefault<UBrickEditorParticleComponent>())
		: FBrickEditorPrimitiveComponentParams(Default),
		  Template(Default->Template)
	{
	}

	void InitializeComponent(UBrickEditorParticleComponent* Comp)
	{
		FBrickEditorPrimitiveComponentParams::InitializeComponent(Comp);

		Comp->Template = Template;
		// Reset the parameters
		Comp->InstanceParameters.Empty();
	}
};
