#pragma once

#include "OnlineSessionUtils.h"
#include "UI/Misc/PagedListEntry.h"
#include "CoreMinimal.h"
#include "BrickOnlineSessionInfo.generated.h"

class UBrickDataSingleton;
class ULevelInfo;
class UGameModeInfo;

USTRUCT(BlueprintType)
struct FBrickOnlineSessionInfo : public FPagedListEntry
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString SessionName;
	UPROPERTY(BlueprintReadOnly)
	FString SessionDescription;
	UPROPERTY(BlueprintReadOnly)
	ULevelInfo* Level = nullptr;
	UPROPERTY(BlueprintReadOnly)
	UGameModeInfo* GameMode = nullptr;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 Ping = 0;
	UPROPERTY(BlueprintReadOnly)
	int32 AverageFrameRate = 0;
	UPROPERTY(BlueprintReadOnly)
	bool bIsDedicated = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsPasswordProtected = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsCompatibleBuild = false;
	UPROPERTY(BlueprintReadOnly)
	bool bHasMods = false;
	UPROPERTY(BlueprintReadOnly)
	bool bHasSameMods = false;
	UPROPERTY(BlueprintReadOnly)
	bool bAllowDifferentMods = false;
	// The associated search result, needed for joining the session
	FOnlineSessionSearchResult SearchResult;

	// ~Constructor
	FBrickOnlineSessionInfo() = default;
	// ~Destructor
	virtual ~FBrickOnlineSessionInfo() override = default;

	// ~Constructor
	FBrickOnlineSessionInfo(const FOnlineSessionSearchResult& InResult);

	// ~Super Interface
	virtual UStruct* GetStruct() const override
	{
		return StaticStruct();
	}

	virtual FText GetDisplayName() const override;
	virtual void ReflectBrickProperties(FBrickPropertyReflection& Params) const override;
	// ~Super Interface
};
