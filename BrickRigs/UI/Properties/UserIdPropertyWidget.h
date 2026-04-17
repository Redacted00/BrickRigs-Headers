// Copyright Fluppisoft, 2020

#pragma once

#include "Interfaces/OnlineUserInterface.h"
#include "CoreMinimal.h"
#include "UI/Properties/PropertyWidget.h"
#include "UserIdPropertyWidget.generated.h"

/**
 * 
 */
UCLASS()
class BRICKRIGS_API UUserIdPropertyWidget : public UPropertyWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	FDelegateHandle DelegateHandle_QueryUserInfo;
	// Cached pointer to the user interface we bound our delegate to
	TWeakPtr<IOnlineUser, ESPMode::ThreadSafe> UserInterfacePtr;
	// The cached user id being displayed
	FUniqueNetIdRepl UserId;
	// ~Variables

public:
	// ~Super Interface
	virtual bool Initialize() override;
	virtual void UninitializeProperty() override;
	virtual void UpdateValue() override;
	virtual void GetSupportedPropertyTypes(TArray<FName>& OutTypes) const override;
	virtual void UpdateIsReadOnly_Implementation(bool bNewReadOnly) override;
	// ~Super Interface

	// Opens the player profile in the browser/overlay
	UFUNCTION(BlueprintCallable)
	void ViewProfileInBrowser();

protected:
	// ~Blueprint Interface
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateUsername(const FText& InUsername);
	// ~Blueprint Interface

private:
	// Clears the online user interface delegate
	void ClearUserInfoDelegate();
	// To be called when the user info is ready
	void OnUserInfoReady(const TSharedRef<FOnlineUser>& UserInfo);
	// Callback for the user info query
	void OnQueryUserInfoComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<TSharedRef<const FUniqueNetId>>& UserIds, const FString& ErrorStr);
};
