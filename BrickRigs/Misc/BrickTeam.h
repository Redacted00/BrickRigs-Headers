// Copyright Fluppisoft, 2020

#pragma once

#include "GenericTeamAgentInterface.h"
#include "Inventory/InventoryLoadout.h"
#include "CoreMinimal.h"
#include "BrickTeam.generated.h"

class UTexture2D;

/**
 * Stores static and runtime info about teams
 */
UCLASS(Abstract, Blueprintable)
class BRICKRIGS_API UBrickTeam : public UObject
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, int32);

	// ~Variables
	// The id this team is associated with
	FGenericTeamId TeamId;
	// The replicated score of the team
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Score)
	uint16 Score;
	UFUNCTION()
	void OnRep_Score();
	// The maximum score for the team
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MaxScore)
	uint16 MaxScore;
	UFUNCTION()
	void OnRep_MaxScore();
	// Key used for more efficient replication
	int32 ReplicationKey;
	// ~Variables

	// ~Properties
	UPROPERTY(EditDefaultsOnly, Category = Team)
	FText DisplayName;

public:
	// Texture displayed on the players' badge
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Team)
	TSoftObjectPtr<UTexture2D> BadgeTexture;
	// Color to be displayed on flags for example
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Team)
	FLinearColor TeamColor;
	// The loadout players of this team are given by default
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Team)
	FInventoryLoadout DefaultLoadout;
	// ~Properties

	// ~Delegates
	FOnScoreChanged OnScoreChangedDelegate;
	FOnScoreChanged OnMaxScoreChangedDelegate;
	// ~Delegates

	// ~Super Interface
	virtual bool IsSupportedForNetworking() const override;
	virtual bool IsNameStableForNetworking() const override;
	// ~Super Interface

	// Returns the current replication key
	int32 GetReplicationKey() const
	{
		return ReplicationKey;
	}

	// Get the display name of the team
	UFUNCTION(BlueprintPure)
	FText GetTeamDisplayName() const
	{
		return DisplayName;
	}

	// Sets the associated team id
	void SetTeamId(const FGenericTeamId& InTeamId);
	// Get the associated team id
	UFUNCTION(BlueprintPure)
	const FGenericTeamId& GetTeamId() const;
	// Sets the current score, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetScore(int32 NewScore);
	// Get the current score, server and clients
	UFUNCTION(BlueprintPure)
	int32 GetScore() const
	{
		return Score;
	}

	// Sets the maximum score, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetMaxScore(int32 NewMaxScore);
	// Get the maximum score
	UFUNCTION(BlueprintPure)
	int32 GetMaxScore() const
	{
		return MaxScore;
	}

private:
	void OnScoreChanged();
	void OnMaxScoreChanged();
};
