// Copyright Fluppisoft, 2022

#pragma once

#include "Misc/BrickAssetManager.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FireExtinguisherComponent.generated.h"

USTRUCT(BlueprintType)
struct FFireExtinguisherProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UParticleSystem> Emitter;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> Sound;
	// Sound played when a flame was extinguished
	UPROPERTY(EditAnywhere)
	USoundBase* ExtinguishSound = nullptr;
	UPROPERTY(EditAnywhere)
	float ExtinguishRadius = 75.f;
	UPROPERTY(EditAnywhere)
	float ExtinguishDistance = 300.f;
	// How likely an object is to be extinguished
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 1))
	float ExtinguishProbability = 0.5f;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BRICKRIGS_API UFireExtinguisherComponent : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DELEGATE_RetVal(FTransform, FGetNozzleTransform);
	DECLARE_DELEGATE_TwoParams(FGetNozzleAttachment, USceneComponent*&, FName&);

	// ~Variables
	FSmartStreamableHandle StreamableHandle_Emitter;
	FSmartStreamableHandle StreamableHandle_Sound;
	FFireExtinguisherProperties ExtinguisherProperties;
	TWeakObjectPtr<UParticleSystemComponent> ParticleComp;
	TWeakObjectPtr<UAudioComponent> AudioComp;
	bool bHadInitialTick = false;
	// ~Variables

public:
	// ~Delegates
	FGetNozzleTransform GetNozzleTransformDelegate;
	FGetNozzleAttachment GetNozzleAttachmentDelegate;
	// ~Delegates

	// ~Constructor
	UFireExtinguisherComponent();

	// ~Super Interface
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	// ~Super Interface

	void InitializeFireExtinguisher(const FFireExtinguisherProperties& Properties);
};
