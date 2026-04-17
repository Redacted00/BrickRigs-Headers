#pragma once

#include "OnlineSubsystemTypes.h"
#include "UObject/CoreOnline.h"
#include "CoreMinimal.h"

class IOnlineUser;

enum class ESmartUserInfoState : uint8
{
	InProgress,
	AlreadyLoaded,
	Finished
};

DECLARE_DELEGATE_TwoParams(FSmartUserInfoDelegate, const TSharedPtr<FOnlineUser>&, const ESmartUserInfoState);

struct FSmartUserInfoHandle
{
	~FSmartUserInfoHandle()
	{
		CancelQuery();
	}

	void QueryUserInfo(const FUniqueNetIdWrapper& InUserId, const FSmartUserInfoDelegate& InDelegate);

	void CancelQuery();

private:
	// Callback for the user info query
	void OnQueryUserInfoComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<TSharedRef<const FUniqueNetId>>& UserIds, const FString& ErrorStr);

	// ~Variables
	FUniqueNetIdWrapper UserId;
	// Cached pointer to the user interface we bound our delegate to
	TWeakPtr<IOnlineUser, ESPMode::ThreadSafe> UserInterfacePtr;
	FDelegateHandle DelegateHandle;
	FSmartUserInfoDelegate Delegate;
	// ~Variables
};
