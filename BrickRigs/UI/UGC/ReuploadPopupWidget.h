// Copyright Fluppisoft, 2020

#pragma once

#include "UGC/UGCTypes.h"
#include "CoreMinimal.h"
#include "UI/Menu/Popups/PopupWidget.h"
#include "ReuploadPopupWidget.generated.h"

class IOnlineUser;

UCLASS()
class BRICKRIGS_API UReuploadPopupParams : public UPopupParams
{
	GENERATED_BODY()

public:
	// ~Variables
	FUGCFileInfo FileInfo;
	// ~Variables
};

/**
 * This popup is used to warn the user about reuploading someone else's creation
 */
UCLASS(Abstract)
class BRICKRIGS_API UReuploadPopupWidget : public UPopupWidget
{
	GENERATED_BODY()

private:
	// ~Variables
	FDelegateHandle DelegateHandle_QueryUserInfo;
	// The online user interface we are using to get the author info
	TWeakPtr<IOnlineUser, ESPMode::ThreadSafe> AuthorUserInterface;
	// The cached name of the author
	FText AuthorName;
	// ~Variables

public:
	// ~Super Interface
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// ~Super Interface

	// Opens the profile page of the author
	UFUNCTION(BlueprintCallable)
	void ViewOriginalAuthorInBrowser();
	// Used to open a link to the original creation
	UFUNCTION(BlueprintCallable)
	void ViewOriginalItemInBrowser();

protected:
	// Allows the blueprint to access the author name
	UFUNCTION(BlueprintPure)
	bool GetOriginalAuthorName(FText& OutName) const;

private:
	// Cancels the author info query and unbinds the delegate
	void CancelAuthorInfoQuery();
	// Callback for the user info query
	void OnQueryUserInfoComplete(int32 LocalUserNum, bool bWasSuccessful, const TArray<TSharedRef<const FUniqueNetId>>& UserIds, const FString& ErrorStr);
};
