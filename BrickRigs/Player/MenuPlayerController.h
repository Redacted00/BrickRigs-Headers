// Copyright Fluppisoft, 2016

#pragma once

#include "BasePlayerController.h"
#include "MenuPlayerController.generated.h"

class AMenuSequence;

/**
 * 
 */
UCLASS()
class BRICKRIGS_API AMenuPlayerController : public ABasePlayerController
{
	GENERATED_BODY()

private:
	// ~Variables
	UPROPERTY(Transient)
	AMenuSequence* MenuSequence;
	// ~Variables

protected:
	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FVector CameraLocation;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FRotator CameraRotation;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	float CameraFOV;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	FViewTargetTransitionParams ViewTargetTransitionParams;
	// ~Properties

public:
	// ~Constructor
	AMenuPlayerController();

	// ~Super Interface
	virtual void ReceivedPlayer() override;
	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;
	virtual bool CanOpenOrCloseMenu(bool bOpen) const override;
	// ~Super Interface

private:
	// Callback for the game mode
	void OnMenuSequenceCreated(AMenuSequence* InSequence);
};
