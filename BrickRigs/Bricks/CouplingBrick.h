// Copyright Fluppisoft, 2016

#pragma once

#include "Bricks/Brick.h"
#include "CouplingBrick.generated.h"

UENUM()
enum class ECouplingMode : uint8
{
	Default,
	Static
};

UCLASS(Abstract)
class BRICKRIGS_API UCouplingBrickBaseStaticInfo : public UBrickStaticInfo
{
	GENERATED_BODY()

public:
	// ~Constructor
	UCouplingBrickBaseStaticInfo();
};

UCLASS(Abstract)
class BRICKRIGS_API UCouplingBrickStaticInfo : public UCouplingBrickBaseStaticInfo
{
	GENERATED_BODY()

public:
	// ~Properties
	// Angular constraint limits in every direction
	UPROPERTY(EditDefaultsOnly, Category = Coupling)
	FRotator AngularLimits;
	// ~Properties

	// ~Constructor
	UCouplingBrickStaticInfo();
};

/**
 *
 */
UCLASS()
class BRICKRIGS_API UCouplingBrick : public UBrick
{
	GENERATED_BODY()

	// ~Variables
	// The associated coupling connection
	TWeakObjectPtr<UCouplingConnection> CouplingConnection;
	// Cached state of the input channel, used to detect a change from false to true
	bool bInputChannelValue = false;
	// ~Variables

protected:
	// ~Brick properties
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Coupling)
	FVehicleInputChannel InputChannel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Coupling)
	ECouplingMode CouplingMode = ECouplingMode::Default;
	// ~Brick properties

public:
	// ~Constructor
	UCouplingBrick();

	// ~Super Interface
	virtual void PostInitializeBrickEditorObject() override;
	virtual void RepairBrick() override;
	virtual bool ShouldBrickTick() const override;
	virtual void TickBrick(float DeltaTime) override;

	virtual bool HasAnyInputChannel() const override
	{
		return CanEngageCoupling();
	}

	virtual void GetInputChannels(TArray<FVehicleInputChannel*>& OutInputChannels) const override;
	virtual UBrickConnection* CreateCustomBrickConnection(UBrick* OtherBrick, const FName& ConnectionName, FBrickConnectionParams& Params) const override;
	virtual void OnBrickConnectionActivated(UBrickConnection* InConnection) override;
	virtual void OnBrickConnectionBroke(UBrickConnection* Connection) override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface

	// Whether the coupling can be engaged via interaction
	bool CanEngageOrDisengageViaInteraction() const
	{
		return CanEngageCoupling() && InputChannel.InputAxis == EVehicleInputAxis::None;
	}

	// Whether the coupling can ever be engaged
	bool CanEngageCoupling() const
	{
		return CouplingMode == ECouplingMode::Default;
	}

	// Whether the coupling is currently engaged
	bool IsCouplingEngaged() const;
	// Engages or the coupling, server only
	void EngageCoupling();
	// Disengages the coupling, server only
	void DisengageCoupling();
	// Called from the coupling connection when it has started the transition animation
	void OnCouplingConnectionTransitionStart();

private:
	// Find a brick within reach that we could attach to
	UBrick* FindEngageTarget(FBrickConnectionParams& OutConnectionParams) const;
	// Called when the input channel value has changed
	void OnInputChannelChanged();

	// Interaction component callbacks
	void GetInteractionOptions(ABrickPlayerController* OtherPC, FInteractionOptions& OutOptions);
	FVector GetInteractionLocation() const;
	UFUNCTION()
	void Interact_EngageCoupling(ABrickPlayerController* PC);
	UFUNCTION()
	void Interact_DisengageCoupling(ABrickPlayerController* PC);

	// Property callbacks
	static void GetCouplingModeItems(const FBrickPropertyContainer& Container, TArray<FEnumPropertyItem>& OutItems);
};
