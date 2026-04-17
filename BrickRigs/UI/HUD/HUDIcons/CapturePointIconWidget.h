// Copyright Fluppisoft, 2020

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/HUDIcons/HUDIconWidget.h"
#include "CapturePointIconWidget.generated.h"

class UBrickTextBlock;
class ACapturePoint;
class ABaseCharacter;
class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UCapturePointIconWidget : public UHUDIconWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	// Cached character of the local player
	UPROPERTY(Transient)
	ABaseCharacter* PlayerCharacter;
	// Cached reference to the capture point
	UPROPERTY(Transient)
	ACapturePoint* CapturePoint;
	// Cached capture point state
	float CaptureRatio;
	bool bHasBeenCaptured;
	FGenericTeamId CapturingTeamId;
	FGenericTeamId CapturedByTeamId;
	// Cached attitude of the capturing team vs the local player team
	ETeamAttitude::Type CapturingTeamAttitude;
	// Whether the local player is currently within the capturing area
	bool bIsLocalPlayerOnCapturePoint;

	// The icon MID for the capture progress
	UPROPERTY(Transient)
	UMaterialInstanceDynamic* MID;
	// Cached parameter indices
	int32 CaptureRatioParamIndex;
	// Capture ratio applied on the material
	float MaterialCaptureRatio;
	// ~Variables

protected:
	// ~Widgets
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickImage* CaptureProgressImage;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	UBrickTextBlock* CharTextBlock;
	// ~Widgets

public:
	// ~Constructor
	UCapturePointIconWidget(const FObjectInitializer& ObjectInitializer);

	// ~Super Interface
	virtual void InitializeIcon_Implementation() override;
	virtual void UninitializeIcon_Implementation() override;
	virtual void OnLocalPlayerChangedTeam(const FGenericTeamId& InTeamId) override;
	virtual FText GetIconDisplayName_Implementation() const override;
	// ~Super Interface

private:
	// Callback for the player controller
	void SetPlayerCharacter(ABaseCharacter* NewCharacter);
	void OnPlayerCharacterChanged(ABaseCharacter* NewCharacter);
	// Update whether the local player is on the capture point
	void UpdatePlayerOnCapturePoint(bool bUpdateWidget);

	// Capture point callbacks
	void SetCaptureRatio(float NewCaptureRatio);
	void OnCaptureRatioChanged(float NewCaptureRatio);
	void SetCapturingTeam(const FGenericTeamId& InTeamId);
	void OnCapturingTeamChanged(const FGenericTeamId& InTeamId);
	void SetCapturedBy(const FGenericTeamId& InTeamId, bool bInHasBeenCaptured);
	void OnCapturedByChanged(const FGenericTeamId& InTeamId, bool bInHasBeenCaptured);
	void OnCharacterEnteredOrLeftCapturePoint();

	// Update the team attitude against the capturing team
	void UpdateCaptureProgressAttitude();
	// Update the capture ratio displayed
	void UpdateCaptureRatio();
};
