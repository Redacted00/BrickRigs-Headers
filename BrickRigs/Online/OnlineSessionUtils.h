#pragma once

#include "OnlineSessionSettings.h"
#include "CoreMinimal.h"
#include "OnlineSubsystem.h"

class ULevelInfo;
class UGameModeInfo;

#define SETTING_PASSWORDPROTECTED "PASSWORD"
#define SETTING_DESCRIPTION "DESCR"
#define SETTING_FRAMERATE "FPS"
// Whether players with different mods are allowed to join
#define SETTING_ALLOWDIFFMODS "ALLOWMODS"
#define SETTING_MODSHASH "MODS"

struct FOnlineSessionUtils
{
	static bool GetSearchPresence(const FOnlineSearchSettings& SearchSettings)
	{
		auto bValue = false;
		SearchSettings.Get(SEARCH_PRESENCE, bValue);
		return bValue;
	}

	static void SetSearchPresence(FOnlineSearchSettings& SearchSettings, bool bSearchPresence)
	{
		SearchSettings.Set(SEARCH_PRESENCE, bSearchPresence, EOnlineComparisonOp::Equals);
	}

	static bool GetAllowDifferentMods(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<bool>(SessionSettings, SETTING_ALLOWDIFFMODS);
	}

	static void SetAllowDifferentMods(FOnlineSessionSettings& SessionSettings, bool bAllow)
	{
		Set<bool>(SessionSettings, SETTING_ALLOWDIFFMODS, bAllow);
	}

	static bool GetSearchAllowDifferentMods(const FOnlineSearchSettings& SearchSettings)
	{
		auto bValue = false;
		SearchSettings.Get(SETTING_ALLOWDIFFMODS, bValue);
		return bValue;
	}

	static void SetSearchAllowDifferentMods(FOnlineSearchSettings& SearchSettings, bool bAllow)
	{
		SearchSettings.Set(SETTING_ALLOWDIFFMODS, bAllow, EOnlineComparisonOp::Equals);
	}

	static int32 GetModsHash(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<int32>(SessionSettings, SETTING_MODSHASH);
	}

	static void SetModsHash(FOnlineSessionSettings& SessionSettings, int32 ModsHash)
	{
		Set(SessionSettings, SETTING_MODSHASH, ModsHash);
	}

	static void SetSearchModsHash(FOnlineSearchSettings& SearchSettings, int32 ModsHash)
	{
		SearchSettings.Set(SETTING_MODSHASH, ModsHash, EOnlineComparisonOp::Equals);
	}

	static FString GetSessionName(const FOnlineSessionSettings& SessionSettings)
	{
		return SessionSettings.SessionName;
	}

	static void SetSessionName(FOnlineSessionSettings& SessionSettings, const FString& SessionName)
	{
		SessionSettings.SessionName = SessionName;
	}

	// Search a keyword within the name of a session
	static void SetSearchKeyword(FOnlineSearchSettings& SearchSettings, const FString& Keyword)
	{
		if (!Keyword.IsEmpty())
		{
			SearchSettings.Set(SEARCH_KEYWORDS, Keyword, EOnlineComparisonOp::Equals);
		}
	}

	static bool IsPasswordProtected(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<bool>(SessionSettings, SETTING_PASSWORDPROTECTED);
	}

	static void SetIsPasswordProtected(FOnlineSessionSettings& SessionSettings, bool bIsProtected)
	{
		Set<bool>(SessionSettings, SETTING_PASSWORDPROTECTED, bIsProtected);
	}

	static void SetSearchPasswordProtected(FOnlineSearchSettings& SearchSettings, bool bSearch)
	{
		if (!bSearch)
		{
			// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
			SearchSettings.Set<int32>(SETTING_PASSWORDPROTECTED, 0, EOnlineComparisonOp::Equals);
		}
	}

	static FString GetLevelName(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_MAPNAME);
	}

	static void SetLevel(FOnlineSessionSettings& SessionSettings, const ULevelInfo* Level);

	static FString GetGameModeName(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_GAMEMODE);
	}

	static void SetGameMode(FOnlineSessionSettings& SessionSettings, const UGameModeInfo* GameMode);

	static FString GetSessionDescription(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_DESCRIPTION);
	}

	static void SetSessionDescription(FOnlineSessionSettings& SessionSettings, const FString& Description)
	{
		if (!Description.IsEmpty())
		{
			Set(SessionSettings, SETTING_DESCRIPTION, Description);
		}
	}

	static int32 GetPlayerSlots(const FOnlineSessionSettings& SessionSettings)
	{
		return SessionSettings.NumPublicConnections;
	}

	static int32 GetPlayerCount(const FOnlineSession& Session)
	{
		return FMath::Max(GetPlayerSlots(Session.SessionSettings) - Session.NumOpenPublicConnections, 0);
	}

	static int32 GetAverageFrameRate(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<int32>(SessionSettings, SETTING_FRAMERATE);
	}

	static void SetAverageFrameRate(FOnlineSessionSettings& SessionSettings, int32 FrameRate)
	{
		Set(SessionSettings, SETTING_FRAMERATE, FrameRate);
	}

	// Whether the session shows up in the server browser
	static bool IsPublicSession(const FOnlineSessionSettings& SessionSettings)
	{
		return !SessionSettings.bAllowJoinViaPresenceFriendsOnly;
	}

	static void SetIsPublicSession(FOnlineSessionSettings& SessionSettings, bool bPublic)
	{
		SessionSettings.bAllowJoinViaPresenceFriendsOnly = !bPublic;
	}

private:
	// Helper function to set a session setting
	template <typename T>
	static void Set(FOnlineSessionSettings& SessionSettings, const FName& Key, const T Value)
	{
		SessionSettings.Set<T>(Key, Value, EOnlineDataAdvertisementType::ViaOnlineService);
	}

	template <bool>
	static void Set(FOnlineSessionSettings& SessionSettings, const FName& Key, const bool bValue)
	{
		// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
		Set<int32>(SessionSettings, Key, bValue ? 1 : 0);
	}

	// Helper function to get a session setting
	template <typename T>
	static T Get(const FOnlineSessionSettings& SessionSettings, const FName& Key)
	{
		auto Value = T();
		SessionSettings.Get<T>(Key, Value);
		return Value;
	}

	template <bool>
	static bool Get(const FOnlineSessionSettings& SessionSettings, const FName& Key)
	{
		// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
		return Get<int32>(SessionSettings, Key) != 0;
	}
};
