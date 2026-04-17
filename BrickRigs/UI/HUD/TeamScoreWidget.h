// Copyright Fluppisoft, 2021

#pragma once

#include "Engine/StreamableManager.h"
#include "Misc/BrickTeam.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TeamScoreWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class BRICKRIGS_API UTeamScoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	TSharedPtr<FStreamableHandle> StreamableHandle_TeamIcon;
	// The associated team
	TWeakObjectPtr<UBrickTeam> BrickTeam;
	// ~Variables

public:
	// ~Super Interface
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Sets the associated team
	void InitializeTeam(UBrickTeam* NewTeam, ETeamAttitude::Type NewTeamAttitude);

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamName(const FText& NewName);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamIcon(UTexture2D* NewIcon);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTeamAttitude(ETeamAttitude::Type NewTeamAttitude);
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateScore(int32 NewScore, int32 NewMaxScore);
	// ~Blueprint Interface

private:
	void SetTeam(UBrickTeam* NewTeam);
	// Calls the blueprint version
	void UpdateScore();
	// Callback for the team
	void OnScoreChanged(int32 NewScore);
	void OnMaxScoreChanged(int32 NewScore);
	// Async loading callback
	void OnTeamIconLoaded();
	void OnTeamIconLoaded(UTexture2D* LoadedIcon);
};
