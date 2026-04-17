// Copyright Fluppisoft, 2016

#pragma once

#include "Misc/BrickTimer.h"
#include "Online/KickedPlayer.h"
#include "OnlineSessionSettings.h"
#include "GameFramework/GameSession.h"
#include "Player/BrickChatMessage.h"
#include "BrickGameSession.generated.h"

class APlayerController;
class ABrickPlayerController;
class ABrickPlayerState;

enum class EUpdateSessionFlags
{
	None = 0,
	PlayerCount = 1,
	MatchSettings = 2,
	All = MAX_uint8
};

ENUM_CLASS_FLAGS(EUpdateSessionFlags);

/**
 *
 */
UCLASS()
class BRICKRIGS_API ABrickGameSession : public AGameSession
{
	GENERATED_BODY()

public:
	constexpr static int32 MaxAdminPasswordLength = 20;

private:
	// Maximum number of chat messages to save
	constexpr static int32 MaxNumChatMessages = 30;

	// ~Variables
	// Delegate handle for the session creation process
	FDelegateHandle DelegateHandle_CreateSession;
	// Timer used for updating the session info
	FBrickTimer Timer_UpdateSession;
	// Indicates what parts of the session to update next
	EUpdateSessionFlags PendingSessionUpdateFlags;
	// The admin password of the server
	UPROPERTY(Config)
	FString AdminPassword;
	// List of players that have been kicked or banned
	UPROPERTY(Config)
	TArray<FKickedPlayer> KickedPlayers;
	// List of recent chat messages
	TArray<FBrickChatMessage> ChatMessages;
	// List of saved messages
	UPROPERTY(Config)
	TArray<FBrickChatMessage> ChatMessageLog;
	// Number of delta time samples collected
	int32 NumDeltaTimeSamples;
	// Accumulated delta time
	float DeltaTimeSamples;
	// The current average frame rate
	int32 AverageFrameRate;
	// ~Variables

public:
	// ~Properties
	// Number of times an incorrect password may be sent, < 0 indicates no limit
	UPROPERTY(EditDefaultsOnly, Config, Category = Admin)
	int32 MaxAdminLoginAttempts;
	// How long for a player will be kicked after failing to login as an admin in seconds
	UPROPERTY(EditDefaultsOnly, Config, Category = Admin)
	int32 FailedAdminLoginKickDuration;
	// ~Properties

	// ~Constructor
	ABrickGameSession();

	// ~Super Interface
	virtual void PostInitProperties() override;
	virtual void InitOptions(const FString& Options) override;
	virtual void RegisterServer() override;
	virtual void Tick(float DeltaTime) override;

private:
	// Overridden to be made private
	virtual bool KickPlayer(APlayerController* KickedPlayer, const FText& Message) override;
	virtual bool BanPlayer(APlayerController* BannedPlayer, const FText& Message) override;
	// ~Super Interface

public:
	// ~Game Session
	// Convenience function to get the game session from the game mode
	UFUNCTION(BlueprintPure, Meta = (WorldContext = WorldContextObject, DisplayName = "Get Game Session"))
	static ABrickGameSession* Get(const UObject* WorldContextObject);
	// ~Game Session

	// ~Game Mode
	// Called from the game mode when a new player has joined
	void OnPlayerJoined(ABrickPlayerController* PC);
	// ~Game Mode

	// ~Admins
	// Set the admin password and save it to config
	void SetAdminPassword(const FString& PW);
	// Return whether the given password is valid
	bool IsAdminPasswordCorrect(const FString& PW) const;
	// ~Admins

	// ~Kicks
	// Kick a player from the game and restrict him from joining again within the given duration
	void KickPlayer(const FUniqueNetIdRepl& PlayerId, const FString& PlayerName, const FString& KickReason, const FTimespan& KickDuration);
	// Kick an active player from the server without registering them as kicked
	void KickPlayer(ABrickPlayerController* PC, const FString& KickReason, const FTimespan& KickDuration) const;
	// Remove the kick/ban for a certain player
	void RevokeKick(const FUniqueNetIdRepl& PlayerId);
	// Return true if the given player id is currently kicked or banned from the server
	bool HasActiveKick(const FUniqueNetIdRepl& UniqueId, FKickedPlayer& OutKick);
	// Used to access the kicked players for the UI
	const auto& GetKickedPlayers() const
	{
		return KickedPlayers;
	}

private:
	// Go through the list of kicked and banned players and remove outdated entries
	void FlushKickedPlayerList(bool bRemoveDoubles);
	// Should be called whenever a change to the kicked player list is made
	void OnKickedPlayerListChanged();
	// ~Kicks

public:
	// ~Password
	// Get the current password of the session
	FString GetSessionPassword() const;
	// Return whether this session is password protected
	bool HasSessionPassword() const;
	// Return whether the given password matches the server password
	bool IsSessionPasswordCorrect(const FString& PW) const;
	// ~Password

	// ~Session
	// Queues an update to the session data
	void QueueOnlineSessionUpdate(EUpdateSessionFlags InUpdateFlags);

private:
	// Callback for creating the game session
	void OnCreateSessionComplete(FName InSessionName, bool bWasSuccessful);
	// Updates the match settings related session settings
	void UpdateMatchSettings(FOnlineSessionSettings& SessionSettings);
	// Update the player count on an existing session
	void UpdateSessionPlayerCount(FNamedOnlineSession* OnlineSession);
	// Called from a timer periodically to update the advertised session settings
	void UpdateOnlineSession();
	// ~Session

public:
	// ~Chat
	// Whether the chat feature is enabled for this session
	bool IsChatEnabled() const;
	// Adds a new chat message, broadcasts it to receiving players
	void AddChatMessage(const FBrickChatMessage& NewMessage);
	// ~Chat
};
