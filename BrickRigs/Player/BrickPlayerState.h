// Copyright Fluppisoft, 2017

#pragma once

#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerState.h"
#include "BrickPlayerState.generated.h"

class ABaseCharacter;

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickPlayerState : public APlayerState, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerNameChanged, const FString&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsAdminChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnTeamChanged, const FGenericTeamId&);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsTeamLeaderChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPingChanged, int32);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnIsAliveChanged, bool);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnScoreChanged, float);
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMoneyChanged, float, float);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnKillsChanged, int32);

	// ~Variables
	// Inactive character that is associated with this player state
	UPROPERTY(Transient)
	ABaseCharacter* InactiveCharacter;

	// Whether the player is the host, i.e. listen server
	bool bIsHost;
	// Whether the player is logged in as an admin
	UPROPERTY(ReplicatedUsing = OnRep_IsAdmin, Transient)
	bool bIsAdmin;
	UFUNCTION()
	void OnRep_IsAdmin();

	// Team id of this player
	UPROPERTY(ReplicatedUsing = OnRep_TeamId, Transient)
	FGenericTeamId TeamId;
	UFUNCTION()
	void OnRep_TeamId();
	// Whether this player is the leader of their team
	UPROPERTY(ReplicatedUsing = OnRep_IsTeamLeader, Transient)
	bool bIsTeamLeader;
	UFUNCTION()
	void OnRep_IsTeamLeader();

	// The money this player has available
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Money)
	float Money;
	UFUNCTION()
	void OnRep_Money(float PrevValue);
	// Kill counter for this player
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Kills)
	uint16 Kills;
	UFUNCTION()
	void OnRep_Kills();
	// Death counter
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Deaths)
	uint16 Deaths;
	UFUNCTION()
	void OnRep_Deaths();

	// Indicates if the player is currently alive, updated from the character
	UPROPERTY(Transient, ReplicatedUsing = OnRep_IsAlive)
	bool bIsAlive;
	UFUNCTION()
	void OnRep_IsAlive();

	// Set to true while properties are being copied after seamless travel
	bool bIsCopyingPropertiesForSeamlessTravel;
	// ~Variables

public:
	// ~Delegates
	FOnPlayerNameChanged OnPlayerNameChangedDelegate;
	FOnIsAdminChanged OnIsAdminChangedDelegate;
	FOnTeamChanged OnTeamChangedDelegate;
	FOnIsTeamLeaderChanged OnIsTeamLeaderChangedDelegate;
	FOnPingChanged OnPingChangedDelegate;
	FOnIsAliveChanged OnIsAliveChangedDelegate;
	FOnScoreChanged OnScoreChangedDelegate;
	FOnMoneyChanged OnMoneyChangedDelegate;
	FOnKillsChanged OnKillsChangedDelegate;
	FOnKillsChanged OnDeathsChangedDelegate;
	// ~Delegates

	// ~Constructor
	ABrickPlayerState();

	// ~Super Interface
	virtual void CopyProperties(APlayerState* PlayerState) override;
	virtual void SeamlessTravelTo(APlayerState* NewPlayerState) override;
	virtual bool ShouldBroadCastWelcomeMessage(bool bExiting = false) override;
	virtual void UpdatePing(float InPing) override;
	virtual void PreNetReceive() override;
	virtual void PostNetReceive() override;
	virtual void OnRep_PlayerName() override;
	// ~Super Interface

	// Converts a player name string to an FText
	static FText PlayerNameToText(const FString& InPlayerName);

	// ~Testing
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	void InitFakePlayerState(int32 Index);
	// Randomly changes the player statistics
	void FakePlayerStats();
	void FakeTeamChange();
#endif
	// ~Testing

	// ~Misc
	// Get the players name as a text
	UFUNCTION(BlueprintPure)
	FText GetPlayerNameText() const
	{
		return PlayerNameToText(GetPlayerName());
	}

	// Get the current ping value, uncompressed
	UFUNCTION(BlueprintPure)
	int32 GetUncompressedPing() const
	{
		return GetPing() * 4;
	}

	// Sets the associated inactive character
	void SetInactiveCharacter(ABaseCharacter* InCharacter);
	// Returns the associated character
	ABaseCharacter* GetInactiveCharacter() const;
	// Used to initially mark the player as the host
	void SetIsHost(bool bNewIsHost);
	// Whether this player is the host (on a listen server)
	UFUNCTION(BlueprintPure)
	bool IsHost() const
	{
		return bIsHost;
	}

	// ~Misc

	// ~Admin
	// Set the admin login state, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetIsAdmin(bool bNewIsAdmin);
	// Return whether the player is logged in as an admin
	UFUNCTION(BlueprintPure)
	bool IsAdmin() const
	{
		return bIsAdmin;
	}

private:
	// Called whenever the admin flag has changed
	void OnIsAdminChanged();
	// ~Admin

public:
	// ~Score
	// Set the total score, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetScore(float NewScore);

private:
	// Called whenever the score has changed
	void OnScoreChanged();
	// ~Score

public:
	// ~Money
	// Set the amount of money owned, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetMoney(float NewValue);
	// Get the current balance
	UFUNCTION(BlueprintPure)
	float GetMoney() const;

private:
	// Called whenever money has been added or removed
	void OnMoneyChanged(float PrevMoney);
	// ~Money

public:
	// ~Kills and Deaths
	// Set the total amount of kills, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetKills(int32 NewKills);
	// Add a kill to the stats
	void AddKill();
	// Get the kill count
	UFUNCTION(BlueprintPure)
	int32 GetKills() const;
	// Sets the total amount of deaths, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetDeaths(int32 NewDeaths);
	// Add a death to the stats
	void AddDeath();
	// Get the death counter
	UFUNCTION(BlueprintPure)
	int32 GetDeaths() const;

private:
	// Called when the kill or death counter has changed
	void OnKillsChanged();
	void OnDeathsChanged();
	// ~Kills and Deaths

public:
	// Let the player update the death flag, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetIsAlive(bool bInIsAlive);
	// Return whether the character owning this player is currently alive
	UFUNCTION(BlueprintPure)
	bool IsAlive() const
	{
		return bIsAlive;
	}

private:
	// Called whenever the alive flag has changed
	void OnIsAliveChanged();

public:
	//  ~Team
	// Sets the team leader flag, server only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void SetIsTeamLeader(bool bNewLeader);
	// Returns whether this player is a team leader
	UFUNCTION(BlueprintPure)
	bool IsTeamLeader() const;

private:
	// Called whenever the team leader flag has been changed
	void OnIsTeamLeaderChanged();
	// To be called when the team id has been changed
	void OnTeamIdChanged();
	//  ~Team

	// Called when the ping was changed
	void OnPingChanged();

public:
	// ~IGenericTeamAgentInterface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamId) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	// ~IGenericTeamAgentInterface
};
