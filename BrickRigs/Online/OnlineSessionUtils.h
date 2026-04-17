#pragma once

#include "OnlineSessionSettings.h"
#include "CoreMinimal.h"

class ULevelInfo;
class UGameModeInfo;

#define SETTING_PASSWORDPROTECTED "PASSWORD"
#define SETTING_DESCRIPTION "DESCR"
#define SETTING_FRAMERATE "FPS"
// Whether players with different mods are allowed to join
#define SETTING_ALLOWDIFFMODS "ALLOWMODS"
#define SETTING_MODSHASH "MODS"

namespace FOnlineSessionUtils
{
	// Helper function to set a session setting
	template <typename T>
	void Set(FOnlineSessionSettings& SessionSettings, const FName& Key, const T Value)
	{
		SessionSettings.Set<T>(Key, Value, EOnlineDataAdvertisementType::ViaOnlineService);
	}

	template <>
	inline void Set<bool>(FOnlineSessionSettings& SessionSettings, const FName& Key, const bool bValue)
	{
		// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
		Set<int32>(SessionSettings, Key, bValue ? 1 : 0);
	}

	// Helper function to get a session setting
	template <typename T>
	T Get(const FOnlineSessionSettings& SessionSettings, const FName& Key)
	{
		auto Value = T();
		SessionSettings.Get<T>(Key, Value);
		return Value;
	}

	template <>
	inline bool Get<bool>(const FOnlineSessionSettings& SessionSettings, const FName& Key)
	{
		// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
		return Get<int32>(SessionSettings, Key) != 0;
	}

	// Helper function to set a search setting
	template <typename T>
	void Set(FOnlineSearchSettings& SearchSettings, const FName& Key, const T Value)
	{
		SearchSettings.Set<T>(Key, Value, EOnlineComparisonOp::Equals);
	}

	template <>
	inline void Set<bool>(FOnlineSearchSettings& SearchSettings, const FName& Key, const bool Value)
	{
		// NOTE: We have to use an int setting instead of bool since steam doesn't support search queries with booleans
		Set<int32>(SearchSettings, Key, Value ? 1 : 0);
	}

	inline bool GetSearchPresence(const FOnlineSearchSettings& SearchSettings)
	{
		// IMPORTANT: Don't use our custom Get function as this has to be a setting of type BOOL for the Steam subsystem
		auto bValue = false;
		SearchSettings.Get(SEARCH_PRESENCE, bValue);
		return bValue;
	}

	inline void SetSearchPresence(FOnlineSearchSettings& SearchSettings, bool bSearchPresence)
	{
		// IMPORTANT: Don't use our custom Set function as this has to be a setting of type BOOL for the Steam subsystem
		SearchSettings.Set(SEARCH_PRESENCE, bSearchPresence, EOnlineComparisonOp::Equals);
	}

	inline bool GetAllowDifferentMods(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<bool>(SessionSettings, SETTING_ALLOWDIFFMODS);
	}

	inline void SetAllowDifferentMods(FOnlineSessionSettings& SessionSettings, bool bAllow)
	{
		Set<bool>(SessionSettings, SETTING_ALLOWDIFFMODS, bAllow);
	}

	inline bool GetSearchAllowDifferentMods(const FOnlineSearchSettings& SearchSettings)
	{
		auto bValue = false;
		SearchSettings.Get(SETTING_ALLOWDIFFMODS, bValue);
		return bValue;
	}

	inline void SetSearchAllowDifferentMods(FOnlineSearchSettings& SearchSettings, bool bAllow)
	{
		Set(SearchSettings, SETTING_ALLOWDIFFMODS, bAllow);
	}

	inline int32 GetModsHash(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<int32>(SessionSettings, SETTING_MODSHASH);
	}

	inline void SetModsHash(FOnlineSessionSettings& SessionSettings, int32 ModsHash)
	{
		Set(SessionSettings, SETTING_MODSHASH, ModsHash);
	}

	inline void SetSearchModsHash(FOnlineSearchSettings& SearchSettings, int32 ModsHash)
	{
		Set(SearchSettings, SETTING_MODSHASH, ModsHash);
	}

	inline FString GetSessionName(const FOnlineSessionSettings& SessionSettings)
	{
		return "Unknown Session";
	}

	inline void SetSessionName(FOnlineSessionSettings& SessionSettings, const FString& SessionName)
	{
	}

	// Search a keyword within the name of a session
	inline void SetSearchKeyword(FOnlineSearchSettings& SearchSettings, const FString& Keyword)
	{
		if (!Keyword.IsEmpty())
		{
			Set(SearchSettings, SEARCH_KEYWORDS, Keyword);
		}
	}

	inline bool IsPasswordProtected(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<bool>(SessionSettings, SETTING_PASSWORDPROTECTED);
	}

	inline void SetIsPasswordProtected(FOnlineSessionSettings& SessionSettings, bool bIsProtected)
	{
		Set<bool>(SessionSettings, SETTING_PASSWORDPROTECTED, bIsProtected);
	}

	// NOTE: If this is set, either only password protected or non protected servers will be found
	inline void SetSearchPasswordProtected(FOnlineSearchSettings& SearchSettings, bool bSearch)
	{
		Set(SearchSettings, SETTING_PASSWORDPROTECTED, bSearch);
	}

	inline FString GetLevelName(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_MAPNAME);
	}

	void SetLevel(FOnlineSessionSettings& SessionSettings, const ULevelInfo* Level);

	inline FString GetGameModeName(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_GAMEMODE);
	}

	void SetGameMode(FOnlineSessionSettings& SessionSettings, const UGameModeInfo* GameMode);

	inline FString GetSessionDescription(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<FString>(SessionSettings, SETTING_DESCRIPTION);
	}

	inline void SetSessionDescription(FOnlineSessionSettings& SessionSettings, const FString& Description)
	{
		if (!Description.IsEmpty())
		{
			Set(SessionSettings, SETTING_DESCRIPTION, Description);
		}
	}

	inline int32 GetPlayerSlots(const FOnlineSessionSettings& SessionSettings)
	{
		return SessionSettings.NumPublicConnections;
	}

	inline int32 GetPlayerCount(const FOnlineSession& Session)
	{
		return FMath::Max(GetPlayerSlots(Session.SessionSettings) - Session.NumOpenPublicConnections, 0);
	}

	inline int32 GetAverageFrameRate(const FOnlineSessionSettings& SessionSettings)
	{
		return Get<int32>(SessionSettings, SETTING_FRAMERATE);
	}

	inline void SetAverageFrameRate(FOnlineSessionSettings& SessionSettings, int32 FrameRate)
	{
		Set(SessionSettings, SETTING_FRAMERATE, FrameRate);
	}

	// Whether the session shows up in the server browser
	inline bool IsPublicSession(const FOnlineSessionSettings& SessionSettings)
	{
		return !SessionSettings.bAllowJoinViaPresenceFriendsOnly;
	}

	inline void SetIsPublicSession(FOnlineSessionSettings& SessionSettings, bool bPublic)
	{
		SessionSettings.bAllowJoinViaPresenceFriendsOnly = !bPublic;
	}
};
